#ifndef __FILTERS__
#define __FILTERS__

#include <stdint.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

extern double gaussian_blur_kernel_3x3[3][3];
extern double gaussian_blur_kernel_5x5[5][5];
extern double edges_kernel_3x3[3][3];
extern double edges_kernel_5x5[5][5];
extern double sharpening_kernel_3x3[3][3];
extern double sharpening_kernel_5x5[5][5];

GdkPixbuf *apply_flip(GdkPixbuf *, gboolean);
GdkPixbuf *apply_filter(const GdkPixbuf *, 
                                double *kernel,
                                gint, gint,
                                double, double);

#endif /* __FILTERS__ */