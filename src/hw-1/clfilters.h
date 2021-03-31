#ifndef __CLFILTERS__
#define __CLFILTERS__

#define CL_TARGET_OPENCL_VERSION 120

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

void host_program(guint8 *, 
                guint8 *, 
                size_t ,
                int ,
                int ,
                float *, 
                int ,
                int ,
                int ,
                float ,
                float);

#endif /* __CLFILTERS__ */