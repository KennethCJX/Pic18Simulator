#include "Exception.h"
#include <stdio.h>
#include <malloc.h>		// memory allocation
#include <string.h>
#include <stdarg.h>


Exception *cerateException(char *msg, int errorCode)
{
	Exception *ex;
	ex = malloc(sizeof(Exception)); //dynamic allocation
	ex->msg = msg;
	ex->errorCode = errorCode;
	ex->data = NULL;
	return ex;
}

void freeException(Exception *e)
{
	if(e->data && e->freeDataOnFreeingException)
	{
	free(e->data);
	}
	if(e->msg)
	{
	free(e->msg);
	}
	free(e);
}

void dumpException(Exception *e)
{
	printf("%s(error=00x%x)\n", e->msg, e->errorCode);
}

void throwException(int errorCode, void *data, int freeDataOnFreeingException,char *message,...)
{
	Exception *ex;
	size_t len;
	char *buffer; 
	
	va_list va;
	va_start(va,message);
	len = vsnprintf(NULL, 0, message, va);
	buffer = malloc(len+1);
	vsprintf (buffer,message,va);
	
	va_end(va);
	
	ex = cerateException(buffer, errorCode);
	ex->data = data;
	ex->freeDataOnFreeingException = freeDataOnFreeingException;
	Throw(ex);
}

/*void print(int count,...)
{
	int i ;
	va_list va;
    va_start(va,count);
for(i = 0; i < count; i++){
	printf("%d",va_arg(va, double));
}
	   
   va_end(va);
printf("\n");   // clean up when done 
}*/