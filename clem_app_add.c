#include "clem_helper.h"

char kernel_source[] = "__kernel void kernel_add(__global float* a){" \
	"int gid = get_global_id(0); float angle = gid;" \
	"a[gid] = sin(angle) * sin(angle) + cos(angle + 1.0) * cos(angle + 1.0);}";

#define VECTOR_SIZE 2048

/*******************************************************
*	MAIN
*******************************************************/
int main(int argc, char** argv)
{
	cl_context context = NULL;
	cl_command_queue queue = NULL;
	cl_kernel kernel = NULL;
	cl_program program = NULL;
	
	int i, ret;
	float *h_vecA;
	cl_mem d_vecA;
	
	float result = 0;

	/* Signal main execution started */
	clem_printf("Init OpenCL app\n");

	/* Init OpenCL environment */ 
	clemSafety_init(&context, &queue, &program, kernel_source, NULL, strlen(kernel_source));
	
	/* Buffer device and host */
	h_vecA = (float*)clem_malloc(VECTOR_SIZE * sizeof(float));
	d_vecA = clCreateBuffer(context, CL_MEM_READ_WRITE, VECTOR_SIZE * sizeof(float), NULL, &ret);
	CHECK(ret);
	
	/* Create kernel */
	kernel = clCreateKernel(program, "kernel_add", &ret);
	CHECK(ret);
	
	/* Set kernel arguments */
	CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&d_vecA));
	
	/* Start timer */
	clem_timer_start(0);
	
	/* Execute OpenCL kernel */
	size_t global_work[3] = {VECTOR_SIZE, 0, 0};
	CHECK(clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_work, NULL, 0, NULL, NULL));

	/* Transfer result from the memory buffer */
	CHECK(clEnqueueReadBuffer(queue, d_vecA, CL_TRUE, 0, VECTOR_SIZE * sizeof(float), h_vecA, 0, NULL, NULL));
	
	/* Stop timer */
	clem_timer_stop(0, "Execution time");
	
	for(i=0; i<VECTOR_SIZE; i++)
		result += (h_vecA[i] / (VECTOR_SIZE - 1));
	clem_printf("Result: %f\n", result);
	
	/* Wait all operations */
	CHECK(clFlush(queue));
	
	/* Resource free */
	clemSafety_finit(&context, &queue, &program);
}