#include "OpenCLController.h"
#include <CL/cl.h>

void OpenCLController::Initialize() {
	Release();
	ProgramIDCounter = 0;
	KernelIDCounter = 0;
	BufferIDCounter = 0;
	cl_int err;
	err = clGetPlatformIDs(1, &platform, nullptr);
	if (err != CL_SUCCESS) {
		throw std::runtime_error("Failed to get OpenCL platform.");
	}

	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
	if (err != CL_SUCCESS) {
		throw std::runtime_error("Failed to get OpenCL device.");
	}

	context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
	if (!context || err != CL_SUCCESS) {
		throw std::runtime_error("Failed to create OpenCL context.");
	}

	queue = clCreateCommandQueueWithProperties(context, device, nullptr, &err);
	if (!queue || err != CL_SUCCESS) {
		throw std::runtime_error("Failed to create OpenCL command queue.");
	}
}
void OpenCLController::Release()
{
	for (auto& p : programs)
	{
		clReleaseProgram(p.second);
	}
	for (auto& k : kernels)
	{
		clReleaseKernel(k.second);
	}
	programs.clear();
	kernels.clear();
	if (queue) clReleaseCommandQueue(queue);
	if (context) clReleaseContext(context);
	if (device) clReleaseDevice(device);
}

std::string OpenCLController::LoadKernelSource(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("Failed to open kernel file: " + filename);
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

int OpenCLController::LoadKernelProgram(std::string kernelFilePath)
{
	cl_int err;
	std::string kernelSource = LoadKernelSource(kernelFilePath);
	const char* sourceStr = kernelSource.c_str();
	size_t sourceSize = kernelSource.size();
	cl_program program = clCreateProgramWithSource(context, 1, &sourceStr, &sourceSize, &err);
	if (!program || err != CL_SUCCESS) {
		throw std::runtime_error("Failed to create OpenCL program.");
	}

	err = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
	if (err != CL_SUCCESS) {
		size_t logSize;
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);
		std::vector<char> buildLog(logSize);
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, logSize, buildLog.data(), nullptr);
		std::cerr << "OpenCL Build Error:\n" << buildLog.data() << std::endl;
		clReleaseProgram(program);
		throw std::runtime_error("OpenCL program build failed.");
	}
	ProgramIDCounter++;
	programs.insert({ ProgramIDCounter , program });
	return ProgramIDCounter;
}
int OpenCLController::LoadKernelProgram(std::string* kernelFilePaths, int programLength)
{
	cl_int err;
	std::stringstream buffer;
	for (int i = 0; i < programLength; i++)
	{
		std::string source = LoadKernelSource(kernelFilePaths[i]);
		buffer << source;
	}

	std::string kernelSource = buffer.str();
	const char* sourceStr = kernelSource.c_str();
	
	size_t sourceSize = kernelSource.size();
	cl_program program = clCreateProgramWithSource(context, 1, &sourceStr, &sourceSize, &err);
	if (!program || err != CL_SUCCESS) {
		throw std::runtime_error("Failed to create OpenCL program.");
	}

	err = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
	if (err != CL_SUCCESS) {
		size_t logSize;
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);
		std::vector<char> buildLog(logSize);
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, logSize, buildLog.data(), nullptr);
		std::cerr << "OpenCL Build Error:\n" << buildLog.data() << std::endl;
		clReleaseProgram(program);
		throw std::runtime_error("OpenCL program build failed.");
	}
	ProgramIDCounter++;
	programs.insert({ ProgramIDCounter , program });
	return ProgramIDCounter;
}
int OpenCLController::LoadKernel(const char* kernelName, int programID)
{
	if (programs.find(programID) == programs.end()) {
		throw std::runtime_error("Invalid program ID.");
	}

	cl_int err;
	auto p = programs[programID];
	cl_kernel kernel = clCreateKernel(p, kernelName, &err);
	if (!kernel || err != CL_SUCCESS) {
		throw std::runtime_error("Failed to create OpenCL kernel.");
	}

	KernelIDCounter++;
	kernels.insert({ KernelIDCounter , kernel });
	return KernelIDCounter;
}

