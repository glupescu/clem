#include "clem_helper.h"

/*******************************************************
*       STDOUT function
*******************************************************/
void clem_printf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
		
	/* platform specific printf code */ 
	vprintf(format, args);
	
	va_end(args);
}


/*******************************************************
*	Error codes
*******************************************************/
const char* CLEM_ERRSTRING(cl_int err) {
switch (err) {
	case CL_SUCCESS:                     	return  "Success!";
	case CL_DEVICE_NOT_FOUND:               return  "Device not found.";
	case CL_DEVICE_NOT_AVAILABLE:           return  "Device not available";
	case CL_COMPILER_NOT_AVAILABLE:         return  "Compiler not available";
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:  return  "Memory object alloc fail";
	case CL_OUT_OF_RESOURCES:               return  "Out of resources";
	case CL_OUT_OF_HOST_MEMORY:             return  "Out of host memory";
	case CL_PROFILING_INFO_NOT_AVAILABLE:   return  "Profiling information N/A";
	case CL_MEM_COPY_OVERLAP:               return  "Memory copy overlap";
	case CL_IMAGE_FORMAT_MISMATCH:          return  "Image format mismatch";
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:     return  "Image format no support";
	case CL_BUILD_PROGRAM_FAILURE:          return  "Program build failure";
	case CL_MAP_FAILURE:                    return  "Map failure";
	case CL_INVALID_VALUE:                  return  "Invalid value";
	case CL_INVALID_DEVICE_TYPE:            return  "Invalid device type";
	case CL_INVALID_PLATFORM:               return  "Invalid platform";
	case CL_INVALID_DEVICE:                 return  "Invalid device";
	case CL_INVALID_CONTEXT:                return  "Invalid context";
	case CL_INVALID_QUEUE_PROPERTIES:       return  "Invalid queue properties";
	case CL_INVALID_COMMAND_QUEUE:          return  "Invalid command queue";
	case CL_INVALID_HOST_PTR:               return  "Invalid host pointer";
	case CL_INVALID_MEM_OBJECT:             return  "Invalid memory object";
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:return  "Invalid image format desc";
	case CL_INVALID_IMAGE_SIZE:             return  "Invalid image size";
	case CL_INVALID_SAMPLER:                return  "Invalid sampler";
	case CL_INVALID_BINARY:                 return  "Invalid binary";
	case CL_INVALID_BUILD_OPTIONS:          return  "Invalid build options";
	case CL_INVALID_PROGRAM:                return  "Invalid program";
	case CL_INVALID_PROGRAM_EXECUTABLE:     return  "Invalid program exec";
	case CL_INVALID_KERNEL_NAME:            return  "Invalid kernel name";
	case CL_INVALID_KERNEL_DEFINITION:      return  "Invalid kernel definition";
	case CL_INVALID_KERNEL:                 return  "Invalid kernel";
	case CL_INVALID_ARG_INDEX:              return  "Invalid argument index";
	case CL_INVALID_ARG_VALUE:              return  "Invalid argument value";
	case CL_INVALID_ARG_SIZE:               return  "Invalid argument size";
	case CL_INVALID_KERNEL_ARGS:            return  "Invalid kernel arguments";
	case CL_INVALID_WORK_DIMENSION:         return  "Invalid work dimension";
	case CL_INVALID_WORK_GROUP_SIZE:        return  "Invalid work group size";
	case CL_INVALID_WORK_ITEM_SIZE:         return  "Invalid work item size";
	case CL_INVALID_GLOBAL_OFFSET:          return  "Invalid global offset";
	case CL_INVALID_EVENT_WAIT_LIST:        return  "Invalid event wait list";
	case CL_INVALID_EVENT:                  return  "Invalid event";
	case CL_INVALID_OPERATION:              return  "Invalid operation";
	case CL_INVALID_GL_OBJECT:              return  "Invalid OpenGL object";
	case CL_INVALID_BUFFER_SIZE:            return  "Invalid buffer size";
	case CL_INVALID_MIP_LEVEL:              return  "Invalid mip-map level";
	default:                                return  "Unknown";
    }
}

