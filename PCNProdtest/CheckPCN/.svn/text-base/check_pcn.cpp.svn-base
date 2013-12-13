#include "check_pcn.h"

static int val_pattern_disp[NUM_LEVEL_DISP] = {DISP_1*16,DISP_2*16,DISP_3*16,DISP_4*16,DISP_5*16};
static float distance_histogram_th[6] = {MAX_MINKOWSKI_DISTANCE,
                                         MAX_HIST_INTERSECT_DISTANCE,
                                         MAX_CHIQUADRO_DISTANCE,
                                         MAX_KOLMOGOROV_DISTANCE,
                                         MAX_COMULATIVE_DISTANCE,
                                         MAX_QFD_DISTANCE };

unsigned char* patternRefGenerate(int* _levels){
    int n_levels = NUM_LEVEL_DISP;
    int* levels = (int*)calloc(n_levels, sizeof(int));
    int img_h = H_CROP;
    int img_w = W_CROP;

    if (_levels != NULL){
        for (int i = 0; i < n_levels; ++i)
            levels[i] = _levels[i];
    }
    else{
        n_levels = NUM_LEVEL_DISP;
        levels[0] = DISP_1;
        levels[1] = DISP_2;
        levels[2] = DISP_3;
        levels[3] = DISP_4;
        levels[4] = DISP_5;
    }
    int* val = (int*)calloc(n_levels, sizeof(int));
    for (int i = 0; i < n_levels; ++i)
        val[i] = *(levels + i) * 16;

    unsigned char* img = (unsigned char*)calloc(img_h*img_w, sizeof(unsigned char));

    int k = 0;

    int* limit = (int*)calloc(4, sizeof(int));
    limit[0] = 17 * (img_w);
    limit[1] = 41 * (img_w);
    limit[2] = 62 * (img_w);
    limit[3] = 89 * (img_w);

    for (int i = 0; i < img_h*img_w; ++i){
        if (i == limit[k] && k < 4)
            k++;
        img[i] = val[k];
    }

    free(levels);
    free(limit);
    free(val);

    return img;
}



/****** Feature *******/

//!< Mean
float meanImage(unsigned char* img, const int& w, const int& h){
    float sum = 0.0f;
    int size = w*h;
    for (int i = 0; i < size; ++i)
        sum += img[i];
    return sum / (w*h);
}

//!< Variance
float varImage(unsigned char* img, const int& w, const int& h){
    float mean = meanImage(img, w, h);
    float out = 0.0f;
    int size = w*h;
    for (int i = 0; i < size; ++i)
        out += ((img[i] - mean)*(img[i] - mean));
    return out / size;
}

//!< Normalized cross-correlation
float nccImage(unsigned char* img, unsigned char* templ, const int& w, const int& h){
    float res = 0.0f;
    float mT = 0.0f;
    int size = w*h;
    for (int i = 0; i < size; ++i){
        mT += templ[i];
    }
    mT = mT / size;

    float mI = 0.0f;
    for (int i = 0; i < size; ++i){
        mI += img[i];
    }
    mI = mI / size;

    float num = 0.0f;
    float den1 = 0.0f;
    float den2 = 0.0f;
    float T = 0.0f;
    float I = 0.0f;

    for (int i = 0; i < size; ++i){
        T = templ[i] - mT;
        I = img[i] - mI;
        num += (T*I);
        den1 += (T*T);
        den2 += (I*I);
    }

    res = num / sqrt(den1*den2);
    return res;
}

//!< SSD
float ssdImage(unsigned char* img_1, unsigned char* img_2, const int& w, const int& h){
    int size = w*h;
    unsigned char I = 0, T = 0, num = 0;
    for (int i = 0; i < size; ++i)
    {
        I += img_1[i];
        T += img_2[i];
        num += ((img_1[i] - img_2[i])*(img_1[i] - img_2[i]));
    }
    return num / sqrt((double)I*T);
}

//!< SAD
float sadImage(unsigned char* img_1, unsigned char* img_2, const int& w, const int& h){
    int size = w*h;
    unsigned char num = 0;
    for (int i = 0; i < size; ++i)
    {
        num += abs(img_1[i] - img_2[i]);
    }
    return num;
}

//!< Num black pixels
int nBlackPixels(unsigned char* img, const int& w, const int& h){
    int size = w*h;
    int out = 0;
    for (int i = 0; i < size; ++i)
        if (img[i] == 0)
            out++;
    return out;
}

