#include <math.h>
#include "clem_helper.h"

#define MEM_SIZE (128)	
#define MAX_SOURCE_SIZE (0x100000)	
#define ERR_KERNEL_READ -1313
#define ERR_KERNEL_STR_ALLOC -1314
#define KERNEL_BINARY_CFILE "ckernel.h"
#define MAX_NUM_DEVICES 6

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
	size_t binary_sizes[MAX_NUM_DEVICES] = {0, 0, 0, 0, 0, 0};
	unsigned char *binary_str[MAX_NUM_DEVICES];
	cl_device_id device_id = NULL;
	cl_program program = NULL;
	cl_context context = NULL;
	cl_command_queue queue = NULL;
	cl_int ret;
	
	if(argc != (NUM_ARGS + 1))
		clem_printf("./fslcl_compiler input.cl output.h\n");
	
	/* Load kernel source code */
	fp = fopen(argv[1], "r");
	if (!fp) {
		clem_printf("ERROR fopen on file '%s'\n", argv[1]);
		return ERR_KERNEL_READ;
	}
	
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	
	/* Show OpenCL source code */
	clem_printf("Will compile OpenCL source code...\n");
	clem_printf("-----------------------------------------\n");
	clem_printf("%s", source_str);
	clem_printf("-----------------------------------------\n");
	
	/* Init OpenCL environment */ 
	clem_init(&context, &queue, &program, (const char*)source_str, NULL, strlen(source_str));

	ret = clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), binary_sizes, NULL);
	CHECK(ret);

	for(i=0; i<MAX_NUM_DEVICES; i++)
		if(binary_sizes[i] > 0){
			clem_printf("Kernel binary size: %d\n", binary_sizes[i]); 
			binary_str[i] = (unsigned char*) malloc(sizeof(unsigned char)* binary_sizes[i]);
		}

	ret = clGetProgramInfo(program, CL_PROGRAM_BINARIES, sizeof(unsigned char*), binary_str, NULL);
	CHECK(ret);

	fp = fopen(argv[2], "w");
	if(fp == 0){
		clem_printf("ERROR fopen on file '%s'\n", argv[2]);
		return ERR_KERNEL_READ;
	}
	fprintf(fp, "\n/* OpenCL precompiled kernel */\n");
	fprintf(fp, "#define BINARY_SIZE %zu\n", 
		binary_sizes[0]);
	fprintf(fp, "const unsigned char binary_program[] = {");
	for(i=0; i < binary_sizes[0]; i++)
		fprintf(fp, "0x%02x, ", binary_str[0][i]);
	fprintf(fp, "0x00};");
	fprintf(fp, "\n /* END OpenCL precompiled kernel */");
    fclose(fp);
	
	clem_printf("Binary kernel written to %s\n\n", argv[2]);
	
	/* Destroy OpenCL environment */
	clem_finit(&context, &queue, &program);

	return 0;
}
