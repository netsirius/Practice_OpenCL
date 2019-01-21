// Practice3.3.cpp: define el punto de entrada de la aplicación de consola.
//
#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <CL/cl.h>

using namespace std;

#pragma comment(lib, "OpenCL.lib")

char* getDescriptionOfError(cl_int error) {
	switch (error) {
	case CL_SUCCESS:                            return "Success!";
	case CL_DEVICE_NOT_FOUND:                   return "Device not found.";
	case CL_DEVICE_NOT_AVAILABLE:               return "Device not available";
	case CL_COMPILER_NOT_AVAILABLE:             return "Compiler not available";
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:      return "Memory object allocation failure";
	case CL_OUT_OF_RESOURCES:                   return "Out of resources";
	case CL_OUT_OF_HOST_MEMORY:                 return "Out of host memory";
	case CL_PROFILING_INFO_NOT_AVAILABLE:       return "Profiling information not available";
	case CL_MEM_COPY_OVERLAP:                   return "Memory copy overlap";
	case CL_IMAGE_FORMAT_MISMATCH:              return "Image format mismatch";
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:         return "Image format not supported";
	case CL_BUILD_PROGRAM_FAILURE:              return "Program build failure";
	case CL_MAP_FAILURE:                        return "Map failure";
	case CL_INVALID_VALUE:                      return "Invalid value";
	case CL_INVALID_DEVICE_TYPE:                return "Invalid device type";
	case CL_INVALID_PLATFORM:                   return "Invalid platform";
	case CL_INVALID_DEVICE:                     return "Invalid device";
	case CL_INVALID_CONTEXT:                    return "Invalid context";
	case CL_INVALID_QUEUE_PROPERTIES:           return "Invalid queue properties";
	case CL_INVALID_COMMAND_QUEUE:              return "Invalid command queue";
	case CL_INVALID_HOST_PTR:                   return "Invalid host pointer";
	case CL_INVALID_MEM_OBJECT:                 return "Invalid memory object";
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:    return "Invalid image format descriptor";
	case CL_INVALID_IMAGE_SIZE:                 return "Invalid image size";
	case CL_INVALID_SAMPLER:                    return "Invalid sampler";
	case CL_INVALID_BINARY:                     return "Invalid binary";
	case CL_INVALID_BUILD_OPTIONS:              return "Invalid build options";
	case CL_INVALID_PROGRAM:                    return "Invalid program";
	case CL_INVALID_PROGRAM_EXECUTABLE:         return "Invalid program executable";
	case CL_INVALID_KERNEL_NAME:                return "Invalid kernel name";
	case CL_INVALID_KERNEL_DEFINITION:          return "Invalid kernel definition";
	case CL_INVALID_KERNEL:                     return "Invalid kernel";
	case CL_INVALID_ARG_INDEX:                  return "Invalid argument index";
	case CL_INVALID_ARG_VALUE:                  return "Invalid argument value";
	case CL_INVALID_ARG_SIZE:                   return "Invalid argument size";
	case CL_INVALID_KERNEL_ARGS:                return "Invalid kernel arguments";
	case CL_INVALID_WORK_DIMENSION:             return "Invalid work dimension";
	case CL_INVALID_WORK_GROUP_SIZE:            return "Invalid work group size";
	case CL_INVALID_WORK_ITEM_SIZE:             return "Invalid work item size";
	case CL_INVALID_GLOBAL_OFFSET:              return "Invalid global offset";
	case CL_INVALID_EVENT_WAIT_LIST:            return "Invalid event wait list";
	case CL_INVALID_EVENT:                      return "Invalid event";
	case CL_INVALID_OPERATION:                  return "Invalid operation";
	case CL_INVALID_GL_OBJECT:                  return "Invalid OpenGL object";
	case CL_INVALID_BUFFER_SIZE:                return "Invalid buffer size";
	case CL_INVALID_MIP_LEVEL:                  return "Invalid mip-map level";
	default: return "Unknown";
	}
}

const char *KernelSource = "\n" \
"#pragma OPENCL EXTENSION cl_khr_fp64 : enable \n" \
"__kernel void addVector(                                                  \n" \
"   __global double* input,                                          \n" \
"   __global double* input2,                                          \n" \
"   __global double* output,                                             \n" \
"   const unsigned int count)                                           \n" \
"{                                                                      \n" \
"   int i = get_global_id(0);                                           \n" \
"   if(i < count)                                                       \n" \
"       output[i] = input[i] + input2[i];                                \n" \
"}                                                                      \n" \
"\n";

bool useGPU = true;

cl_uint usePlatform = 1;
cl_uint useDevice = 1;

