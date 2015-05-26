#include "clem_helper.h"

/* kernel binary program */
#include "kernel_add.h"

/*******************************************************
*	MAIN
*******************************************************/
int main(int argc, char** argv)
{
	cl_context context = NULL;
	cl_command_queue queue = NULL;
	cl_kernel kernel = NULL;
	cl_program program = NULL;

	/* Signal main execution started */
	clem_printf("Init OpenCL app\n");

	/* Init OpenCL environment */ 
	clem_init(&context, &queue, &program, NULL, binary_program, BINARY_SIZE);
}