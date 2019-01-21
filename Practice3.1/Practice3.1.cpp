// Practice3.1.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <CL/cl.h>
#include <iostream>

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

int main(int argc, char** argv)
{
	int sizeBuffer = 10240;
	char* buffer = new char[sizeBuffer];
	cl_uint buf_uint;
	cl_ulong buf_ulong;
	cl_int error = 0;   // Utilizado para la gestion de errores
	cl_uint numPlatforms;
	cl_platform_id* platforms;
	cl_uint numDevices;
	cl_device_id* devices;


	// Platform
	error = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (error != CL_SUCCESS) {
		cout << "Error getting platform id: " << getDescriptionOfError(error) << endl;
		exit(error);
	}

	cout << numPlatforms << " platforms" << endl;

	platforms = new cl_platform_id[numPlatforms];
	error = clGetPlatformIDs(numPlatforms, platforms, NULL);
	if (error != CL_SUCCESS) {
		cout << "Error getting platform id: " << getDescriptionOfError(error) << endl;
		exit(error);
	}

	for (int p = 0; p < numPlatforms; ++p) {
		// Number of devices
		cl_platform_id platform = platforms[p];
		error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
		if (error != CL_SUCCESS) {
			cout << "Error getting device ids: " << getDescriptionOfError(error) << endl;
			continue;
		}

		cout << "  =========================" << endl;
		clGetPlatformInfo(platform, CL_PLATFORM_PROFILE, sizeBuffer, buffer, NULL);
		cout << "  PROFILE = " << buffer << endl;
		clGetPlatformInfo(platform, CL_PLATFORM_VERSION, sizeBuffer, buffer, NULL);
		cout << "  VERSION = " << buffer << endl;
		clGetPlatformInfo(platform, CL_PLATFORM_NAME, sizeBuffer, buffer, NULL);
		cout << "  NAME = " << buffer << endl;
		clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, sizeBuffer, buffer, NULL);
		cout << "  VENDOR = " << buffer << endl;
		clGetPlatformInfo(platform, CL_PLATFORM_EXTENSIONS, sizeBuffer, buffer, NULL);

		// Devices
		devices = new cl_device_id[numDevices];
		error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);
		if (error != CL_SUCCESS) {
			cout << "Error getting devices: " << getDescriptionOfError(error) << endl;
		}

		for (int d = 0; d < numDevices; ++d) {
			cl_device_id device = devices[d];
			cout << "  -------------------------" << endl;
			clGetDeviceInfo(device, CL_DEVICE_NAME, sizeBuffer, buffer, NULL);
			cout << "    DEVICE_NAME = " << buffer << endl;
			cl_device_type deviceType;
			clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(deviceType), &deviceType, NULL);
			if (deviceType == CL_DEVICE_TYPE_GPU)
				cout << "    CL_DEVICE_TYPE = " << "GPU" << endl;
			if (deviceType == CL_DEVICE_TYPE_CPU)
				cout << "    CL_DEVICE_TYPE = " << "CPU" << endl;
			clGetDeviceInfo(device, CL_DEVICE_VENDOR, sizeBuffer, buffer, NULL);
			cout << "    DEVICE_VENDOR = " << buffer << endl;
			clGetDeviceInfo(device, CL_DEVICE_VERSION, sizeBuffer, buffer, NULL);
			cout << "    DEVICE_VERSION = " << buffer << endl;
			clGetDeviceInfo(device, CL_DRIVER_VERSION, sizeBuffer, buffer, NULL);
			cout << "    DRIVER_VERSION = " << buffer << endl;
			clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buf_uint), &buf_uint, NULL);
			cout << "    DEVICE_MAX_COMPUTE_UNITS = " << (unsigned int)buf_uint << endl;
			clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buf_uint), &buf_uint, NULL);
			cout << "    DEVICE_MAX_CLOCK_FREQUENCY = " << (unsigned int)buf_uint << endl;
			clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, NULL);
			cout << "    DEVICE_GLOBAL_MEM_SIZE = " << (unsigned long long)buf_ulong << endl;
			clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(buf_uint), &buf_uint, NULL);
			cout << "    DEVICE_MAX_WORK_GROUP_SIZE = " << (unsigned int)buf_uint << endl;
			clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(buf_uint), &buf_uint, NULL);
			cout << "    CL_DEVICE_MAX_CONSTANT_ARGS = " << (unsigned int)buf_uint << endl;
			clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(buf_ulong), &buf_ulong, NULL);
			cout << "    CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE = " << (unsigned long long)buf_ulong << endl;

		}
	}
	system("pause");
}
