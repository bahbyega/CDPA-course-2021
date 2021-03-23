#include "filters.h"

GdkPixbuf *apply_flip_on_x_axis(GdkPixbuf *pixbuf)
{
    g_print("WE'RE HERE 2\n");
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

#define filterWidth 5
#define filterHeight 5

double gaussian_blur_kernel[filterWidth][filterHeight] = 
{
    { 1, 4,  6,  4,  1 },
    { 4, 16, 24, 16, 4 },
    { 6, 24, 36, 24, 6 },
    { 4, 16, 24, 16, 4 },
    { 1, 4,  6,  4,  1 }
};

double factor = 1/256.00;
double bias = 0.0;


GdkPixbuf *apply_filter(const GdkPixbuf *pixbuf)
{
    GdkColorspace colorspace = gdk_pixbuf_get_colorspace(pixbuf);
    gboolean has_alpha       = gdk_pixbuf_get_has_alpha(pixbuf);
    gint bits_per_sample     = gdk_pixbuf_get_bits_per_sample(pixbuf);
    gint width               = gdk_pixbuf_get_width(pixbuf);
    gint height              = gdk_pixbuf_get_height(pixbuf);
    gint rowstride           = gdk_pixbuf_get_rowstride(pixbuf);
    gint channels            = gdk_pixbuf_get_n_channels(pixbuf);

    if (bits_per_sample != 8) return NULL;

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

        for(gint filterY = 0; filterY < filterHeight; filterY++)
        for(gint filterX = 0; filterX < filterWidth; filterX++)
        {
            gint imageX = (x - filterWidth / 2 + filterX + width) % width;
            gint imageY = (y - filterHeight / 2 + filterY + height) % height;

            src_pixel = src_image + imageY * rowstride + imageX * channels;
            out_pixel = res_image + imageY * rowstride + imageX * channels;

            red   += src_pixel[0] * gaussian_blur_kernel[filterY][filterX];
            green += src_pixel[1] * gaussian_blur_kernel[filterY][filterX];
            blue  += src_pixel[2] * gaussian_blur_kernel[filterY][filterX];

            if (has_alpha) out_pixel[3] = src_pixel[3];
        }
        
        out_pixel[0] = MIN(MAX((gint)(factor * red + bias),   0), (gint)255);
        out_pixel[1] = MIN(MAX((gint)(factor * green + bias), 0), (gint)255);
        out_pixel[2] = MIN(MAX((gint)(factor * blue + bias),  0), (gint)255);
    }
    
    return result;
}
