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
void clem_printf(const char *format, ...);
const char* CL_ERRSTRING(cl_int err);
void CL_ERRLOG(cl_program program, cl_device_id device);
int CHECK(int cuerr);
int CHECK_COMPILE(int cuerr, cl_program program, cl_device_id device);
