#ifndef _CHECK_PCN_H_
#define _CHECK_PCN_H_

#define DEBUG

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <qstring>
#include <QImage>
#include <QDebug>
#include <QPixmap>
#include <QCoreApplication>
#include "../utils.h"
#include "../Structures.h"
#include "GraphDisparity.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define test_alloc(X) do {if ((void *)(X) == NULL) fprintf(stderr, "Out of memory in %s, (%s, line %d).\n", __FUNCTION__, __FILE__, __LINE__); } while (0)

#ifndef NULL
#define NULL (0)
#endif

// Limiti immagini
#define BORDER_X 12
#define BORDER_Y 8
#define BORDER_X_2 18

#define NY 120
#define NX 160
#define NN NX*NY

#define H_CROP 104// NY-2*BORDER_Y
#define	W_CROP 130//NX-(BORDER_X+BORDER_X_2)
#define CROP H_CROP*W_CROP


#define N_BINS 256

// Pattern
#define NUM_LEVEL_DISP 5
#define DISP_1 7
#define DISP_2 8
#define DISP_3 9
#define DISP_4 10
#define DISP_5 11

// DISPARITY MAP : UNIFORM VALUE
#define UNIFORM_VALUE 16
// SOBEL KERNEL
#define FACT_SOBEL_KERNEL 4.0f
#define RADIUS 3
const char KERNEL_H_SOBEL[9] = {1,0,-1,2,0,-2,1,0,-1};
const char KERNEL_V_SOBEL[9] = {1,2,1,0,0,0,-1,-2,-1};
#define PI 3.14159265

// LAPLACIAN KERNEL
static double laplacian_kernel [9]= { 0.1667,0.6667f,0.1667f,0.6667f,-3.3333f,0.6667f,0.1667,0.6667f,0.1667f};
#define LAP_RADIUS 3

/****** Calc Features ******/

//!< Print values features
void printFeatures(float* features);

//!< Crop Image
unsigned char* CropImage(unsigned char* img, const int& new_w, const int& new_h);


float compute_focus_measurement(unsigned char*  img,
                                const int& w,
                                const int& h);

//!< compute_laplacian
unsigned char* compute_laplacian(unsigned char*  img,
                                 const int& w,
                                 const int& h);




//!< check_focus
bool check_focus(unsigned char*  & img_dx,
                 unsigned char*  & img_sx,
                 const int& w,
                 const int& h);


//!< Setta i parametri per il check
void setParameters( tParametersCheck param );





static float FOCUS_MEASURAMENT = 30.0f;
static int MAX_PEAK_DIFF_VG = 20;




#endif
