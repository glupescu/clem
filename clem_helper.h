#include <CL/cl.h>

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/************************************
* OS Type
************************************/
#define CLEM_OS_LINUX	1
//#define CLEM_OS_AUTOSAR 2

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
int clemSafety_init(cl_context *ptr_context, 
			cl_command_queue *ptr_queue,
			cl_program *ptr_program,
			const char *ptr_source,
			const char *ptr_binary,
			size_t str_size);
int clemSafety_finit(cl_context *ptr_context, 
			cl_command_queue *ptr_queue,
			cl_program *ptr_program);
			
void clem_timer_start(int id);
void clem_timer_stop(int id, const char* msg);

void* clem_malloc(size_t size);
void clem_free(void* ptr);
