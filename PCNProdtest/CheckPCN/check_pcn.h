/*!
 * \file check_pcn.h
 * \brief File di header per la definizione di define e funzioni
 *
 * \author Omar Zandonà (eVS - embedded Vision Systems s.r.l. www.embeddedvisionsystems.it)
 */

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
#define BORDER_X 12  //!< Bordo nero (colonne) dimensione dell' immagine proveniente dal PCN dovuto alla correlazione
#define BORDER_Y 8   //!< Bordo nero (righe) dimensione dell' immagine proveniente dal PCN dovuto alla correlazione
#define BORDER_X_2 18 //!< Secondo bordo nero (colonne) dimensione dell' immagine proveniente dal PCN dovuto alla correlazione

#define NY 120 //!< Altezza immagine proveniente dal PCN
#define NX 160 //!< Larghezza immagine proveniente dal PCN
#define NN NX*NY //!< Dimensione dell' immagine proveniente dal PCN

#define H_CROP 104 //!< Altezza dell'imamgine tagliata// NY-2*BORDER_Y
#define	W_CROP 130 //!< Larghezza dell'imamgine tagliata //NX-(BORDER_X+BORDER_X_2)
#define CROP H_CROP*W_CROP  //!< Dimensione dell'immagine tagliata


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
#define KERNEL_DIM 9  //!< Dimensione del filtro di sobel
#define RADIUS 3 //!< Raggio del kernel
const char KERNEL_H_SOBEL[KERNEL_DIM] = {1,0,-1,2,0,-2,1,0,-1}; //!< Filtro orizzontale
const char KERNEL_V_SOBEL[KERNEL_DIM] = {1,2,1,0,0,0,-1,-2,-1}; //!< Filtro verticale
#define PI 3.14159265

// LAPLACIAN KERNEL
static double laplacian_kernel [KERNEL_DIM]= { 0.1667,0.6667f,0.1667f,0.6667f,-3.3333f,0.6667f,0.1667,0.6667f,0.1667f};
#define LAP_RADIUS 3  //!< Raggio del kernel del laplaciano

/****** Calc Features ******/


void printFeatures(float* features);//!< Visualizza il valore delle feature

unsigned char* CropImage(unsigned char* img, const int& new_w, const int& new_h);//!< Taglia le immagini delle dimensioni specificate come parametro

float compute_focus_measurement(unsigned char*  img,
                                const int& w,
                                const int& h);//!< Restituisce la misura del fuoco

unsigned char* compute_laplacian(unsigned char*  img,
                                 const int& w,
                                 const int& h);//!< Calcola il laplacaiano dell'immagine passata come parametro

bool check_focus(unsigned char*  & img_dx,
                 unsigned char*  & img_sx,
                 const int& w,
                 const int& h);//!< Effettua il check del focus

void setParameters( tParametersCheck param );//!< Setta i parametri per il check


static float FOCUS_MEASURAMENT = 30.0f;  //!< Soglia del valore del fuoco
static int MAX_PEAK_DIFF_VG = 20;


#endif
