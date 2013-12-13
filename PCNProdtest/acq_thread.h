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
    AcqThread();

signals:
    //Per visualizzazione immagini
    void imageSx( const QImage& image );
    void imageDx( const QImage& image );
    void imageDsp( const QImage& image, bool external );

    // Per Processing
    void imageNumberProgress( int i );
    void imagesMode( tModeImages imgsToSend );

public slots:
    void setParameters( tAcqThreadParam param );    //!< Setting dei parametri prima del run()
    void abort();               //!<
    void startProcessing();     //!< Inizia il processing
    void abortAcquisition();    //!< Ferma l'acquisizione
    void update();  // Decodifico i dati e li invio come immagini al metodo mainWindow

protected:
    void run();


private:

    // Socket di ricezione immagini
    SOCKET *imgfd;
    struct sockaddr_in *my_addr_data;
    struct sockaddr_in *serv_addr_data;
    struct sockaddr_in *serv_addr;
    
    // Struttura dati per le immagini
    PixelType buffer[BUFFERSIZE];
    PixelType ImageSX[NN],ImageDX[NN],ImageDSP[NN];

    QMutex mutex;    //!< Per gestione fermata
    bool stop;

    // Allocazione e deallocazione dei datasets per il calcolo della moda
    void allocationDatasets();
    void deallocationDatasets();
    // Variabili acquisizione n frame e processing
    bool startProc;          //!< Per far partire acquisizione e processing
    bool datasetAllocated;   //!< Indica se i datasets sono stati allocati oppure no
    int n_imgs;              //!< Indica quanti campioni ho preso
    PixelType **datasetSx;   //!< N immagini da processare (buffer per calcolo delle mode)
    PixelType **datasetDx;
    PixelType **datasetDsp;

};

#endif // ACQ_THREAD_H
