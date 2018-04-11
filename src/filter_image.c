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

    int x,y,c,fx,fy;
    float q, filter_val, pixel_val;

    image cim = (preserve == 1) ? make_image(im.w,im.h,im.c) : make_image(im.w,im.h,1);

    if (im.c == filter.c) {
        if (preserve == 0) {
            for (y = 0; y < cim.h; y++) {
                for (x = 0; x < cim.w; x++) {
                    q = 0;
                    for (c = 0; c < im.c; c++) {
                        for (fy = 0; fy < filter.h; fy++) {
                            for (fx = 0; fx < filter.w; fx++) {
                                filter_val = get_pixel(filter,fx,fy,c);
                                pixel_val = get_pixel(im, x-(filter.w/2)+fx, y-(filter.h/2)+fy,c) ;
                                q += filter_val*pixel_val;
                            }
                        }
                    }
                    q = (q > 0.0) ? ((q < 1.0) ? q : 1.0) : 0.0;
                    set_pixel(cim,x,y,0,q);
                }
            }
        }
        else {
            for (y = 0; y < cim.h; y++) {
                for (x = 0; x < cim.w; x++) {
                    for (c = 0; c < im.c; c++) {
                        q = 0;
                        for (fy = 0; fy < filter.h; fy++) {
                            for (fx = 0; fx < filter.w; fx++) {
                                filter_val = get_pixel(filter,fx,fy,c);
                                pixel_val = get_pixel(im, x-(filter.w/2)+fx, y-(filter.h/2)+fy,c);
                                q += filter_val*pixel_val;
                            }
                        }
                        q = (q > 0.0) ? ((q < 1.0) ? q : 1.0) : 0.0;
                        set_pixel(cim,x,y,c,q);
                    }
                }
            }
        }
    } else {
        if (preserve == 0) {
            for (y = 0; y < cim.h; y++) {
                for (x = 0; x < cim.w; x++) {
                    q = 0;
                    for (c = 0; c < im.c; c++) {
                        for (fy = 0; fy < filter.h; fy++) {
                            for (fx = 0; fx < filter.w; fx++) {
                                filter_val = get_pixel(filter,fx,fy,0);
                                pixel_val = get_pixel(im, x-(filter.w/2)+fx, y-(filter.h/2)+fy,c) ;
                                q += filter_val*pixel_val;
                            }
                        }
                    }
                    q = q / im.c;
                    q = (q > 0.0) ? ((q < 1.0) ? q : 1.0) : 0.0;
                    set_pixel(cim,x,y,0,q);
                }
            }
        } else {
            for (y = 0; y < cim.h; y++) {
                for (x = 0; x < cim.w; x++) {
                    for (c = 0; c < im.c; c++) {
                        q = 0;
                        for (fy = 0; fy < filter.h; fy++) {
                            for (fx = 0; fx < filter.w; fx++) {
                                filter_val = get_pixel(filter,fx,fy,0);
                                pixel_val = get_pixel(im, x-(filter.w/2)+fx, y-(filter.h/2)+fy,c);
                                q += filter_val*pixel_val;
                            }
                        }
                        q = (q > 0.0) ? ((q < 1.0) ? q : 1.0) : 0.0;
                        set_pixel(cim,x,y,c,q);
                    }
                }
            }
        }
    }

    // for (int x = 0; x < cim.w; x++) {
    //     for (int y = 0; y < cim.h; y++) {
    //         if (preserve == 1) {
    //             for (int c = 0; c < cim.c; c++) {
    //                 int fc = (filter.c > 1) ? c : 0;
    //                 float q = 0;
    //                 for (int fx = 0; fx < filter.w; fx++) {
    //                     for (int fy = 0; fy < filter.h; fy++) {

    //                         q += get_pixel(filter,fx,fy,fc) * get_pixel(im,x-(filter.w/2)+fx,y-(filter.h/2)+fy,c);
    //                     }
    //                 }
    //                 set_pixel(cim,x,y,c,q);
    //             }
    //         } else {
    //             float q = 0;
    //             for (int c = 0; c < im.c; c++) {
    //                 int fc = (filter.c > 1) ? c : 0;
    //                 for (int fx = 0; fx < filter.w; fx++) {
    //                     for (int fy = 0; fy < filter.h; fy++) {
    //                         q += get_pixel(filter,fx,fy,fc) * get_pixel(im,x-(filter.w/2)+fx,y-(filter.h/2)+fy,c);
    //                     }
    //                 }
    //             }
    //             if (filter.c == im.c) {
    //                 set_pixel(cim,x,y,0,q);
    //             }
    //             else {
    //                 set_pixel(cim,x,y,0,q/im.c);
    //             }
    //         }
    //     }
    // }

    return cim;
}

image make_highpass_filter()
{
    image f = make_box_filter(3);
    set_pixel(f,0,0,0,0);
    set_pixel(f,1,0,0,-1.);
    set_pixel(f,2,0,0,0);
    set_pixel(f,0,1,0,-1.);
    set_pixel(f,1,1,0,4.);
    set_pixel(f,2,1,0,-1.);
    set_pixel(f,0,2,0,0);
    set_pixel(f,1,2,0,-1.);
    set_pixel(f,2,2,0,0);
    return f;
}

image make_sharpen_filter()
{
    image f = make_box_filter(3);
    set_pixel(f,0,0,0,0);
    set_pixel(f,1,0,0,-1);
    set_pixel(f,2,0,0,0);
    set_pixel(f,0,1,0,-1);
    set_pixel(f,1,1,0,5);
    set_pixel(f,2,1,0,-1);
    set_pixel(f,0,2,0,0);
    set_pixel(f,1,2,0,-1);
    set_pixel(f,2,2,0,0);
    return f;
}

image make_emboss_filter()
{
    image f = make_box_filter(3);
    set_pixel(f,0,0,0,-2);
    set_pixel(f,1,0,0,-1);
    set_pixel(f,2,0,0,0);
    set_pixel(f,0,1,0,-1);
    set_pixel(f,1,1,0,1);
    set_pixel(f,2,1,0,1);
    set_pixel(f,0,2,0,0);
    set_pixel(f,1,2,0,1);
    set_pixel(f,2,2,0,2);
    return f;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: We should use preserve on sharpen and emboss because their purpose is to add an effect to the image
// whereas the highpass filter is designed to look for features in the image where we don't care about color.
// i.e. an edge between a red and green thing should be treated the same as an edge between a blue and a red thing.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: We need to do post-processing for any filter which does not sum to 0 (e.g. sharpen,emboss),
// because otherwise we may end up with pixel values >1 if sum is + or <0 if sum is -.

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