int main(void)
{
	cl_int error;
	int dataSize = 1024 * 1024;
	cl_double* data = new cl_double[dataSize];
	cl_double* data2 = new cl_double[dataSize];
	cl_double* results = new cl_double[dataSize];
	unsigned int correct;

	size_t global;
	size_t local;

	cl_platform_id platform_id;
	cl_device_id device_id;             // compute device id
	cl_context context;                 // compute context
	cl_command_queue commands;          // compute command queue
	cl_program program;                 // compute program
	cl_kernel kernel;                   // compute kernel

	cl_mem input;                       // device memory used for the first input array
	cl_mem input2;                       // device memory used for the second input array
	cl_mem output;                      // device memory used for the output array

										// Fill our data set with random float values
										//
	int i = 0;
	unsigned int count = dataSize;
	for (i = 0; i < count; i++)
		data[i] = rand() / (float)RAND_MAX;

	i = 0;
	count = dataSize;
	for (i = 0; i < count; i++)
		data2[i] = rand() / (float)RAND_MAX;

	// Platform
	cl_uint numPlatforms;
	error = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (error != CL_SUCCESS) {
		cout << "Error getting platform id: " << getDescriptionOfError(error) << endl;
		exit(error);
	}

	cl_platform_id* platforms_id = new cl_platform_id[numPlatforms];
	error = clGetPlatformIDs(numPlatforms, platforms_id, NULL);
	if (error != CL_SUCCESS) {
		cout << "Error getting platform id: " << getDescriptionOfError(error) << endl;
		exit(error);
	}
	platform_id = platforms_id[usePlatform];

	// Connect to a compute device
	//
	cl_uint devices_n = 0;
	error = clGetDeviceIDs(platform_id, useGPU ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, useDevice, &device_id, &devices_n);
	if (error != CL_SUCCESS)
	{
		cout << "Error: Failed to create a device group: " << getDescriptionOfError(error) << endl;
		return EXIT_FAILURE;
	}

	// Create a compute context
	//
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &error);
	if (error != CL_SUCCESS)
	{
		cout << "Error: Failed to create a compute context: " << getDescriptionOfError(error) << endl;
		return EXIT_FAILURE;
	}

	// Create a command commands
	//
	commands = clCreateCommandQueue(context, device_id, 0, &error);
	if (error != CL_SUCCESS)
	{
		cout << "Error: Failed to create a command commands: " << getDescriptionOfError(error) << endl;
		return EXIT_FAILURE;
	}

	// Create the compute program from the source buffer
	//
	program = clCreateProgramWithSource(context, 1, (const char **)&KernelSource, NULL, &error);
	if (error != CL_SUCCESS)
	{
		cout << "Error: Failed to create compute program: " << getDescriptionOfError(error) << endl;
		return EXIT_FAILURE;
	}

	// Build the program executable
	//
	error = clBuildProgram(program, 0, NULL, "-cl-opt-disable", NULL, NULL);
	if (error != CL_SUCCESS)
	{
		size_t len;
		char buffer[2048];

		cout << "Error: Failed to build program executable: " << getDescriptionOfError(error) << endl;
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		cout << buffer << endl;
		exit(1);
	}

	// Create the compute kernel in the program we wish to run
	//
	kernel = clCreateKernel(program, "addVector", &error);
	if (!kernel || error != CL_SUCCESS)
	{
		cout << "Error: Failed to create compute kernel: " << getDescriptionOfError(error) << endl;
		exit(1);
	}

	// Create the input and output arrays in device memory for our calculation
	//
	input = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_double) * count, NULL, NULL);
	input2 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_double) * count, NULL, NULL);
	output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_double) * count, NULL, NULL);
	if (!input || !input2 || !output)
	{
		cout << "Error: Failed to allocate device memory: " << getDescriptionOfError(error) << endl;
		exit(1);
	}

	// Write our data set into the first input array in device memory
	//
	error = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, sizeof(cl_double) * count, data, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		cout << "Error: Failed to write to source array: " << getDescriptionOfError(error) << endl;
		exit(1);
	}

	// Write our data set into the second input array in device memory
	//
	error = clEnqueueWriteBuffer(commands, input2, CL_TRUE, 0, sizeof(cl_double) * count, data2, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		cout << "Error: Failed to write to source array: " << getDescriptionOfError(error) << endl;
		exit(1);
	}

	// Set the arguments to our compute kernel
	//
	error = 0;
	error = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
	error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &input2);
	error |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &output);
	error |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &count);
	if (error != CL_SUCCESS)
	{
		cout << "Error: Failed to set kernel arguments: " << getDescriptionOfError(error) << error << endl;
		exit(1);
	}

	// Get the maximum work group size for executing the kernel on the device
	//
	error = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	if (error != CL_SUCCESS)
	{
		cout << "Error: Failed to retrieve kernel work group info!" << endl;
		exit(1);
	}

	// Execute the kernel over the entire range of our 1d input data set
	// using the maximum number of work group items for this device
	//
	global = count;
	error = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
	if (error)
	{
		cout << "Error: Failed to execute kernel: " << getDescriptionOfError(error) << endl;
		return EXIT_FAILURE;
	}

	// Wait for the command commands to get serviced before reading back results
	//
	clFinish(commands);

	// Read back the results from the device to verify the output
	//
	error = clEnqueueReadBuffer(commands, output, CL_TRUE, 0, sizeof(cl_double) * count, results, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		cout << "Error: Failed to read output array: " << getDescriptionOfError(error) << endl;
		exit(1);
	}

	// Validate our results
	//
	correct = 0;
	for (i = 0; i < count; i++)
	{
		if (fabsf(results[i] - (data[i] + data2[i])) == 0) //  < 0.000001)
			correct++;
	}

	// Print a brief summary detailing the results
	//
	cout << "Computed '" << correct << "/" << count << "' correct values!" << endl;

	// Shutdown and cleanup
	//
	clReleaseMemObject(input);
	clReleaseMemObject(input2);
	clReleaseMemObject(output);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);
	system("pause");
	return 0;
}