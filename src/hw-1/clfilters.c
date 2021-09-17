#include "clfilters.h"

void host_program(guint8 *src_pixbuf,
                guint8 *res_pixbuf,
                size_t pixbuf_size,
                int img_width,
                int img_height,
                double *filter_kernel,
                int filter_kernel_size,
                int rowstride,
                int channels,
                double factor,
                double bias)
{
    // read the kernel
    FILE *fp;
    long filelen;
    long readlen;
    char *kernel_src;

    fp = fopen("src/hw-1/clkernel.cl", "r");
    if (fp == NULL) g_print("we're here\n");
    fseek(fp, 0L, SEEK_END);
    filelen = ftell(fp);

    rewind(fp);

    kernel_src = calloc(filelen + 1, sizeof(char));
    readlen = fread(kernel_src, 1, filelen, fp);
    if (readlen != filelen)
    {
        g_print("Error reading kernel file\n");
        exit(1);
    }
    kernel_src[filelen + 1] = '\0';

    // setup OpenCL
    cl_int err;
    cl_platform_id platform_id;
    cl_uint num_platforms_returned;

    err = clGetPlatformIDs(1, &platform_id, &num_platforms_returned);
    if (err != CL_SUCCESS)
    {
        g_print("Couldn't get platform ID. Error code %d\n", err);
        exit(1);
    }

    cl_device_id device_id;
    cl_uint num_devs_returned;

    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1,
                        &device_id, &num_devs_returned);
    if (err != CL_SUCCESS)
    {
        g_print("Couldn't get device ID. Is GPU driver installed? "
                "Error code %d\n", err);
        exit(1);
    }

    cl_context context;
    cl_context_properties properties[] =
    {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)platform_id,
        0
    };

    context = clCreateContext(properties, 1, &device_id, NULL, NULL, &err);
    if (err != CL_SUCCESS)
    {
        g_print("Couldn't create context. Error code=%d\n", err);
        exit(1);
    }

    cl_command_queue command_queue;
    
    command_queue = clCreateCommandQueue(context, device_id, 0, &err);
    if (err != CL_SUCCESS)
    {
        g_print("Couldn't create command queue. Error code=%d\n", err);
        exit(1);
    }

    cl_program program;
    program = clCreateProgramWithSource(context, 1, (const char **)
                                        &kernel_src, NULL, &err);
    if (err != CL_SUCCESS)
    {
        g_print("Couldn't create program object. Error code=%d\n", err);
        exit(1);
    }

    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        g_print("CL Build failed, Error code=%d\n", err);

        // print build log
        size_t len;
        char buffer[4096];
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,
                              sizeof(buffer), buffer, &len);
        g_print(" --- Build log --- \n %s\n", buffer);
        exit(1);
    }

    cl_kernel kernel;
    kernel = clCreateKernel(program, "apply_filter", &err);
    
    if (err != CL_SUCCESS)
    {
        printf("Couldn't create kernel object. Error Code=%d\n",err);
        exit(1);
    }

    // data transfering
    cl_mem src_image_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                            pixbuf_size, src_pixbuf, &err);
    if (err != CL_SUCCESS)
    {
        g_print("Couldn't create src buffer. Error code=%d\n", err);
        exit(1);
    }

    cl_mem res_image_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                            pixbuf_size, NULL, &err);
    if (err != CL_SUCCESS)
    {
        g_print("Couldn't create res buffer. Error code=%d\n", err);
        exit(1);
    }                                        

    cl_mem filter_kernel_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                                sizeof(double) * filter_kernel_size * filter_kernel_size,
                                                filter_kernel, &err);
    if (err != CL_SUCCESS)
    {
        g_print("Couldn't create kernel buffer. Error code=%d\n", err);
        exit(1);
    }

    if (clSetKernelArg(kernel, 0, sizeof(cl_mem), &src_image_buffer) ||
        clSetKernelArg(kernel, 1, sizeof(cl_mem), &res_image_buffer) ||
        clSetKernelArg(kernel, 2, sizeof(cl_mem), &filter_kernel_buffer) ||
        clSetKernelArg(kernel, 3, sizeof(cl_int), &img_width) ||
        clSetKernelArg(kernel, 4, sizeof(cl_int), &img_height) ||
        clSetKernelArg(kernel, 5, sizeof(cl_int), &filter_kernel_size) ||
        clSetKernelArg(kernel, 6, sizeof(cl_int), &rowstride) ||
        clSetKernelArg(kernel, 7, sizeof(cl_int), &channels) ||
        clSetKernelArg(kernel, 8, sizeof(cl_double), &factor) ||
        clSetKernelArg(kernel, 9, sizeof(cl_double), &bias)
        != CL_SUCCESS)
    {
        g_print("Couldn't set kernel arguments. Error code=%d\n", err);
        exit(1);
    }

    // execution
    size_t global_work_size = img_height * img_width;
    

    err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
                                 &global_work_size, 0, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        g_print("Couldn't enqueue kernel command. Error code=%d\n", err);
        exit(1);
    }

    clFinish(command_queue);

    // retreiving data
    clEnqueueReadBuffer(command_queue, res_image_buffer, CL_TRUE, 0,
                        pixbuf_size, res_pixbuf, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error enqueuing read buffer command. Error Code=%d\n", err);
        exit(1);
    }

    //clFinish(command_queue);

    // cleaning up
    clReleaseMemObject(src_image_buffer);
    clReleaseMemObject(res_image_buffer);
    clReleaseMemObject(filter_kernel_buffer);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);
    free(kernel_src);
}