#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void apply_filter(__global uchar *src_buffer,
                           __global uchar *res_buffer,
                           __constant double *filter_kernel,
                            int img_width,
							int img_height,
                            int filter_size,
                            int rowstride,
                            int channels,
                            double factor,
                            double bias)
{
	int N = get_global_id(0);
    int x = N % img_width;  
    int y = N / img_height;

    int l = get_local_id(0);
    if (l == 0) {
        double red = 0.0, green = 0.0, blue = 0.0;
        __global uchar *src_pixel;
        __global uchar *out_pixel;

        for(int filterX = 0; filterX < filter_size; filterX++)
        for(int filterY = 0; filterY < filter_size; filterY++)
        {
            int imageX = (x - filter_size / 2 + filterX + img_width) % img_width;
            int imageY = (y - filter_size / 2 + filterY + img_height) % img_height;

            src_pixel = src_buffer + imageY * rowstride + imageX * channels;
            out_pixel = res_buffer + imageY * rowstride + imageX * channels;

            red   += src_pixel[0] * filter_kernel[filterY * filter_size + filterX];
            green += src_pixel[1] * filter_kernel[filterY * filter_size + filterX];
            blue  += src_pixel[2] * filter_kernel[filterY * filter_size + filterX];
        }
        
        out_pixel[0] = min(max((int)(factor * red + bias),   0), 255);
        out_pixel[1] = min(max((int)(factor * green + bias), 0), 255);
        out_pixel[2] = min(max((int)(factor * blue + bias),  0), 255);

        // // debug code
        // out_pixel[0] = 100;
        // out_pixel[1] = 100;
        // out_pixel[2] = 100;
    }
}