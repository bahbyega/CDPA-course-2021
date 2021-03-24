#include "filters.h"

double gaussian_blur_kernel_3x3[3][3] =
{
  { 1, 2, 1 },
  { 2, 4, 2 },
  { 1, 2, 1 }
}; // divide by 16

double gaussian_blur_kernel_5x5[5][5] = 
{
    { 1, 4,  6,  4,  1 },
    { 4, 16, 24, 16, 4 },
    { 6, 24, 36, 24, 6 },
    { 4, 16, 24, 16, 4 },
    { 1, 4,  6,  4,  1 }
}; // divide by 256

double edges_kernel_3x3[3][3] =
{
  { 0, -1,  0 },
  {-1,  4, -1 },
  { 0, -1,  0 }
};

double edges_kernel_5x5[5][5] =
{
   { 0,  0, -1,  0,  0 },
   { 0,  0, -1,  0,  0 },
   { 0,  0,  4,  0,  0 },
   { 0,  0, -1,  0,  0 },
   { 0,  0, -1,  0,  0 }
};

double sharpening_kernel_3x3[3][3] =
{
  { -1, -1, -1 },
  { -1,  9, -1 },
  { -1, -1, -1 }
};

double sharpening_kernel_5x5[5][5] =
{
  { -1, -1, -1, -1, -1 },
  { -1,  2,  2,  2, -1 },
  { -1,  2,  8,  2, -1 },
  { -1,  2,  2,  2, -1 },
  { -1, -1, -1, -1, -1 }
};

GdkPixbuf *apply_flip_on_x_axis(GdkPixbuf *pixbuf)
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

    for (gint i = 0; i < height; i++)
    for (gint j = 0; j < width; j++)
	{
	    init_pixline = src_image + j * channels + (gsize)i * rowstride;
	    flip_pixline = res_image + (width - j - 1) * channels + (gsize)i * rowstride;
	    
        memcpy(flip_pixline, init_pixline, channels);
	}
    g_print("SUCCESS\n");
    return result;
}

GdkPixbuf *apply_flip_on_y_axis(GdkPixbuf *pixbuf)
{
    GdkColorspace colorspace = gdk_pixbuf_get_colorspace(pixbuf);
    gboolean has_alpha       = gdk_pixbuf_get_has_alpha(pixbuf);
    gint bits_per_sample     = gdk_pixbuf_get_bits_per_sample(pixbuf);
    gint width               = gdk_pixbuf_get_width(pixbuf);
    gint height              = gdk_pixbuf_get_height(pixbuf);
    gint rowstride           = gdk_pixbuf_get_rowstride(pixbuf);

    GdkPixbuf *result = gdk_pixbuf_new(colorspace,
                                       has_alpha,
                                       bits_per_sample, 
			                           width, 
			                           height);

    const guchar *init_pixline;
    guchar       *flip_pixline;

    const guint8 *src_image = gdk_pixbuf_read_pixels(pixbuf);
    guint8       *res_image = gdk_pixbuf_get_pixels(result);

    for (gint i = 0; i < height; i++)
	{
	    init_pixline = src_image + (gsize)i * rowstride;
	    flip_pixline = res_image + (gsize)(height - i - 1) * rowstride;
	    
        memcpy(flip_pixline, init_pixline, rowstride);
	}

    return result;
}

GdkPixbuf *apply_filter_3_size(const GdkPixbuf *pixbuf, double kernel[3][3],
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

        for(gint filterY = 0; filterY < 3; filterY++)
        for(gint filterX = 0; filterX < 3; filterX++)
        {
            gint imageX = (x - 3 / 2 + filterX + width) % width;
            gint imageY = (y - 3 / 2 + filterY + height) % height;

            src_pixel = src_image + imageY * rowstride + imageX * channels;
            out_pixel = res_image + imageY * rowstride + imageX * channels;

            red   += src_pixel[0] * kernel[filterY][filterX];
            green += src_pixel[1] * kernel[filterY][filterX];
            blue  += src_pixel[2] * kernel[filterY][filterX];

            if (has_alpha) out_pixel[3] = src_pixel[3];
        }
        
        out_pixel[0] = MIN(MAX((gint)(factor * red + bias),   0), (gint)255);
        out_pixel[1] = MIN(MAX((gint)(factor * green + bias), 0), (gint)255);
        out_pixel[2] = MIN(MAX((gint)(factor * blue + bias),  0), (gint)255);
    }
    
    return result;
}

GdkPixbuf *apply_filter_5_size(const GdkPixbuf *pixbuf, double kernel[5][5],
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

        for(gint filterY = 0; filterY < 5; filterY++)
        for(gint filterX = 0; filterX < 5; filterX++)
        {
            gint imageX = (x - 5 / 2 + filterX + width) % width;
            gint imageY = (y - 5 / 2 + filterY + height) % height;

            src_pixel = src_image + imageY * rowstride + imageX * channels;
            out_pixel = res_image + imageY * rowstride + imageX * channels;

            red   += src_pixel[0] * kernel[filterY][filterX];
            green += src_pixel[1] * kernel[filterY][filterX];
            blue  += src_pixel[2] * kernel[filterY][filterX];

            if (has_alpha) out_pixel[3] = src_pixel[3];
        }
        
        out_pixel[0] = MIN(MAX((gint)(factor * red + bias),   0), (gint)255);
        out_pixel[1] = MIN(MAX((gint)(factor * green + bias), 0), (gint)255);
        out_pixel[2] = MIN(MAX((gint)(factor * blue + bias),  0), (gint)255);
    }
    
    return result;
}
