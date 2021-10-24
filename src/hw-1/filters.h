#ifndef __FILTERS__
#define __FILTERS__

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <stdint.h>

#include "clfilters.h"

extern double gaussian_blur_kernel_5x5[5][5];
extern double edges_kernel_5x5[5][5];
extern double sharpening_kernel_5x5[5][5];
extern double motionblur_kernel_9x9[9][9];

typedef struct {
  gboolean use_gpu_flag;
  gint apply_times;
  gint ker_width;
  double *kernel;
  double factor;
  double bias;
} FilterData;

typedef struct {
  FilterData *filter_data;
  gchar *folder;
} FilterToSaveData;

GdkPixbuf *apply_flip(GdkPixbuf *, gboolean);
GdkPixbuf *apply_filter(const GdkPixbuf *, double *kernel, gint, gint, double,
                        double);

GdkPixbuf *apply_filter_parallel(const GdkPixbuf *, double *kernel, gint, gint,
                                 double, double);

GdkPixbuf *apply_filter_GPGPU(GdkPixbuf *, double *kernel, gint, gint, double,
                              double);

GdkPixbuf *apply_filter_service(GdkPixbuf *, FilterData *);

void save_filtering_result(GdkPixbuf *, gchar *, gchar *, FilterData *);

#endif /* __FILTERS__ */