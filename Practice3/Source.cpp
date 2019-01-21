
#include <iostream>
#include <math.h>
#include <CL/cl.h>
#include "Image.h"
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

const char* KernelSource = "";

bool useGPU = true;

cl_uint usePlatform = 1;
cl_uint useDevice = 1;

char* LoadFile(const char* filename) {
	FILE *fp;
	char *source_str;
	size_t source_size, program_size;

	fp = fopen(filename, "rb");
	if (!fp) {
		printf("Failed to load kernel\n");
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	program_size = ftell(fp);
	rewind(fp);
	source_str = (char*)malloc(program_size + 1);
	source_str[program_size] = '\0';
	fread(source_str, sizeof(char), program_size, fp);
	fclose(fp);
	return source_str;
}
int main(void)
{
	image::CImage img;
	img.LoadBMP("tornillo.bmp");
	if (!img.IsCreated()) {
		cout << "No se ha podido cargar la imagen" << endl;
		exit(1);
	}
	cl_int error;
	cl_uint imageSizeWidth = img.GetWidth();
	cl_uint imageSizeHeight = img.GetHeight();
	int dataSize = img.GetWidth()*img.GetHeight();
	cl_double* data = new cl_double[dataSize];
	cl_double* results = new cl_double[dataSize];

	size_t global[2];
	size_t local[2];

	cl_platform_id platform_id;
	cl_device_id device_id;             // compute device id
	cl_context context;                 // compute context
	cl_command_queue commands;          // compute command queue
	cl_program program;                 // compute program
	cl_kernel kernel;                   // compute kernel

	cl_mem input;                       // device memory used for the input array
	cl_mem output;                      // device memory used for the output array

										// Fill our data set with random float values
										//
	int i = 0;
	for (int x = 0; x < img.GetWidth(); x++)
		for (int y = 0; y < img.GetHeight(); y++)
			data[i++] = img.GetPixelBW(x, y);

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
	KernelSource = LoadFile("kernel.cl");
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
		char buffer[50048];

		cout << "Error: Failed to build program executable: " << getDescriptionOfError(error) << endl;
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		cout << buffer << endl;
		exit(1);
	}

	// Create the compute kernel in the program we wish to run
	//
	kernel = clCreateKernel(program, "edge_detector", &error);
	if (!kernel || error != CL_SUCCESS)
	{
		cout << "Error: Failed to create compute kernel: " << getDescriptionOfError(error) << endl;
		exit(1);
	}

	// Create the input and output arrays in device memory for our calculation
	//
	input = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_double) * dataSize, NULL, NULL);
	output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_double) * dataSize, NULL, NULL);
	if (!input || !output)
	{
		cout << "Error: Failed to allocate device memory: " << getDescriptionOfError(error) << endl;
		exit(1);
	}

	// Write our data set into the input array in device memory
	//
	cout << "Copiamos imagen" << endl;
	error = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, sizeof(cl_double) * dataSize, data, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		cout << "Error: Failed to write to source array: " << getDescriptionOfError(error) << endl;
		exit(1);
	}

	// Set the arguments to our compute kernel
	//
	error = 0;
	error = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
	error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
	error |= clSetKernelArg(kernel, 2, sizeof(cl_uint), &imageSizeWidth);
	error |= clSetKernelArg(kernel, 3, sizeof(cl_uint), &imageSizeHeight);
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
	global[0] = imageSizeWidth;
	global[1] = imageSizeHeight;
	cout << "Ejecutamos" << endl;

	error = clEnqueueNDRangeKernel(commands, kernel, 2, NULL, global, NULL, 0, NULL, NULL);
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
	cout << "Recuperamos resultados" << endl;
	error = clEnqueueReadBuffer(commands, output, CL_TRUE, 0, sizeof(cl_double) * dataSize, results, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		cout << "Error: Failed to read output array: " << getDescriptionOfError(error) << endl;
		exit(1);
	}

	// Validate our results
	//
	cout << "Guardamos imagen" << endl;
	image::CImage img2;
	img2.Create(img);

	i = 0;
	for (int x = 0; x < img.GetWidth(); x++) {
		for (int y = 0; y < img.GetHeight(); y++) {
			image::BWValue bw(results[i++]);
			// bw = img.GetPixelBW(x, y);
			img2.SetPixelBW(x, y, bw);
		}
	}
	img2.SaveBMP("results.bmp");


	cout << "Eliminamos memoria" << endl;
	// Shutdown and cleanup
	//
	clReleaseMemObject(input);
	clReleaseMemObject(output);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);
	system("pause");
	return 0;
}
