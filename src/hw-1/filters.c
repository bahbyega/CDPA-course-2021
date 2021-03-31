#include "filters.h"

double gaussian_blur_kernel_5x5[5][5] = 
{
    { 1, 4,  6,  4,  1 },
    { 4, 16, 24, 16, 4 },
    { 6, 24, 36, 24, 6 },
    { 4, 16, 24, 16, 4 },
    { 1, 4,  6,  4,  1 }
}; // divide by 256

float edges_kernel_5x5[5][5] =
{
   { 0,  0, -1,  0,  0 },
   { 0,  0, -1,  0,  0 },
   { 0,  0,  4,  0,  0 },
   { 0,  0, -1,  0,  0 },
   { 0,  0, -1,  0,  0 }
};

double sharpening_kernel_5x5[5][5] =
{
  { -1, -1, -1, -1, -1 },
  { -1,  2,  2,  2, -1 },
  { -1,  2,  8,  2, -1 },
  { -1,  2,  2,  2, -1 },
  { -1, -1, -1, -1, -1 }
}; // divide by 8


double motionblur_kernel_9x9[9][9] =
{
  { 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1 },
}; // divide by 9

#define FLIP_DIRECTION_X_AXIS TRUE
#define FLIP_DIRECTION_Y_AXIS FALSE

/**
 * Flips image in two directions.
 * 
 * Parameters:
 *  pixbuf: a GdkPixbuf of image data.
 *  direction: a boolean, if TRUE then flip on x axis
 *                        if FALSE then flip on y axis.
 **/
GdkPixbuf *apply_flip(GdkPixbuf *pixbuf, gboolean direction)
{
    GdkColorspace colorspace = gdk_pixbuf_get_colorspace(pixbuf);
    gboolean has_alpha       = gdk_pixbuf_get_has_alpha(pixbuf);
    gint bits_per_sample     = gdk_pixbuf_get_bits_per_sample(pixbuf);
    gint width               = gdk_pixbuf_get_width(pixbuf);
    gint height              = gdk_pixbuf_get_height(pixbuf);
    gint rowstride           = gdk_pixbuf_get_rowstride(pixbuf);
    gint channels            = gdk_pixbuf_get_n_channels(pixbuf);

    GdkPixbuf *result = gdk_pixbuf_new(colorspace,
                                       has_alpha,
                                       bits_per_sample, 
			                           width, 
			                           height);

    const guchar *init_pixline;
    guchar       *flip_pixline;

    const guint8 *src_image = gdk_pixbuf_read_pixels(pixbuf);
    guint8       *res_image = gdk_pixbuf_get_pixels(result);

    if (direction == FLIP_DIRECTION_X_AXIS)
    {
      for (gint i = 0; i < height; i++)
      for (gint j = 0; j < width; j++)
	    {
	      init_pixline = src_image + j * channels + (gsize)i * rowstride;
	      flip_pixline = res_image + (width - j - 1) * channels + (gsize)i * rowstride;
	      memcpy(flip_pixline, init_pixline, channels);
	    }
    }
    else if (direction == FLIP_DIRECTION_Y_AXIS)
    {
      for (gint i = 0; i < height; i++)
	    {
	      init_pixline = src_image + (gsize)i * rowstride;
	      flip_pixline = res_image + (gsize)(height - i - 1) * rowstride;
	      memcpy(flip_pixline, init_pixline, rowstride);
	    }
    }

    return result;
}

/**
 * Calculates new RGB values for source image after filter
 * is applied. The filter is represented by its kernel.
 * I decided to pass a pointer to filter's first element
 * and its width and height. In this case, this function
 * is able to work with any kernel, you only need to 
 * specify a pointer to kernel's first element
 * and kernel's size and it'll be eaten by this function.
 * 
 * Parameters:
 *  pixbuf: a GdkPixbuf of image data.
 *  kernel: a double pointer to kernel's first element.
 *  ker_width: width of a kernel.
 *  ker_height: height of a kernel.
 *  factor: weight of a filter (so if factor is 0.3
 *          it multiplies filter elements by 0.3).
 *  bias: adds value to calculated pixel's color
 *        use it if you want to change image brightness.
 * 
 * Returns: a GdkPixbuf of filtered image data.
 **/
GdkPixbuf *apply_filter(const GdkPixbuf *pixbuf, double *kernel,
                        gint ker_width, gint ker_height,
                        double factor, double bias)
{
    GdkColorspace colorspace = gdk_pixbuf_get_colorspace(pixbuf);
    gboolean has_alpha       = gdk_pixbuf_get_has_alpha(pixbuf);
    gint bits_per_sample     = gdk_pixbuf_get_bits_per_sample(pixbuf);
    gint width               = gdk_pixbuf_get_width(pixbuf);
    gint height              = gdk_pixbuf_get_height(pixbuf);
    gint rowstride           = gdk_pixbuf_get_rowstride(pixbuf);
    gint channels            = gdk_pixbuf_get_n_channels(pixbuf);

    if (bits_per_sample != 8)
    {
        g_print("Couldn't load image for filtering\n");
        return NULL;
    }

    GdkPixbuf *result = gdk_pixbuf_new(colorspace,
                           has_alpha,
                           bits_per_sample, 
	                       width, 
	                       height);

    const guint8 *src_image = gdk_pixbuf_read_pixels(pixbuf);
    guint8       *res_image = gdk_pixbuf_get_pixels(result);
                
    for (gint x = 0; x < width; x++)
    for (gint y = 0; y < height; y++)
    {
        double red = 0.0, green = 0.0, blue = 0.0;
        const guchar *src_pixel;
        guchar       *out_pixel;

        for(gint filterY = 0; filterY < ker_height; filterY++)
        for(gint filterX = 0; filterX < ker_width; filterX++)
        {
            gint imageX = (x - ker_width / 2 + filterX + width) % width;
            gint imageY = (y - ker_height / 2 + filterY + height) % height;

            src_pixel = src_image + imageY * rowstride + imageX * channels;
            out_pixel = res_image + imageY * rowstride + imageX * channels;

            red   += src_pixel[0] * kernel[filterY * ker_width + filterX];
            green += src_pixel[1] * kernel[filterY * ker_width + filterX];
            blue  += src_pixel[2] * kernel[filterY * ker_width + filterX];

            if (has_alpha) out_pixel[3] = src_pixel[3];
        }
        
        out_pixel[0] = MIN(MAX((gint)(factor * red + bias),   0), (gint)255);
        out_pixel[1] = MIN(MAX((gint)(factor * green + bias), 0), (gint)255);
        out_pixel[2] = MIN(MAX((gint)(factor * blue + bias),  0), (gint)255);
    }
    
    return result;
}

