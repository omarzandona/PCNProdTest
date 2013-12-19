/*!
 *\file structures.h
 *\brief File header contenente la definizione di alcune strutture utilizzate nel programma.
 *
 * Sono definite le strutture per la socket per il thread di acquisizione, per il salvataggio delle immagini moda, dei parametri dell'applicativo e del test.
 * Inoltre ? definita la struttura necessaria per l'analisi della disparit&agrave;
 *
 *\author Alessio Montagnini, Omar Zandon&agrave; (eVS - embedded Vision Systems s.r.l. www.embeddedvisionsystems.it)
 *
*/

#ifndef STRUCTURES_H
#define STRUCTURES_H
#define NOMINMAX
#include <vector>
#include <Windows.h>

#include "define_file.h"

/*!
 * Struttura per descrivere i parametri della socket per
 * l'acquisizione delle immagini
 *
*/
typedef struct tAcqThreadParameters{
    SOCKET *imgfd;  //!< File descriptor della socket
    struct sockaddr_in *my_addr_data;  //!< Struttura per la gestione degli indirizzi per la socket (lato client)
    struct sockaddr_in *serv_addr_data; //!< Struttura per la gestione degli indirizzi per la socket (lato server)
    struct sockaddr_in *serv_addr; //!< Struttura per la gestione degli indirizzi per la socket (lato server)

    /*!
     * \brief tAcqThreadParameters
     *  Costruttore di una struttura per la gestione dei parametri della socket vuota
     */
    tAcqThreadParameters(){
        imgfd = 0;
        my_addr_data = 0;
        serv_addr_data = 0;
        serv_addr = 0;
    }

}tAcqThreadParam;

/*!
 * Struttura per il salvataggio delle immagini "moda" ottenute dopo l'acquisizione
 * di 50 immagini di sinistra, destra e di disparit&agrave;
 */
typedef struct tModeImages_{
    PixelType* imgSx;
    PixelType* imgDx;
    PixelType* imgDsp;
    /*!
     * \brief tModeImages_
     * Costrutture struttura vuota tModeImages_
     */
    tModeImages_(){
        imgSx = 0;
        imgDx = 0;
        imgDsp = 0;
    }
}tModeImages;


/*!
 * Struttura per il settaggio dei parametri di check delle immagini
 *  \see GraphDisparity.cpp
 */
typedef struct tParametersCheck_{
    float FOCUS_MEASURAMENT;  //!< Soglia per la misura del fuoco
    int min_num_pixel_for_node;  //!< Minimo numero di pixel per un nodo nella mappa di disparit&agrave;
    int max_row_delta;  //!< Massimo delta tra le righe (di inizio e di fine) di una banda di disparit&agrave;
    int min_num_pixel_for_centroid;  //!< Minimo numero di pixel di un nodo per cui calcolare il centroide
    /*!
     * \brief tParametersCheck_
     * Costruttore della struttura con parametri di default
     */
    tParametersCheck_(){
        FOCUS_MEASURAMENT = 30.0f;
        min_num_pixel_for_node = 30;
        max_row_delta = 25;
        min_num_pixel_for_centroid = 400;
    }
}tParametersCheck;


/*!
 * Struttura per il settaggio dei parametri generali dell'applicazione
 */
typedef struct tParametersGeneral_{
    QString pathCalibrationDat;  //!< Path dei file di calibrazione
    QString pathReport;  //!< Path in cui verr&agrave; salvato il file di report
    /*!
     * \brief tParametersGeneral_
     * Costruttore per il settaggio dei parametri di default
     */
    tParametersGeneral_(){
        pathCalibrationDat = "C:/";
        pathReport = "C:/";
    }
}tParametersGeneral;


typedef struct tParameters_{
    tParametersGeneral paramGeneral; //!< Struttura relativa ai parametri generali dell'interfaccia
    tParametersCheck paramCheck;  //!< Struttura relatica ai parametri relativi al check della disparit&agrave;
}tParameters;

/*!
 * Struttura per il calcolo del grafo delle disparit&agrave;
 */
typedef struct tNode_{
    PixelType value;                              //!< Valore del nodo (livello di grigio)
    int label;                                    //!< Etichetta del nodo
    std::vector<int> positions;			  //!< Posizioni nelle immagini
    std::vector< std::pair<int,int> > neighbors;  //!< Archi pesati
    /*!
     * \brief tNode_
     * Costruttore della struttura tNode_
     */
    tNode_(){
        value = -1; //!< Valore identificativo
        }
    }tNode;

/*!
 * Struttura per contenere il risultato del test
 */
typedef struct tResultsCheckGraph_{
    std::string name;            //!< Nome del test
    bool response;               //!< Esito del test
    std::vector<int> positions;  //!< Vector contenente le poszioni (?)

    /*!
     * \brief tResultsCheckGraph_
     * Costruttore della classe tResultsCheckGraph_
     */
    tResultsCheckGraph_(){
        response = false;
        }
    }tResultsCheckGraph;

#endif