//!< Num uniform pixels
int nUniformPixels(unsigned char* img, const int& w, const int& h){
    int size = w*h;
    int out = 0;
    for (int i = 0; i < size; ++i)
        if (img[i] == UNIFORM_VALUE)
            out++;
    return out;
}

//!< Compute distance between histogram
float* calcHist(unsigned char* img, const int& w, const int& h, bool norm){
    int size = w*h;
    float* hist = (float*)calloc(N_BINS, sizeof(float));
    for (int i = 0; i < size; ++i)
        hist[img[i]]++;
    if (norm)	// Normalizza
        for (int i = 0; i < N_BINS; ++i)
            hist[i] = hist[i] / size;
    return hist;
}

float calcMinkowski(float* hist_1, float* hist_2){
    float distance = 0.0f;
    for (int i = 0; i < N_BINS; ++i)
        distance += abs(hist_1[i] - hist_2[i]);

    return distance;
}

float calcHistogramIntersect(float* hist_1, float* hist_2){
    float sum = 0.0f, temp = 0.0f;

    for (int i = 0; i < N_BINS; ++i)
    {
        temp += min(hist_1[i], hist_2[i]);
        sum += hist_2[i];
    }

    return 1 - temp / sum;
}

float* calcHistCum(float* hist){
    float* hist_cum = (float*)calloc(N_BINS, sizeof(float));
    hist_cum[0] = hist[0];
    for (int i = 1; i < N_BINS; ++i)
        hist_cum[i] = hist[i] + hist_cum[i - 1];
    // Normalizzazione
    for (int i = 0; i < N_BINS; ++i)
        hist_cum[i] /= hist_cum[N_BINS - 1];
    return hist_cum;
}

float calcCumulativeHistograms(float* hist_1, float* hist_2){
    float distance = 0.0f;
    float* hist_cum_1 = calcHistCum(hist_1);
    float* hist_cum_2 = calcHistCum(hist_2);
    for (int i = 0; i < N_BINS; ++i)
        distance += abs(hist_cum_1[i] - hist_cum_2[i]);
    free(hist_cum_1);
    free(hist_cum_2);
    return distance;
}

float calcChiQuadro(float* hist_1, float* hist_2){
    float distance = 0.0f;
    float mean = 0.0f;
    for (int i = 0; i < N_BINS; ++i){
        mean = (hist_1[i] + hist_2[i]) / 2;
        if (mean != 0)
            distance += ((hist_1[i] - mean)*(hist_1[i] - mean)) / mean;
    }
    return distance;
}


float calcKolmogorov(float* hist_1, float* hist_2){
    float distance = 0.0f;
    float max_distance = 0.0f;
    for (int i = 0; i<N_BINS; ++i){
        max_distance = abs(hist_1[i] - hist_2[i]);
        if (max_distance > distance)
            distance = max_distance;
    }
    return distance;
}

float quadraticFromDistance(float* hist_1, float* hist_2){
    return 1.0f;
}


void computeDistHist(unsigned char* img_1, unsigned char* img_2, const int& w, const int& h, float& dis_1, float& dis_2, float& dis_3, float& dis_4, float& dis_5, float& dis_6){
    /*
    Output distance = vettore di distanze
    dist_1)Minkowski (l1) (bin-by-bin)
    dist_2)Histogram Intersect (bin-by-bin)
    dist_3)istogrammi cumulativi (bin-by-bin)
    dist_4)distanza CHI-Quadro
    dist_5)Kolmogorov
    dist_6)Quadratic-from-distance (cross-bin)
    */

    float* hist_1 = calcHist(img_1, w, h);
    float* hist_2 = calcHist(img_2, w, h);

    dis_1 = calcMinkowski(hist_1, hist_2);
    dis_2 = calcHistogramIntersect(hist_1, hist_2);
    dis_3 = calcCumulativeHistograms(hist_1, hist_2);
    dis_4 = calcChiQuadro(hist_1, hist_2);
    dis_5 = calcKolmogorov(hist_1, hist_2);
    dis_6 = quadraticFromDistance(hist_1, hist_2);

    free(hist_1);
    free(hist_2);
}

