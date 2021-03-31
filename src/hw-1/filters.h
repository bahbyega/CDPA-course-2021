#ifndef __FILTERS__
#define __FILTERS__

#include <stdint.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "clfilters.h"

extern double gaussian_blur_kernel_5x5[5][5];
extern float edges_kernel_5x5[5][5];
extern double sharpening_kernel_5x5[5][5];
extern double motionblur_kernel_9x9[9][9];

GdkPixbuf *apply_flip(GdkPixbuf *, gboolean);
GdkPixbuf *apply_filter(const GdkPixbuf *, 
                                double *kernel,
                                gint, gint,
                                double, double);

GdkPixbuf *apply_filter_parallel(const GdkPixbuf *, 
                                double *kernel,
                                gint, gint,
                                double, double);

GdkPixbuf *apply_filter_GPGPU(GdkPixbuf *, 
                                float *kernel,
                                gint, gint,
                                double, double);

#endif /* __FILTERS__ */