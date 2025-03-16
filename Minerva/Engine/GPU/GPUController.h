#pragma once
typedef unsigned __int64 Flag_Buffer_Creation;
#define FLAG_BUFFER_READ_WRITE                           (1 << 0)
#define FLAG_BUFFER_WRITE_ONLY                           (1 << 1)
#define FLAG_BUFFER_READ_ONLY                            (1 << 2)
#define FLAG_BUFFER_USE_HOST_PTR                         (1 << 3)
#define FLAG_BUFFER_ALLOC_HOST_PTR                       (1 << 4)
#define FLAG_BUFFER_COPY_HOST_PTR                        (1 << 5)
/* reserved                                         (1 << 6)    */
#define FLAG_BUFFER_HOST_WRITE_ONLY                      (1 << 7)
#define FLAG_BUFFER_HOST_READ_ONLY                       (1 << 8)
#define FLAG_BUFFER_HOST_NO_ACCESS                       (1 << 9)
#define FLAG_BUFFER_SVM_FINE_GRAIN_BUFFER                (1 << 10)   /* used by cl_svm_mem_flags only */
#define FLAG_BUFFER_SVM_ATOMICS                          (1 << 11)   /* used by cl_svm_mem_flags only */
#define FLAG_BUFFER_KERNEL_READ_AND_WRITE                (1 << 12)

class GPUController
{
public:
	//GPUController(const GPUController& other) = delete;
	//GPUController& operator=(const GPUController& rhs) = delete;
	virtual int LoadKernelProgram(std::string kernelFilePath) {
		  return -1;
	}
	virtual int LoadKernelProgram(std::string* kernelFilePath, int  programLength) {
		return -1;
	}
	virtual int LoadKernel(const char* kernelName, int programID) { return -1; }

	virtual int CreateBuffer(Flag_Buffer_Creation flag, size_t size, void* host_ptr) { return -1; }
	virtual void ReleaseBuffer(int id) {}
	virtual void* GetBufferPtr(const int bufferID) { return nullptr; }
	virtual void ReadBuffer(int bufferID, int size, void* destination) {}
	virtual void SetBufferDatas(int bufferID, const void* value, size_t size) {}

	virtual void InitializeKernelArguments(int kernelID, int length, size_t* sizes, const void** args) {}
	virtual void ExecuteKernel(int kernelID, int length, int argumentLength, size_t* sizes, const void** args) {}
	virtual void Initialize() { }
	virtual void Release() {}

	virtual void Finish() {}

protected:
	int ProgramIDCounter;
	int KernelIDCounter;
	int BufferIDCounter;
};


extern GPUController* GController;
#define GPUControl (*GController)