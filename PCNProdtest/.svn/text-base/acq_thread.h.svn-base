/*!
 *\file Acq_thread.h
 *
 * File di header per la definizione del thread per l' acquisizione delle immagini provenienti dal PCN
 *
 * Vengono definite la classe AcqThread() e le relative funzioni, i segnali, gli slot e le variabili per la gestione del thread di acquisizione
 *
 *\author Alessio Montagnini, Omar Zandon&agrave; (eVS - embedded Vision Systems s.r.l. www.embeddedvisionsystems.it)
*/
#ifndef ACQ_THREAD_H
#define ACQ_THREAD_H
#define NOMINMAX
#include "define_file.h"
#include <QThread>
#include <QImage>
#include <QMutex>
#include <QMessageBox>
#include <QtCore>
#include <QFuture>
#include "structures.h"
#include "utils.h"


class AcqThread : public QThread
{
    Q_OBJECT
public:
    AcqThread();  //!< Costruttore vuoto della classe

signals:
    //Per visualizzazione immagini
    void imageSx( const QImage& image );  //!< Segnale per la visualizzazione dell'immagine di sinistra
    void imageDx( const QImage& image );  //!< Segnale per la visualizzazione dell'immagine di destra
    void imageDsp( const QImage& image, bool external );  //!< Segnale per la visualizzazione dell'immagine di disparit&agrave;

    // Per Processing
    void imageNumberProgress( int i );  //!<
    void imagesMode( tModeImages imgsToSend ); //!< Segnale per calcolo della moda

public slots:
    void setParameters( tAcqThreadParam param );    //!< Setting dei parametri prima del run()
    void abort();               //!< Annulla l'acquizione
    void startProcessing();     //!< Inizia il processing
    void abortAcquisition();    //!< Ferma l'acquisizione
    void update();  //!< Decodifica i dati e li invio come immagini al metodo mainWindow

protected:
    void run();  //!< Implementa la fase di esecuzione del thread


private:

    SOCKET *imgfd;  //!< File descriptor della socket di ricezione immagini
    struct sockaddr_in *my_addr_data; //!< Struttura necessarie per la definizione della socket
    struct sockaddr_in *serv_addr_data;  //!< Struttura necessarie per la definizione della socket
    struct sockaddr_in *serv_addr;  //!< Struttura necessarie per la definizione della socket
    
    // Struttura dati per le immagini
    PixelType buffer[BUFFERSIZE];  //!< Buffer di dimensione @BUFFERSIZE per contenere il file proveniente dal PCN
    PixelType ImageSX[NN];         //!< Buffer che contiene l'immagine di sinistra
    PixelType ImageDX[NN];         //!< Buffer che contiene l'immagine di destra
    PixelType ImageDSP[NN];        //!< Buffer che contiene l'immagine di disparti&agrave;
    QMutex mutex;                  //!< Mutex per gestione fermata
    bool stop;                     //!< Indica lo stop per l'acqizione delle immagini

    // Allocazione e deallocazione dei datasets per il calcolo della moda
    void allocationDatasets();     //!< Alloca il datasets per l'acquizione delle immagini
    void deallocationDatasets();   //!< Dealloca il dataset se precedentemente allocato
    // Variabili acquisizione n frame e processing
    bool startProc;          //!< Per far partire acquisizione e processing
    bool datasetAllocated;   //!< Indica se i datasets sono stati allocati oppure no
    int n_imgs;              //!< Indica quanti campioni ho preso
    PixelType **datasetSx;   //!< N immagini di sinistra da processare (buffer per calcolo delle mode)
    PixelType **datasetDx;   //!< N immagini di destra da processare (buffer per calcolo delle mode)
    PixelType **datasetDsp;  //!< N immagini di disparit&agrave; da processare (buffer per calcolo delle mode)

};

#endif // ACQ_THREAD_H
