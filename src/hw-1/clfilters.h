#ifndef __CLFILTERS__
#define __CLFILTERS__

#define CL_TARGET_OPENCL_VERSION 120

#include <gtk/gtk.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

void host_program(guint8 *, 
                guint8 *, 
                size_t ,
                int ,
                int ,
                double *, 
                int ,
                int ,
                int ,
                double ,
                double);

#endif /* __CLFILTERS__ */