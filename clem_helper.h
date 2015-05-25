#include <CL/cl.h>

#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/************************************
* List of helper functions
************************************/

/* error/debug functions */
const char* CL_ERRSTRING(cl_int err);
void CL_ERRLOG(cl_program program, cl_device_id device);
int CHECK(int cuerr);
int CHECK_COMPILE(int cuerr, cl_program program, cl_device_id device);

/* wrapper/helper functions */
void clem_printf(const char *format, ...);
int clem_init(cl_context *ptr_context, 
			cl_command_queue *ptr_queue,
			cl_program *ptr_program,
			const char *ptr_source,
			const char *ptr_binary);
int clem_finit(cl_context *ptr_context, 
			cl_command_queue *ptr_queue,
			cl_program *ptr_program);