//!< check_histogram_distance
float check_histogram_distance(unsigned char*  & img,
                               unsigned char*  & pattern,
                               const int& w,
                               const int& h,
                               int num_dist)

{
    float* hist_1 = calcHist(img, w,h,true);
    float* hist_2 = calcHist(pattern,w,h,true);
    float distance;
    switch (num_dist)
    {
    case 1:   distance = calcMinkowski(hist_1, hist_2);
        break;
    case 2:   distance = calcHistogramIntersect(hist_1, hist_2);
        break;
    case 3:   distance =  calcCumulativeHistograms(hist_1, hist_2);
        break;
    case 4:   distance =  calcChiQuadro(hist_1, hist_2);
        break;
    case 5:   distance =calcKolmogorov(hist_1, hist_2);
        break;
    case 6:   distance = quadraticFromDistance(hist_1, hist_2);
        break;

    }
    free(hist_1);
    free(hist_2);
    return distance;
}

//!< Num disp found
int numDispFound(unsigned char* img, const int& w, const int& h)
{
    float* hist = calcHist(img, w, h, false);
    int out = 0;

    for (int i = 0; i < N_BINS; ++i)
        if (hist[i] > 0 && i > UNIFORM_VALUE)
        {
            out++;
        }

    free(hist);
    return out;
}

//!< Val disp found
unsigned char* ValDispFound(unsigned char* img, const int& w, const int& h)
{
    qDebug() << "DisparitÃ  trovate";
    float* hist = calcHist(img, w, h, false);
    int num_disp_found = numDispFound(img, w, h);
    unsigned char* disp_found = (unsigned char*)malloc(num_disp_found*sizeof(unsigned char));
    int idx = 0;
    for (int i = 0; i < N_BINS; ++i)
    {
        if (hist[i] > 0 && i > UNIFORM_VALUE)
        {
            qDebug() << i;
            disp_found[idx] = i;
            ++idx;
        }
    }
    free(hist);
    return disp_found;
}
bool check_disp(unsigned char* disp_found,const int& num_disp_found)
{
    bool res = true;
    for ( int i = 0;i < num_disp_found;++i)
    {
        // qDebug() << disp_found[i] << val_pattern_disp[i];
        res = (disp_found[i] == val_pattern_disp[i]);

    }
    return res;
}

/*
Crop Image
*/
unsigned char* CropImage(unsigned char* img, const int& new_w, const int& new_h)
{

    int sz_crop_image = new_w*new_h;
    unsigned char* crop_image = (unsigned char*)malloc(sz_crop_image*sizeof(unsigned char));
    unsigned char* ptr_crop_image;
    unsigned char* ptr_img;
    int cr = 0;
    for (int r = BORDER_Y; r < NY - BORDER_Y; ++r, ++cr)
        {
        ptr_img = &img[r*NX + BORDER_X];
        ptr_crop_image = &crop_image[cr*new_w];
        for (int c = BORDER_X; c < NX - BORDER_X_2; ++c, ++ptr_img, ++ptr_crop_image)
            {
            *ptr_crop_image = *ptr_img;
            }

        }
    return crop_image;
}
/*
 Gradient
*/
void Gradient(unsigned char* &  i_img,
              const int &  i_rows,
              const int & i_cols,
              double* &  o_img_hg,
              double* & o_img_vg)
{

    o_img_hg = conv2D(i_img, i_rows, i_cols, KERNEL_H_SOBEL);

    o_img_vg = conv2D(i_img, i_rows, i_cols, KERNEL_V_SOBEL);

}
/*
conv2D
*/
double* conv2D(unsigned char* &  i_img,
               const int &  i_rows,
               const int &  i_cols,
               const char i_kernel[])
{
    int radius = (int)RADIUS / 2;
    int sz = i_rows*i_cols;
    int rows_pd;
    int cols_pd;

    // PADDING
    unsigned char* padding_img = padding(i_img, i_rows, i_cols, radius, rows_pd, cols_pd);
    int sz_pd = cols_pd*rows_pd;

    // Compute gradient of PADDING image (fixed)
    unsigned char* ptr_pd_img = &padding_img[radius*cols_pd];
    double* pd_conv_img = (double*)calloc(sz_pd, sizeof(double));
    for (int i = radius; i < rows_pd - radius; ++i)
    {
        for (int j = radius; j < cols_pd - radius; ++j)
        {
            int tmp = 0;
            const char* ptr_i_kernel = &i_kernel[0];
            for (int r = i - radius; r <= i + radius; ++r)
                for (int c = j - radius; c <= j + radius; ++c, ++ptr_i_kernel)
                    tmp += int((padding_img[r*cols_pd + c])*(int)(*ptr_i_kernel));


            pd_conv_img[i*cols_pd + j] = double(-tmp / FACT_SOBEL_KERNEL);
        }
    }
    // DE - PADDING
    double* o_img = depadding(pd_conv_img, rows_pd, cols_pd, radius, i_rows, i_cols);
    free(pd_conv_img);
    return o_img;
}

