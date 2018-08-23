#include "shared.h"
#include <dlfcn.h>  /* dl stuff       */
#include <stdint.h> /* intptr_t       */

int load_library(const char * libname, void ** libhandle)
{
  *libhandle = dlopen(libname, RTLD_LAZY);

  if (libhandle == NULL)
  {
    return FAILURE;
  }
  return SUCCESS;
}

int get_functions(void * libhandle, LibraryFunctions * functions,
                  const char ** fn_names)
{
  functions->create = (MM_CREATE)(intptr_t)dlsym(libhandle, fn_names[0]);
   if (libhandle == NULL)
   {
     return FAILURE;
   }
  functions->destroy = (MM_DESTROY)(intptr_t)dlsym(libhandle, fn_names[1]);
   if (libhandle == NULL)
   {
     return FAILURE;
   }
  functions->allocate = (MM_ALLOCATE)(intptr_t)dlsym(libhandle, fn_names[2]);
   if (!functions->allocate)
   {
     return FAILURE;
   }
  functions->deallocate=(MM_DEALLOCATE)(intptr_t)dlsym(libhandle,fn_names[3]);
   if (!functions->deallocate)
   {
     return FAILURE;
   }
  return SUCCESS;
}