cl_mem_flags ConvertFlag2OpenCL(Flag_Buffer_Creation flag)
{
	cl_mem_flags result = 0;
	if (0 != (flag & FLAG_BUFFER_READ_WRITE))				result = result | CL_MEM_READ_WRITE;
	if (0 != (flag & FLAG_BUFFER_WRITE_ONLY))				result = result | CL_MEM_WRITE_ONLY;
	if (0 != (flag & FLAG_BUFFER_READ_ONLY))				result = result | CL_MEM_READ_ONLY;
	if (0 != (flag & FLAG_BUFFER_USE_HOST_PTR))				result = result | CL_MEM_USE_HOST_PTR;
	if (0 != (flag & FLAG_BUFFER_ALLOC_HOST_PTR))			result = result | CL_MEM_ALLOC_HOST_PTR;
	if (0 != (flag & FLAG_BUFFER_COPY_HOST_PTR))			result = result | CL_MEM_COPY_HOST_PTR;
	if (0 != (flag & FLAG_BUFFER_HOST_WRITE_ONLY))			result = result | CL_MEM_HOST_WRITE_ONLY;
	if (0 != (flag & FLAG_BUFFER_HOST_READ_ONLY))			result = result | CL_MEM_HOST_READ_ONLY;
	if (0 != (flag & FLAG_BUFFER_HOST_NO_ACCESS))			result = result | CL_MEM_HOST_NO_ACCESS;
	if (0 != (flag & FLAG_BUFFER_SVM_FINE_GRAIN_BUFFER))	result = result | CL_MEM_SVM_FINE_GRAIN_BUFFER;
	if (0 != (flag & FLAG_BUFFER_SVM_ATOMICS))				result = result | CL_MEM_SVM_ATOMICS;
	if (0 != (flag & FLAG_BUFFER_KERNEL_READ_AND_WRITE))	result = result | CL_MEM_KERNEL_READ_AND_WRITE;
	return result;

}
int OpenCLController::CreateBuffer(Flag_Buffer_Creation flag, size_t size, void* host_ptr)
{

	size_t maxAllocSize;
	clGetDeviceInfo(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(size_t), &maxAllocSize, NULL);
	if (size > maxAllocSize) {
		throw std::runtime_error("Error: Buffer 크기가 CL_DEVICE_MAX_MEM_ALLOC_SIZE를 초과함!");
		return -1;
	}

	cl_int err;
	auto buffer = clCreateBuffer(context, ConvertFlag2OpenCL(flag), size, host_ptr, &err);
	if (err != CL_SUCCESS) {
		throw std::runtime_error("ERROR");
	}
	BufferIDCounter++;
	buffers.insert({ BufferIDCounter , buffer });
	return BufferIDCounter;
}
void OpenCLController::ReleaseBuffer(int id)
{
	clReleaseMemObject(GetBuffer(id));
}
void* OpenCLController::GetBufferPtr(const int bufferID)
{
	return &(buffers[bufferID]);
}
void OpenCLController::ReadBuffer(int bufferID, int size, void* destination)
{
	cl_int err = clEnqueueReadBuffer(queue, GetBuffer(bufferID), CL_TRUE, 0, size, destination, 0, nullptr, nullptr);
	if (err != CL_SUCCESS) {
		 //throw std::runtime_error("ERROR");
	}
}
void OpenCLController::Finish()
{
	//clFlush(queue);
	clFinish(queue);
}
void OpenCLController::SetBufferDatas(int bufferID, const void* value, size_t size)
{
	cl_int err;
	err = clEnqueueFillBuffer(queue, GetBuffer(bufferID), value, sizeof(int), 0, size, 0, nullptr, nullptr);
	if (err != CL_SUCCESS) {
		throw std::runtime_error("ERROR");
	}
}

void OpenCLController::InitializeKernelArguments(int kernelID, int length, size_t* sizes, const void** args)
{
	auto kernel = GetKernel(kernelID);
	for (int idx = 0; idx < length; idx++)
	{
		auto t = sizes[idx];
		cl_int err = clSetKernelArg(kernel, idx, t, args[idx]);
		if (err != CL_SUCCESS) {
			throw std::runtime_error("ERROR");
		}
	}
}
void OpenCLController::ExecuteKernel(int kernelID, int length, int argumentLength, size_t* sizes, const void** args)
{
	size_t globalSize = length;
	InitializeKernelArguments(kernelID, argumentLength, sizes, args);
	
	cl_int err = clEnqueueNDRangeKernel(queue, GetKernel(kernelID), 1, nullptr, &globalSize, nullptr, 0, nullptr, nullptr);
	clFinish(queue);
	// err = clEnqueueTask(queue, GetKernel(kernelID), 1, nullptr, &globalSize, nullptr, 0, nullptr, nullptr);
	if (err != CL_SUCCESS) {
		throw std::runtime_error("ERROR");
	}

	
}


cl_kernel OpenCLController::GetKernel(int kernelID)
{
	return kernels[kernelID];
}
cl_mem OpenCLController::GetBuffer(const int bufferID)
{
	return buffers[bufferID];
}