/*
 *padding
*/
unsigned char* padding(unsigned char* const & i_img,
                       const int  & i_rows,
                       const  int  & i_cols,
                       int const & i_radius,
                       int  & io_rows_pd,
                       int  & io_cols_pd)
{
    io_rows_pd = (i_rows + i_radius * 2);
    io_cols_pd = (i_cols + i_radius * 2);
    unsigned char* o_padding_img = (unsigned char*)calloc(io_rows_pd*io_cols_pd, sizeof(unsigned char));
    for (int i = i_radius; i < io_rows_pd - i_radius; ++i)
    {
        unsigned char* ptr_pd_img = &o_padding_img[i*io_cols_pd + i_radius];
        for (int j = i_radius; j < io_cols_pd - i_radius; ++j, ++ptr_pd_img)
            *ptr_pd_img = i_img[(i - i_radius)*i_cols + (j - i_radius)];
    }

    return o_padding_img;
}
/*
depadding
*/
double* depadding(double* &  i_pad_img,
                  int &  i_rows_pd,
                  int &  i_cols_pd,
                  int & i_radius,
                  const int &  i_rows,
                  const  int &  i_cols)
{
    double* o_depadded_img = (double*)calloc(i_rows*i_cols, sizeof(double));
    for (int i = i_radius; i < i_rows_pd - i_radius; ++i)
        for (int j = i_radius; j < i_cols_pd - i_radius; ++j)
            o_depadded_img[(i - i_radius)*i_cols + (j - i_radius)] = i_pad_img[i*i_cols_pd + j];

    return o_depadded_img;
}

/*
 Magnitude
*/
unsigned char* Magnitude(double* &  o_img_hg,
                         double* &  o_img_vg,
                         const  int &  i_cols,
                         const int &  i_rows)
{
    int size = i_rows*i_cols;
    unsigned char* magnitude = (unsigned char*) malloc(size*sizeof(unsigned char));
    for (int i = 0; i < size; ++i)
        magnitude[i] = abs(int(o_img_hg[i])) + abs(int(o_img_vg[i]));

    return magnitude;
}

/*void compute_laplacian(unsigned char* & img,
                       unsigned char* & hg,
                       unsigned char* & vg,
                       const int& w,
                       const int& h)
{
    unsigned char* prev_row_ptr;
    unsigned char* row_ptr;
    for (int y = 1; y < h-1 ; ++y)
    {
        prev_row_ptr = &(img[(y-1) * w]);
        row_ptr = &(img[y * w]);
        for (int x = 0; x < w; ++x,++prev_row_ptr,++row_ptr,++hg,++vg)
        {
            *hg = (unsigned char) abs(*row_ptr - *(row_ptr-1));
            *vg = (unsigned char) abs(*row_ptr - *prev_row_ptr);
        }
    }
}
*/
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
 * compute_img_diff
* */
unsigned char* compute_img_diff(unsigned char*  & img_test,
                                unsigned char*  & img_ref,
                                const int& w,
                                const int& h)
{
    int sz = w*h;
    unsigned char* img_diff = (unsigned char*) calloc(sz,sizeof(unsigned char));
    unsigned char* ptr_img_diff= img_diff;
    unsigned char* ptr_img_ref = img_ref;
    unsigned char* ptr_img_test = img_test;

    for ( int i = 0; i < sz; ++i,++ptr_img_test,++ptr_img_ref,++ptr_img_diff)
    {

        *ptr_img_diff = (unsigned char) abs((int)*ptr_img_ref - (int) *ptr_img_test );

    }

    return img_diff;

}