/*******************************************************
*	Error log
*******************************************************/
void CLEM_ERRLOG(cl_program program, cl_device_id device)
{
	char* build_log;
	size_t log_size;

	/* first call to know the proper size */
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
			0, NULL, &log_size);
	build_log = (char*)malloc((log_size + 1) * sizeof(char));

	/* Second call to get the log */
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
			log_size, build_log, NULL);
	build_log[log_size] = '\0';
	clem_printf("%s",build_log);
}

/*******************************************************
*       Check error
*******************************************************/
int CHECK(int cuerr){

	if(cuerr != CL_SUCCESS)
	{
		clem_printf("\n%s\n", CLEM_ERRSTRING(cuerr));
		return 1;
	}
	return 0;
}

/*******************************************************
*       Check error program compile
*******************************************************/
int CHECK_COMPILE(int cuerr, cl_program program, cl_device_id device){

	if(cuerr != CL_SUCCESS)
	{
		clem_printf("\n%s\n", CLEM_ERRSTRING(cuerr));
		CLEM_ERRLOG(program, device);
		return 1;
     	}
	return 0;
}

/*******************************************************
*       Init OpenCL Environment
*******************************************************/
int clem_init(cl_context *ptr_context, 
			cl_command_queue *ptr_queue,
			cl_program *ptr_program,
			const char *ptr_source,
			const char *ptr_binary,
			size_t str_size)
{
	cl_int ret;
	char buffer[128];
	cl_platform_id cpPlatform;
	cl_device_id device_id;
	
	size_t source_size;
	size_t binary_sizes[2] = {0, 0};
	
	CHECK(clGetPlatformIDs(1, &cpPlatform, NULL));
	CHECK(clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 
		1, &device_id, NULL));
		
	CHECK(clGetDeviceInfo(device_id, CL_DEVICE_VENDOR, sizeof(buffer), buffer, NULL));
	clem_printf("Vendor: %s\n", buffer);
	
	CHECK(clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(buffer), buffer, NULL));
	clem_printf("Device: %s\n", buffer);
		
	*ptr_context = clCreateContext(0, 1, &device_id, NULL, NULL, &ret);
	CHECK(ret);
	
	*ptr_queue = clCreateCommandQueue(*ptr_context, device_id, 0, &ret);
	CHECK(ret);
	
	if(ptr_source != NULL) {
		source_size = str_size;
		clem_printf("Kernel source size: %zub\n", source_size);
	
		*ptr_program = clCreateProgramWithSource(*ptr_context, 1, (const char **)&ptr_source, (const size_t *)&source_size, &ret);
		CHECK(ret);
		
		ret = clBuildProgram(*ptr_program, 1, &device_id, NULL, NULL, NULL);
		CHECK_COMPILE(ret, *ptr_program, device_id);
	}
	else if(ptr_binary != NULL){
		binary_sizes[0] = str_size;
		clem_printf("Kernel binary size: %zub\n", binary_sizes[0]);
		
		*ptr_program = clCreateProgramWithBinary(*ptr_context, 1, &device_id, 
			(const size_t *)binary_sizes, 
			(const unsigned char **)&ptr_binary, 
			NULL, &ret);
		CHECK(ret); 
						
		ret = clBuildProgram(*ptr_program, 1, &device_id, NULL, NULL, NULL);
		CHECK_COMPILE(ret, *ptr_program, device_id);
		
		return CL_SUCCESS;
	}
	else {
		return CL_SUCCESS;
		}
}

/*******************************************************
*       Destroy OpenCL Environment
*******************************************************/
int clem_finit(cl_context *ptr_context, 
			cl_command_queue *ptr_queue,
			cl_program *ptr_program)
{
	if(ptr_program != NULL)
		clReleaseProgram(*ptr_program);
	
	if(ptr_queue != NULL)
		clReleaseCommandQueue(*ptr_queue);
		
	if(ptr_context != NULL)
		clReleaseContext(*ptr_context);
	
	return CL_SUCCESS;
}




