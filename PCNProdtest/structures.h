#ifndef STRUCTURES_H
#define STRUCTURES_H
#define NOMINMAX
#include <vector>
#include <Windows.h>
//#pragma comment(lib,"Ws2_32.lib")
//#include <WinSock2.h>

#include "define_file.h"

typedef struct tAcqThreadParameters{
    SOCKET *imgfd;
    struct sockaddr_in *my_addr_data;
    struct sockaddr_in *serv_addr_data;
    struct sockaddr_in *serv_addr;

    tAcqThreadParameters(){
        imgfd = 0;
        my_addr_data = 0;
        serv_addr_data = 0;
        serv_addr = 0;
    }

}tAcqThreadParam;

typedef struct tModeImages_{
    PixelType* imgSx;
    PixelType* imgDx;
    PixelType* imgDsp;
    tModeImages_(){
        imgSx = 0;
        imgDx = 0;
        imgDsp = 0;
    }
}tModeImages;

typedef struct tParametersCheck_{
    float FOCUS_MEASURAMENT;
    // Per eliminazione nodi interni
    int min_num_pixel_for_node;
    // Per il controllo di forma
    int max_row_delta;
    int min_num_pixel_for_centroid;
    tParametersCheck_(){
        FOCUS_MEASURAMENT = 30.0f;
        min_num_pixel_for_node = 30;
        max_row_delta = 25;
        min_num_pixel_for_centroid = 400;
    }
}tParametersCheck;

typedef struct tParametersGeneral_{
    QString pathCalibrationDat;
    QString pathReport;
    //QString serialString;
    tParametersGeneral_(){
        pathCalibrationDat = "C:/";
        pathReport = "C:/";
        //serialString = "test_serial_port";
    }
}tParametersGeneral;

typedef struct tParameters_{
    tParametersGeneral paramGeneral;
    tParametersCheck paramCheck;
}tParameters;

/** Struttura per il calcolo del grafo delle disparit√  */
typedef struct tNode_{
    PixelType value;
    int label;
    std::vector<int> positions;					//!< Posizioni nelle immagini
    std::vector< std::pair<int,int> > neighbors;				  //!< Archi pesati
    //std::vector<int> neighbors_weights;	//!< Puntatore ai pesi dei collegamenti fra i nodi
    tNode_(){
        value = -1; //!< Valore identificativo
        }
    }tNode;

typedef struct tResultsCheckGraph_{
    std::string name;
    bool response;
    std::vector<int> positions;

    tResultsCheckGraph_(){
        response = false;
        }
    }tResultsCheckGraph;
#endif // STRUCTURES_H