/*
  find_max_value
*/
unsigned char find_max_value(unsigned char*  & img,
                             const int& w,
                             const int& h)
{
    unsigned char tmp = 0;
    unsigned char* ptr_img = img;
    for ( int i = 0; i < w*h; ++i,++ptr_img)
        tmp = max(tmp,*ptr_img);

    return tmp;
}

/*
 CheckPCN
*/

std::vector< std::pair<QString,bool> > CheckPCN(unsigned char*  & img_orig_dsp,
                                                unsigned char*  & img_orig_dx,
                                                unsigned char*  & img_orig_sx,
                                                const int& w,
                                                const int& h)
{
    std::vector< std::pair<QString,bool> > results_check;

    unsigned char*  img_dsp = CropImage(img_orig_dsp,W_CROP,H_CROP);
    unsigned char*  img_dx = CropImage(img_orig_dx,W_CROP,H_CROP);
    unsigned char*  img_sx = CropImage(img_orig_sx,W_CROP,H_CROP);
    unsigned char*  pattern_dsp = patternRefGenerate();
    unsigned char*  pattern_dx = CropImage(Utils::rgb2gray(QImage(QCoreApplication::applicationDirPath() + "/imgs_ref/1_imgDx.png","PNG")),W_CROP,H_CROP);
    unsigned char*  pattern_sx = CropImage(Utils::rgb2gray(QImage(QCoreApplication::applicationDirPath() + "/imgs_ref/1_imgSx.png", "PNG")),W_CROP,H_CROP);
    QString path_dx("C:/Users/Omar/Dropbox/condivisa_EVS/DatasetsQT/testing/tmp_dx.png");
    QString path_sx("C:/Users/Omar/Dropbox/condivisa_EVS/DatasetsQT/testing/tmp_sx.png");
    Utils::saveImage(pattern_dx,path_dx);
    Utils::saveImage(pattern_sx,path_sx);

    results_check.push_back(std::make_pair("Focus:         ",check_focus(img_dx,img_sx, w, h)));

    //results_check.push_back(std::make_pair("Disparity:     ",check_odc(img_dsp,pattern_dsp, w, h)));
    GraphDisparity graph;	//!< Classe per il testing della disparità
    vector< tResultsCheckGraph > testDisp = graph.check(img_dsp, W_CROP, H_CROP);
    for( int i=0; i<testDisp.size(); ++i )
        results_check.push_back( std::make_pair(QString::fromStdString(testDisp.at(i).name), testDisp.at(i).response) );

    //results_check.push_back(std::make_pair("FPN:          ",check_fpn(img_dsp,img_dx,img_sx,
    //                                                                  pattern_dsp,pattern_dx,pattern_sx, w, h)));


   // results_check.push_back(std::make_pair("Impurity:     ",check_impurity(img_dsp,img_dx,img_sx,
      //                                                                     pattern_dsp,pattern_dx,pattern_sx, w, h)));

    //results_check.push_back(std::make_pair("Black pixels: ",check_blackpixel(img_dsp,w, h)));

    free(pattern_dsp);
    free(pattern_dx);
    free(pattern_sx);

    return results_check;
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

    //float p_dx = compute_focus_measurement(pattern_dx,w,h);
    //float p_sx = compute_focus_measurement(pattern_sx,w,h);
#ifdef DEBUG
    //qDebug() <<"Focus measure: "<< fm_dx << fm_sx  ;
#endif
    bool res = ( fm_dx > FOCUS_MEASURAMENT && fm_sx > FOCUS_MEASURAMENT);
    // res = fm_dx < && fm_sx <
    return res;
}

/*
 check_odc
*/
bool check_odc(unsigned char*  & img_dsp,unsigned char*  & pattern_dsp, const int& w, const int& h)
{

    int num_disp_found = numDispFound(img_dsp,w,h);
    unsigned char* disp_found = ValDispFound(img_dsp,w,h);
    int uniform_pixel = nUniformPixels(img_dsp,w,h);
    bool dsp_check  = check_disp(disp_found,num_disp_found);
    int num_dist = 1;
    float hist_distance = check_histogram_distance(img_dsp,pattern_dsp,w,h,num_dist);
#ifdef DEBUG
    qDebug() <<"Disparity:";
    qDebug() <<"  Number disparity:" << num_disp_found;
    qDebug() <<"  Number uniform pixel:"<< uniform_pixel;
    qDebug() <<"  Distance histogram: :"<< hist_distance;
    qDebug() <<"  Disparity check:"<< dsp_check ;
#endif
    bool res = ( (hist_distance < distance_histogram_th[num_dist])
                 && (num_disp_found == NUM_LEVEL_DISP)
                 && dsp_check
                 && (uniform_pixel < MAX_NUM_UNIFORM_PIXEL)
                 );

    free(disp_found);
    return res;
}

