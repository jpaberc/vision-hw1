#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                set_pixel(im,x,y,c,get_pixel(im,x,y,c)/(1.0*im.w*im.h));
            }
        }
    }
}

image make_box_filter(int w)
{
    image bf = make_image(w,w,1);
    for (int c = 0; c < bf.c; c++) {
        for (int y = 0; y < bf.h; y++) {
            for (int x = 0; x < bf.w; x++) {
                set_pixel(bf,x,y,c,1.0);
            }
        }
    }
    l1_normalize(bf);
    return bf;
}

image convolve_image(image im, image filter, int preserve)
{
    assert(filter.c == im.c || filter.c == 1);

    image cim = (preserve == 1) ? make_image(im.w,im.h,im.c) : make_image(im.w,im.h,filter.c);

    for (int x = 0; x < cim.w; x++) {
        for (int y = 0; y < cim.h; y++) {
            if (preserve == 1) {
                for (int c = 0; c < cim.c; c++) {
                    int fc = (filter.c > 1) ? c : 0;
                    float q = 0;
                    for (int fx = 0; fx < filter.w; fx++) {
                        for (int fy = 0; fy < filter.h; fy++) {

                            q += get_pixel(filter,fx,fy,fc) * get_pixel(im,x-(filter.w/2)+fx,y-(filter.h/2)+fy,c);
                        }
                    }
                    set_pixel(cim,x,y,c,q);
                }
            } else {
                float q = 0;
                for (int c = 0; c < im.c; c++) {
                    int fc = (filter.c > 1) ? c : 0;
                    for (int fx = 0; fx < filter.w; fx++) {
                        for (int fy = 0; fy < filter.h; fy++) {
                            q += get_pixel(filter,fx,fy,fc) * get_pixel(im,x-(filter.w/2)+fx,y-(filter.h/2)+fy,c);
                        }
                    }
                }
                set_pixel(cim,x,y,0,q);
            }
        }
    }

    return cim;
}

image make_highpass_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_sharpen_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_emboss_filter()
{
    // TODO
    return make_image(1,1,1);
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    return make_image(1,1,1);
}

image add_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image sub_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image make_gx_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    return make_image(1,1,1);
}

void feature_normalize(image im)
{
    // TODO
}

image *sobel_image(image im)
{
    // TODO
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
