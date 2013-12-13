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
//!< Genera il pattern di riferimento (sono previsti NUM_LEVEL_DISP livelli)
unsigned char* patternRefGenerate(int* _levels = NULL);

/****** Calc Features ******/
//!< Mean
float meanImage(unsigned char* img, const int& w, const int& h);
//!< Variance
float varImage(unsigned char* img, const int& w, const int& h);
//!< NCC
float nccImage(unsigned char* img, unsigned char* templ, const int& w, const int& h );
//!< SSD
float ssdImage(unsigned char* img_1, unsigned char* img_2, const int& w, const int& h );
//!< SAD
float sadImage(unsigned char* img_1, unsigned char* img_2, const int& w, const int& h );
//!< Num black pixels
int nBlackPixels(unsigned char* img, const int& w, const int& h);
//!< Num uniform pixels
int nUniformPixels(unsigned char* img, const int& w, const int& h);
//!< Compute distance between histogram
float* calcHist(unsigned char* img, const int& w, const int& h, bool norm = true);
float* calcHistCum(float* hist);
float calcMinkowski(float* hist_1, float* hist_2);
float calcHistogramIntersect(float* hist_1, float* hist_2);
float calcCumulativeHistograms(float* hist_1, float* hist_2);
float calcChiQuadro(float* hist_1, float* hist_2);
float calcKolmogorov(float* hist_1, float* hist_2);
float quadraticFromDistance(float* hist_1, float* hist_2);
void computeDistHist(unsigned char* img_1,
                     unsigned char* img_2,
                     const int& w,
                     const int& h,
                     float& dis_1,
                     float& dis_2,
                     float& dis_3,
                     float& dis_4,
                     float& dis_5,
                     float& dis_6);
//!< Num disp found
int numDispFound(unsigned char* img, const int& w, const int& h);
//!< Val disp found
unsigned char* ValDispFound(unsigned char* img, const int& w, const int& h);
//!< Rate features
//int rateFeature(float* f_ref, float* f_test);
//!< Print values features
void printFeatures(float* features);
//!< check_disp
bool check_disp(unsigned char* disp_found,const int& num_disp_found);
//!< Crop Image
unsigned char* CropImage(unsigned char* img, const int& new_w, const int& new_h);
//!< compute_laplacian
/*void compute_laplacian(unsigned char* & img,
                       unsigned char* & hg,
                       unsigned char* & vg,
                       const int& w,
                       const int& h);
*/
float compute_focus_measurement(unsigned char*  img,
                                const int& w,
                                const int& h);

unsigned char* compute_laplacian(unsigned char*  img,
                                 const int& w,
                                 const int& h);


//!< Gradient
void Gradient(unsigned char* &  i_img,
              const int &  i_rows,
              const int &  i_cols,
              double* &  o_img_hg,
              double* &  o_img_vg);

unsigned char* abs_gradient(double* & img,
                            const int& w,
                            const int& h);

unsigned char find_diff_gradient_peak(unsigned char* img,
                                      unsigned char* pattern,
                                      const int& w,
                                      const int& h);

int find_coordinate_edge(double* & img,
                         const int& w,
                         const int& h);
unsigned char* align_img(unsigned char*  & img,
                         unsigned char*  & pattern,
                         const int& w,
                         const int& h,
                         int & index_start_img,
                         int & index_start_pattern
                         );
unsigned char find_peaks(unsigned char* img,
                         unsigned char* pattern,
                         const int& w,
                         const int& h);

//!< conv2D
double* conv2D(unsigned char* &  i_img,
               const int &  i_rows,
               const int & i_cols,
               const char i_kernel[]);
//!< depadding
double* depadding(double* & i_pad_img,
                  int &  i_rows_pd,
                  int &  i_cols_pd,
                  int &  i_radius,
                  const int & i_rows,
                  const int &  i_cols);
//!<  padding
unsigned char* padding(unsigned char* const & i_img,
                       const int  & i_rows,
                       const int  & i_cols,
                       int const & i_radius,
                       int  & io_rows_pd,
                       int  & io_cols_pd);

//!< Magnitude
unsigned char* Magnitude(double* & o_img_hg,
                         double* & o_img_vg,
                         const int &  i_cols,
                         const int &  i_rows);
//!< compute_img_diff
unsigned char* compute_img_diff(unsigned char*  & img_test,
                                unsigned char*  & img_ref,
                                const int& w,
                                const int& h);
//!< find_max_value

unsigned char find_max_value(unsigned char*  & img,
                             const int& w,
                             const int& h);

//!< compute mse

double compute_mse(unsigned char*  & img_test,
                   unsigned char*  & img_ref,
                   const int& w,
                   const int& h);


//!<check histogram distance
float check_histogram_distance(unsigned char*  & img,
                               unsigned char*  & pattern,
                               const int& w,
                               const int& h,
                               int num_dist);

//!< CheckPCN
std::vector< std::pair<QString,bool> > CheckPCN(unsigned char*  & img_dsp,
                                                unsigned char*  & img_dx,
                                                unsigned char*  & img_sx,
                                                const int& w,
                                                const int& h);
//!< check_focus
bool check_focus(unsigned char*  & img_dx,
                 unsigned char*  & img_sx,
                 const int& w,
                 const int& h);
//!< check_odc
bool check_odc(unsigned char*  & img_dsp,unsigned char*  & pattern_dsp, const int& w, const int& h);

//!< check_fpn
bool check_fpn(unsigned char*  & img_dsp,
               unsigned char*  & img_dx,
               unsigned char*  & img_sx,
               unsigned char*  & pattern_dsp,
               unsigned char*  & pattern_dx,
               unsigned char*  & pattern_sx,
               const int& w,
               const int& h);

//!< check_impurity
bool check_impurity(unsigned char*  & img_dsp,
                    unsigned char*  & img_dx,
                    unsigned char*  & img_sx,
                    unsigned char*  & pattern_dsp,
                    unsigned char*  & pattern_dx,
                    unsigned char*  & pattern_sx,
                    const int& w,
                    const int& h);
//!< check_blackpixel
bool check_blackpixel(unsigned char*  & img_dsp,
                      const int& w,
                      const int& h);

//!< Setta i parametri per il check
void setParameters( tParametersCheck param );



//!< Values for feature rate
static int MAX_ERR_MEAN = 10;						// percentage
static int MAX_ERR_VAR = 10;						// percentage
static int MAX_NUM_BLACK_PIXEL = 40;
static int MAX_NUM_UNIFORM_PIXEL = 10;
static int MAX_SAD = 250;
static float MIN_NCC = 0.90f;								// percentage
static float MAX_SSD = 0.30f;								// percentage
static int MAX_PEAK_DIFF_IMAGE = 35;
static int MAX_MSE = 55;

static float FOCUS_MEASURAMENT = 30.0f;
static int MAX_PEAK_DIFF_VG = 20;

//!< Values for histogram distance
//static int NUM_DIST_HISTO = 6;
//#define MAX_MINKOWSKI_DISTANCE  0.2f
#define MAX_HIST_INTERSECT_DISTANCE  0.2f
#define MAX_CHIQUADRO_DISTANCE  0.2f
#define MAX_KOLMOGOROV_DISTANCE  0.2f
#define MAX_COMULATIVE_DISTANCE  0.2f
#define MAX_QFD_DISTANCE  0.2


#endif
