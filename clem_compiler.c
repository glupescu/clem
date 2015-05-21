#include <math.h>
#include "clem_helper.h"

#define MEM_SIZE (128)	
#define MAX_SOURCE_SIZE (0x100000)	
#define ERR_KERNEL_READ -1313
#define ERR_KERNEL_STR_ALLOC -1314
#define KERNEL_BINARY_CFILE "ckernel.h"

/* ./fslcl_compiler input.cl output.h */
#define NUM_ARGS 2
 
/******************************************
 * MAIN
 *****************************************/
int main(int argc, char **argv)
{
	cl_int i;	
	int mem[MEM_SIZE];
	FILE *fp;
	size_t source_size;
	char *source_str;
	size_t binary_size;
	unsigned char *binary_str;
	cl_device_id device_id = NULL;
	cl_program program = NULL;
	cl_context context = NULL;
	cl_int ret;
	
	if(argc != (NUM_ARGS + 1))
		clem_printf("./fslcl_compiler input.cl output.h");
	
	/* Load kernel source code */
	fp = fopen(argv[1], "r");
	if (!fp) {
		clem_printf("ERROR fopen on file '%s'\n", argv[1]);
		return ERR_KERNEL_READ;
	}
	
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	
	/* Create Kernel program from the read in source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
	CHECK(ret);	

	/* Build Kernel Program */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	CHECK(ret);

	ret = clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &binary_size, NULL);
	CHECK(ret);

	clem_printf("Kernel binary size: %d\n", binary_size); 
	binary_str = (unsigned char*) malloc(sizeof(unsigned char)* binary_size);

	ret = clGetProgramInfo(program, CL_PROGRAM_BINARIES, sizeof(unsigned char*), &binary_str, NULL);
	CHECK(ret);

	fp = fopen(argv[2], "w");
	if(fp == 0){
		clem_printf("ERROR fopen on file '%s'\n", argv[2]);
		return ERR_KERNEL_READ;
	}

	fprintf(fp, "\n/* OpenCL precompiled kernel */\n");
	fprintf(fp, "const unsigned char binary_program[] = {");
	for(i=0; i < binary_size; i++)
		fprintf(fp, "0x%02x, ", binary_str[i]);
	fprintf(fp, "0x00};");
	fprintf(fp, "\n /* END OpenCL precompiled kernel */");
    	fclose(fp);
	
	clem_printf("Binary kernel written to %s\n\n", argv[2]);

	return 0;
}
