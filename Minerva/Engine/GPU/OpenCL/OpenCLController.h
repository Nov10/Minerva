#pragma once
#define CL_TARGET_OPENCL_VERSION 220
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <CL/cl.h>
#include "../GPUController.h"

class OpenCLController : public GPUController
{
public:
	//OpenCLController(const OpenCLController& other) = delete;
	//OpenCLController& operator=(const OpenCLController& rhs) = delete;
	virtual void Initialize();
	virtual void Release();

	std::string LoadKernelSource(const std::string& filename);
	virtual int LoadKernelProgram(std::string kernelFilePath);
	virtual int LoadKernelProgram(std::string* kernelFilePaths, int  programLength);
	virtual int LoadKernel(const char* kernelName, int programID);

	virtual int CreateBuffer(Flag_Buffer_Creation flag, size_t size, void* host_ptr);
	virtual void ReleaseBuffer(int id);
	virtual void* GetBufferPtr(const int bufferID);
	virtual void ReadBuffer(int bufferID, int size, void* destination);
	virtual void SetBufferDatas(int bufferID, const void* value, size_t size);

	virtual void InitializeKernelArguments(int kernelID, int length, size_t* sizes, const void** args);
	virtual void ExecuteKernel(int kernelID, int length, int argumentLength, size_t* sizes, const void** args);

	virtual void Finish();


private:
	std::map<int, cl_kernel> kernels;
	std::map<int, cl_program> programs;
	std::map<int, cl_mem> buffers;
	cl_command_queue queue;
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
private:
	cl_kernel GetKernel(int kernelID);
	cl_mem GetBuffer(const int bufferID);
};