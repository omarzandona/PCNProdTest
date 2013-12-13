#ifndef DEFINE_FILE_H
#define DEFINE_FILE_H

#define NOMINMAX
#include "SOCKET/apc_defines.h"
//#include "CheckPCN/check_pcn.h"

#include <QDebug>


#define _VERSION_PROGRAM_ "1.0" // Nome versione programma
#define _N_VERSION_PROGRAM_ 1   // Numero versione programma

//#define _PROJECTS_EXTENSION_ ".pcnv"
#define _TITLE_WINDOW_ "PCNProdTest"

#define IP_ADRESS_PCN "10.100.10.100"

#define MAX_STR_LENGTH 256

#define BUFFERSIZE 4+6*4+3*NN+4+4+4+6*2       // header+6*ID+ImageSX+ImageDX+ImageDSP+counter_in+counter_out+count_true_false+6*CRC

#define PixelType unsigned char

#define NUM_FRAME_TO_PROCESS 50

#define test_all(X) if ((void *)(X) == NULL) { QMessageBox::critical(NULL, "PCNProdTest", "ERROR: out of memory"); return; }
#define test_alloc(X) do {if ((void *)(X) == NULL) fprintf(stderr, "Out of memory in %s, (%s, line %d).\n", __FUNCTION__, __FILE__, __LINE__); } while (0)


typedef unsigned int uint;



#define N_BINS 256


//!< Values for feature rate
/*#define MAX_ERR_MEAN 10						// percentage
#define MAX_ERR_VAR 10						// percentage
#define MAX_NUM_BLACK_PIXEL 100
#define MAX_NUM_UNIFORM_PIXEL 5
#define MAX_SAD 20000
#define MIN_NCC 90								// percentage
#define MAX_SSD 10								// percentage*/
#define MAX_MINKOWSKI_DISTANCE  0.2
#define NUM_DISP_FOUND 5

//!< ProprietÃ  delle immagini
//#define H_CROP 104
//#define	W_CROP 136
//#define CROP H_CROP*W_CROP
//#define BORDER_X 12
//#define BORDER_Y 8
//#define NY 120
//#define NX 160



#endif
