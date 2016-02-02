/*
File: OCLWrapper.h
Class: OCLWrapper
Author: Hugo Tunius
Web: http://hugotunius.se
Date: May 2012

Description:
The class contained in this file is a naive wrapper for basic opencl functions, it's in no way a complete
library and is only tested under very special cases of OpenCL usage.

The purpose of the wrapper is to create a simple way to manage memory, read, execute and build the kernel source code.

Lisence:
Copyright (C) 2012 Hugo Tunius

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef OCLWRAPPER_H
#define OCLWRAPPER_H

#include <CL/cl.h>
#include <vector>
#include <fstream>
#include <string>

using std::ifstream;
using std::vector;
using std::ios;
using std::string;

typedef unsigned int uint;
/* Opencl error codes */
static const string errors[] = {"CL_SUCCESS",
    "CL_DEVICE_NOT_FOUND",
    "CL_DEVICE_NOT_AVAILABLE",
    "CL_COMPILER_NOT_AVAILABLE",
    "CL_MEM_OBJECT_ALLOCATION_FAILURE",
    "CL_OUT_OF_RESOURCES",
    "CL_OUT_OF_HOST_MEMORY",
    "CL_PROFILING_INFO_NOT_AVAILABLE",
    "CL_MEM_COPY_OVERLAP",
    "CL_IMAGE_FORMAT_MISMATCH",
    "CL_IMAGE_FORMAT_NOT_SUPPORTED",
    "CL_BUILD_PROGRAM_FAILURE",
    "CL_MAP_FAILURE",
    "CL_MISALIGNED_SUB_BUFFER_OFFSET",
    "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST",

    "CL_INVALID_VALUE",
    "CL_INVALID_DEVICE_TYPE",
    "CL_INVALID_PLATFORM",
    "CL_INVALID_DEVICE",
    "CL_INVALID_CONTEXT",
    "CL_INVALID_QUEUE_PROPERTIES",
    "CL_INVALID_COMMAND_QUEUE",
    "CL_INVALID_HOST_PTR",
    "CL_INVALID_MEM_OBJECT",
    "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
    "CL_INVALID_IMAGE_SIZE",
    "CL_INVALID_SAMPLER",
    "CL_INVALID_BINARY",
    "CL_INVALID_BUILD_OPTIONS",
    "CL_INVALID_PROGRAM",
    "CL_INVALID_PROGRAM_EXECUTABLE",
    "CL_INVALID_KERNEL_NAME",
    "CL_INVALID_KERNEL_DEFINITION",
    "CL_INVALID_KERNEL",
    "CL_INVALID_ARG_INDEX",
    "CL_INVALID_ARG_VALUE",
    "CL_INVALID_ARG_SIZE",
    "CL_INVALID_KERNEL_ARGS",
    "CL_INVALID_WORK_DIMENSION",
    "CL_INVALID_WORK_GROUP_SIZE",
    "CL_INVALID_WORK_ITEM_SIZE",
    "CL_INVALID_GLOBAL_OFFSET",
    "CL_INVALID_EVENT_WAIT_LIST",
    "CL_INVALID_EVENT",
    "CL_INVALID_OPERATION",
    "CL_INVALID_GL_OBJECT",
    "CL_INVALID_BUFFER_SIZE",
    "CL_INVALID_MIP_LEVEL",
    "CL_INVALID_GLOBAL_WORK_SIZE",
    "CL_INVALID_PROPERTY"};

class OCLWrapper {
public:
	/*Constructor
		Arguments:
			char* src Kernel source code location
		Example 
			OCLWrapper("kernel.cl");
	*/
    OCLWrapper (char*);

	/*Destructor
		Free memory buffers and other opencl types
	*/
    ~OCLWrapper ();

	/*freeMemory
		Description: Free memory resources 
	*/
    void freeMemory ();

    /*addArgument
		Add an argument to the kernel. 
		Arguments must be added in the same order on the host as the device!

		Arguments:
			void* t A pointer to the argument
			uint length The size in bytes of the data
	*/
	void addArgument (void*, uint length);

	/*addReturn
		Prepare the return from the kernel, should be the last arguments to the kernel.

		Arguments:
			uint length Size in bytes of the return data
	*/
    void addReturn (uint length);

	/*build
		Buid the kernel. If the build fails the build log is availble in buildLog and the failed variable will be set.

		Arguments:
			char* kernelName The name of the kernel entry point
		Return:
			bool - The result of the build
	*/
    bool build (char*);

	/*execute
		Run the kernel once(1) on the device, results after the kernel has run is by using readMemory

		Arguments:
			size_t work_size Opencl work size
			size_t local_size Opencl local size

	*/
    void execute (size_t, size_t);

	/*readMemory
		Read memory back from the device, used to aquire the return value after the kernel has ran

		Arguments:
			uint size The size of the data being read
			uint arg  The index of the argument being read
			void* t	  A pointer to the location of the data 

		Example:
			For the kernel __kernel void kernel_entry(__constant struct B* b, __constant Map* map,__global  Map* R)
			reading R from the memory is done by this call 
				ocl->readMemory(sizeof(Map)*numberOfElementsInMapArray, 2, &map);
	*/
    int readMemory (uint, uint, void*);
    

    /* Getters */
    char* getSource ();
	bool isBuilt ();
    bool isDone ();
	/*buildLog
		Returns any errors that occured during build
	*/
    char* buildLog ();
    /*Setters*/
    void setDebug (bool);
	void setArgument (void*, uint, uint);


private:
    bool failed;
    //True if the kernel has been ran at least once
    bool ran;
    //True if the kernel has been built successfully
    bool built;
    //Output debug to std out
    bool printDebug;

    //opencl variables
    cl_platform_id platform_ids;
    cl_uint num_platforms;
    cl_uint num_devices;
    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;
    cl_program program;
    cl_kernel kernel;
    //Arguments to the kernel
    vector<cl_mem> arguments;
    //Return values from the kernel
    vector<cl_mem> return_values;

    /* Related to reading the source code */
    char* source;
    //Length of source array
    int srclength;

    void handleError (cl_int, int);
	void readSource(char* file);
};




#endif