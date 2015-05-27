#include "clem_helper.h"

/* kernel binary program */
#include "kernel_sha3.h"

#define VECTOR_SIZE (4096*4096)

/*******************************************************
*	MAIN
*******************************************************/
int main(int argc, char** argv)
{
	cl_context context = NULL;
	cl_command_queue queue = NULL;
	cl_kernel kernel = NULL;
	cl_program program = NULL;
	
	int i, j, ret;
	char *h_vecA;
	cl_mem d_vecA;

	/* Signal main execution started */
	clem_printf("Init OpenCL app\n");

	/* Init OpenCL environment */ 
	clem_init(&context, &queue, &program, NULL, binary_program, BINARY_SIZE);
	
	/* Buffer device and host */
	h_vecA = (char*)clem_malloc(VECTOR_SIZE * sizeof(char));
	for(i=0; i<VECTOR_SIZE; i++)
		h_vecA[i] = i / 32;
	
	d_vecA = clCreateBuffer(context, CL_MEM_READ_WRITE, VECTOR_SIZE * sizeof(char), NULL, &ret);
	CHECK(ret);
	
	/* Create kernel */
	kernel = clCreateKernel(program, "kernel_sha3", &ret);
	CHECK(ret);
	
	/* Set kernel arguments */
	CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&d_vecA));
	
	/* Start timer */
	clem_timer_start(0);
	
	/* Execute OpenCL kernel */
	size_t global_work[3] = {VECTOR_SIZE/8, 0, 0};
	CHECK(clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_work, NULL, 0, NULL, NULL));

	/* Transfer result from the memory buffer */
	CHECK(clEnqueueReadBuffer(queue, d_vecA, CL_TRUE, 0, VECTOR_SIZE * sizeof(char), h_vecA, 0, NULL, NULL));
	
	/* Stop timer */
	clem_timer_stop(0, "Execution time");
	
	for(i=0; i<32; i++){
		for(j=0; j<16; j++)
			clem_printf("%02x", h_vecA[i*32 + j]);
		clem_printf("\n");
	}
	
	/* Wait all operations */
	CHECK(clFlush(queue));
	
	/* Resource free */
	clem_finit(&context, &queue, &program);
}