/**
 * Parallel version of previous function. 
 * The only thing changed is that i added openmp parallelization for a loop. 
 * Tried applying it for multiple loops, but benchmarks showed that this 
 * is the fastest solution.
 * Increases speed of previous function in ~4 times. (I've got 4 cores).
 **/
GdkPixbuf *apply_filter_parallel(const GdkPixbuf *pixbuf, double *kernel,
                        gint ker_width, gint ker_height,
                        double factor, double bias)
{
    GdkColorspace colorspace = gdk_pixbuf_get_colorspace(pixbuf);
    gboolean has_alpha       = gdk_pixbuf_get_has_alpha(pixbuf);
    gint bits_per_sample     = gdk_pixbuf_get_bits_per_sample(pixbuf);
    gint width               = gdk_pixbuf_get_width(pixbuf);
    gint height              = gdk_pixbuf_get_height(pixbuf);
    gint rowstride           = gdk_pixbuf_get_rowstride(pixbuf);
    gint channels            = gdk_pixbuf_get_n_channels(pixbuf);

    if (bits_per_sample != 8)
    {
        g_print("Couldn't load image for filtering\n");
        return NULL;
    }

    GdkPixbuf *result = gdk_pixbuf_new(colorspace,
                           has_alpha,
                           bits_per_sample, 
	                       width, 
	                       height);

    const guint8 *src_image = gdk_pixbuf_read_pixels(pixbuf);
    guint8       *res_image = gdk_pixbuf_get_pixels(result);

    # pragma omp parallel for // collapse(2)
    for (gint x = 0; x < width; x++)
    for (gint y = 0; y < height; y++)
    {
        double red = 0.0, green = 0.0, blue = 0.0;
        const guchar *src_pixel;
        guchar       *out_pixel;

        for(gint filterY = 0; filterY < ker_height; filterY++)
        for(gint filterX = 0; filterX < ker_width; filterX++)
        {
            gint imageX = (x - ker_width / 2 + filterX + width) % width;
            gint imageY = (y - ker_height / 2 + filterY + height) % height;

            src_pixel = src_image + imageY * rowstride + imageX * channels;
            out_pixel = res_image + imageY * rowstride + imageX * channels;

            red   += src_pixel[0] * kernel[filterY * ker_width + filterX];
            green += src_pixel[1] * kernel[filterY * ker_width + filterX];
            blue  += src_pixel[2] * kernel[filterY * ker_width + filterX];

            if (has_alpha) out_pixel[3] = src_pixel[3];
        }
        
        out_pixel[0] = MIN(MAX((gint)(factor * red + bias),   0), (gint)255);
        out_pixel[1] = MIN(MAX((gint)(factor * green + bias), 0), (gint)255);
        out_pixel[2] = MIN(MAX((gint)(factor * blue + bias),  0), (gint)255);
    }
    
    return result;
}

/**
 * Initial version of GPGPU filtering
 **/ 
GdkPixbuf *apply_filter_GPGPU(GdkPixbuf *pixbuf, float *kernel,
                        gint ker_width, gint ker_height __attribute__((unused)),
                        double factor, double bias)
{
    GdkColorspace colorspace = gdk_pixbuf_get_colorspace(pixbuf);
    gboolean has_alpha       = gdk_pixbuf_get_has_alpha(pixbuf);
    gint bits_per_sample     = gdk_pixbuf_get_bits_per_sample(pixbuf);
    gint width               = gdk_pixbuf_get_width(pixbuf);
    gint height              = gdk_pixbuf_get_height(pixbuf);
    gint rowstride           = gdk_pixbuf_get_rowstride(pixbuf);
    gint channels            = gdk_pixbuf_get_n_channels(pixbuf);

    if (bits_per_sample != 8)
    {
        g_print("Couldn't load image for filtering\n");
        return NULL;
    }

    GdkPixbuf *result = gdk_pixbuf_new(colorspace,
                           has_alpha,
                           bits_per_sample, 
	                        width, 
	                       height);

    guint pixbuf_size;

    guint8 *src_pixbuf = gdk_pixbuf_get_pixels(pixbuf);
    guint8 *res_pixbuf = gdk_pixbuf_get_pixels_with_length(result, &pixbuf_size);

    host_program(src_pixbuf, res_pixbuf, (size_t)pixbuf_size, width, height, 
                kernel, ker_width, rowstride, channels, (float)factor, (float)bias);

    return result;
}