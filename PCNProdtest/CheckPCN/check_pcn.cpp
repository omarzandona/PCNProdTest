#include "check_pcn.h"

static int val_pattern_disp[NUM_LEVEL_DISP] = {DISP_1*16,DISP_2*16,DISP_3*16,DISP_4*16,DISP_5*16};


/****** Feature *******/



float compute_focus_measurement(unsigned char*  img,
                                const int& w,
                                const int& h)
{
    unsigned char* laplacian = compute_laplacian(img,w,h);
    int size = w*h;
    // Lap^2
    float sum = 0.0f;
    for (int i = 0; i < size; ++i)
    {
        //qDebug() << laplacian[i];
        unsigned char temp = (unsigned char) (laplacian[i]*laplacian[i]);
        sum += temp;
    }

    float fm = (float) sum / (size);
    free(laplacian);

    return fm;

}

unsigned char* compute_laplacian(unsigned char*  img,
                                 const int& w,
                                 const int& h)
{
    unsigned char* lap = (unsigned char*)calloc(w*h, sizeof(unsigned char));
    int radius = int(LAP_RADIUS/2);

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w ; ++j)
        {
            if (  i== 0 || i == h-radius || j == 0 || j == w-radius)
                lap[i*w + j] = (unsigned char) img[i*w + j];
            else{
                int k = 0;
                double tmp = 0 ;
                for (int r = i - radius; r <= i + radius; ++r)
                    for (int c = j - radius; c <= j + radius; ++c,++k)
                        tmp += double( (double(img[r*w + c])*(laplacian_kernel[k])));

                lap[i*w + j] = (unsigned char) abs(tmp);
            }
        }
    }

    return lap;
}


/*
check_focus
*/
bool check_focus(unsigned char*  & img_dx,
                 unsigned char*  & img_sx,
                 const int& w,
                 const int& h)
{


    float fm_dx = compute_focus_measurement(img_dx,w,h);
    float fm_sx = compute_focus_measurement(img_sx,w,h);
    bool res = ( fm_dx > FOCUS_MEASURAMENT && fm_sx > FOCUS_MEASURAMENT);
    return res;
}


void setParameters( tParametersCheck param ){
    FOCUS_MEASURAMENT = param.FOCUS_MEASURAMENT;
}