/*
 check_fpn
*/
bool check_fpn(unsigned char*  & img_dsp,
               unsigned char*  & img_sx,
               unsigned char*  & img_dx,
               unsigned char*  & pattern_dsp,
               unsigned char*  & pattern_sx,
               unsigned char*  & pattern_dx,
               const int& w,
               const int& h)
{

    float mean_pattern_dsp = meanImage( pattern_dsp, w, h);
    float mean_dsp = meanImage(img_dsp, w, h);

    float mse_dx = compute_mse(img_dx,pattern_dx,w,h);
    float mse_sx = compute_mse(img_sx,pattern_sx,w,h);
    bool res_mse = (mse_dx < MAX_MSE && mse_sx < MAX_MSE);
#ifdef DEBUG
    qDebug()<<"FPN:" ;
    qDebug()<<" mean pattern:"<< mean_pattern_dsp;
    qDebug()<<" mean test:"<<  mean_dsp;
    qDebug()<<" NCC:"<< (nccImage(img_dsp,pattern_dsp,w,h));
    qDebug()<<" SAD:"<<  (sadImage(img_dsp,pattern_dsp,w,h));
    qDebug()<<" MSE dsp:"<<  res_mse;
#endif
    bool res_dsp = ( (mean_dsp >= (mean_pattern_dsp)*0.8f && mean_dsp <= (mean_pattern_dsp)*1.2f)
                     && (nccImage(img_dsp,pattern_dsp,w,h) > MIN_NCC)
                     && (sadImage(img_dsp,pattern_dsp,w,h) < MAX_SAD)
                     //&& res_mse
                     ) ;

    return  res_dsp;
}

/*
 check_impurity
*/
bool check_impurity(unsigned char*  & img_dsp,
                    unsigned char*  & img_dx,
                    unsigned char*  & img_sx,
                    unsigned char*  & pattern_dsp,
                    unsigned char*  & pattern_dx,
                    unsigned char*  & pattern_sx,
                    const int& w,
                    const int& h)
{
    // Right image
    unsigned char* abs_diff_dx = compute_img_diff(img_dx,pattern_dx,w,h);
    unsigned char max_value_dx = find_max_value(abs_diff_dx,w,h);
    // Left image
    unsigned char* abs_diff_sx = compute_img_diff(img_sx,pattern_sx,w,h);
    unsigned char max_value_sx = find_max_value(abs_diff_sx,w,h);
    // Find difference after registration
    unsigned char max_value_1 =  find_peaks(img_dx,pattern_dx,w,h);
    unsigned char max_value_2 =  find_peaks(img_sx,pattern_sx,w,h);
    // Find difference gradient
    unsigned char max_vg_sx = find_diff_gradient_peak(img_sx,pattern_sx,w,h);
    unsigned char max_vg_dx = find_diff_gradient_peak(img_dx,pattern_dx,w,h);

#ifdef DEBUG
    qDebug() <<"Impurity: ";
    qDebug() <<" Impurity diff reg: " << max_value_1 <<  max_value_2;
    qDebug() <<" Impurity diff grad: " << max_vg_dx <<  max_vg_sx;
    qDebug() <<" Impurity diff img: " << max_value_dx <<  max_value_sx;
#endif
    bool res = (max_vg_sx < MAX_PEAK_DIFF_VG && max_vg_dx < MAX_PEAK_DIFF_VG)
            //&&(max_value_dx < MAX_PEAK_DIFF_IMAGE && max_value_sx < MAX_PEAK_DIFF_IMAGE)
            ;

    free(abs_diff_dx);
    free(abs_diff_sx);
    return res;
}

unsigned char find_diff_gradient_peak(unsigned char* img,
                                      unsigned char* pattern,
                                      const int& w,
                                      const int& h)
{
    double* tmp_hg;
    double* tmp_vg;
    Gradient(img,h,w,tmp_hg,tmp_vg);
    unsigned char* abs_grad_img = abs_gradient(tmp_vg,w,h);

    Gradient(pattern,h,w,tmp_hg,tmp_vg);
    unsigned char* abs_grad_pattern = abs_gradient(tmp_vg,w,h);

    unsigned char* abs_diff = compute_img_diff(abs_grad_img,abs_grad_pattern,w,h);
    unsigned char max_diff = find_max_value(abs_diff,w,h);


    free(tmp_hg);
    free(tmp_vg);
    free(abs_grad_img);
    free(abs_grad_pattern);
    free(abs_diff);

    return max_diff;
}

unsigned char* abs_gradient(double* & img,
                            const int& w,
                            const int& h)
{
    int sz = w*h;
    unsigned char* abs_img = (unsigned char*) calloc(sz,sizeof(unsigned char));
    for ( int i = 0; i < sz; ++i)
        abs_img[i] = (unsigned char)abs(img[i]);

    return abs_img;
}

/*
 check_blackpixel
*/
bool check_blackpixel(unsigned char*  & img_dsp,
                      const int& w,
                      const int& h)
{
    bool res = (nBlackPixels(img_dsp,w,h) < MAX_NUM_BLACK_PIXEL);
#ifdef DEBUG
    qDebug() <<"BlackPixels: " << nBlackPixels(img_dsp,w,h);
#endif
    return res;
}



double compute_mse(unsigned char*  & img_test,
                   unsigned char*  & img_ref,
                   const int& w,
                   const int& h)
{

    double tmp = 0;
    int sz = w*h;
    for (int i = 1; i < sz; ++i)
    {
        double diff = double( (img_test[i] - img_ref[i])*(img_test[i] - img_ref[i]) );
        tmp = tmp + diff;
    }

    double   mse = tmp/sz;
    return mse;
}

unsigned char find_peaks(unsigned char* img,
                         unsigned char* pattern,
                         const int& w,
                         const int& h)
{

    double* tmp_hg;
    double* tmp_vg;
    Gradient(img,h,w,tmp_hg,tmp_vg);
    int c_img = find_coordinate_edge(tmp_hg,w,h);


    Gradient(pattern,h,w,tmp_hg,tmp_vg);
    int c_pattern = find_coordinate_edge(tmp_hg,w,h);

    qDebug() << c_img << c_pattern;
    int index_start_img = 0;
    int index_start_pattern = 0;
    if (c_img < c_pattern )
    {
        index_start_img = 0 ;
        index_start_pattern = c_pattern - c_img;
    }
    else
    {
        index_start_img = c_img - c_pattern ;
        index_start_pattern = 0 ;
    }

    unsigned char* img_align;
    if (c_img != c_pattern)
        img_align =  align_img(img,pattern, w,h,index_start_img,index_start_pattern);
    else
    {
        img_align =  (unsigned char*) malloc(w*h*sizeof(unsigned char));
        memcpy(img_align,pattern,w*h);
    }

    unsigned char* abs_diff = compute_img_diff(img_align,pattern,w,h);
    unsigned char max_diff = find_max_value(abs_diff,w,h);


    free(tmp_hg);
    free(tmp_vg);
    free(img_align);
    free(abs_diff);

    return max_diff;
}


int find_coordinate_edge(double* & img,
                         const int& w,
                         const int& h)
{
    bool find = false;
    int ret = 0;
    int sign = (img[0] > 0) ? 1 : -1;
    for (int k = 0; k < h*w && !find ; ++k )
    {
        int tmp = (img[k] > 0) ? 1 : -1;
        if ( tmp != sign)
        {
            ret = k;
            find = true;
        }
    }

    return ret;
}

unsigned char* align_img(unsigned char*  & img,
                         unsigned char*  & pattern,
                         const int& w,
                         const int& h,
                         int & index_start_img,
                         int & index_start_pattern
                         )
{
    int size = w*h;
    unsigned char* img_aligned = (unsigned char*) malloc(size*sizeof(unsigned char));
    memcpy(img_aligned,pattern,w*h);

    for (int r = 0; r < h ; ++r )
    {
        int cp = index_start_img;
        for (int c = index_start_pattern; c < w; ++c,++cp)
        {
            img_aligned[r*w + c] = img[r*w + cp];
        }
    }
    return img_aligned;
}

void setParameters( tParametersCheck param ){
    FOCUS_MEASURAMENT = param.FOCUS_MEASURAMENT;
}
