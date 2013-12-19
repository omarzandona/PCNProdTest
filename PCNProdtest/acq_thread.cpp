/*!
\file acq_thread.cpp
\brief Crea e gestisce il thread di acquisizione delle immagini provenienti dal PCN

Questa funzione gestisce il thread di acquisizione delle immagini. Inoltre alloca e dealloca le strutture ncessarie all'acquisizione del
dataset e alla creazione delle immagine "modate" cioè ottenute come risultato dell'operazione di moda delle 50  immagini salvate

\author Alessio Montagnini, Omar Zandon&agrave (eVS - embedded Vision Systems s.r.l. www.embeddedvisionsystems.it)
*/

#include "acq_thread.h"

/*!
 * \brief AcqThread::AcqThread
 * Costruttore della  classe AcqThread: Inizializza la struttura dati e le variabili per la socket e per l'allocazione dei dataset
 */
AcqThread::AcqThread()
{
    qRegisterMetaType<tModeImages>("tModeImages");

    this->imgfd = 0;
    this->my_addr_data = 0;
    this->serv_addr_data = 0;
    this->serv_addr = 0;

    mutex.lock();
    stop = false;
    startProc = false;
    this->datasetSx = 0;
    this->datasetDx = 0;
    this->datasetDsp = 0;
    this->datasetAllocated = false;
    n_imgs = 0;
    mutex.unlock();

}

/*!
 * \brief AcqThread::setParameters
 * Setta i parametri della socket per l'acquisizione delle immagini
 * \param param [in] Struttura contenente i parametri di configurazione della socket
 */
void AcqThread::setParameters( tAcqThreadParam param ){
    this->imgfd = param.imgfd;
    this->my_addr_data = param.my_addr_data;
    this->serv_addr_data = param.serv_addr_data;
    this->serv_addr = param.serv_addr;

    mutex.lock();
    stop = false;
    mutex.unlock();
}
/*!
 * \brief AcqThread::run
 *  Attivata in caso di richiesta di visualizzazione delle immagini da parte dello scheduler fino al comando di stop dello stesso.
 *  Se riceve dati dalla socket alloca il dataset per l'acquisione e richiama la funzione
 *  \see update() per l'aggiornamento delle strutture dati che contengo le immagini
 */
void AcqThread::run(){

    int bytes;
    int addr_len = sizeof(struct sockaddr);

    mutex.lock();
    while(!stop)
    {
        mutex.unlock();
        memset(buffer,0,BUFFERSIZE);
        bytes = recvfrom(*imgfd,(char *)buffer,BUFFERSIZE,0,(struct sockaddr *) serv_addr_data,&addr_len);

        if((serv_addr_data->sin_addr.s_addr == serv_addr->sin_addr.s_addr) && (bytes > 0))
        {
            this->allocationDatasets();
            this->update();
        }

        mutex.lock();
    }
    mutex.unlock();
}

/*!
 * \brief AcqThread::update
 * Decodifica il buffer ricevuto dal PCN e aggiorna le strutture dati delle immagini ImageSX, ImageDX e ImageDSP.
 * Visualizza successivamente le immagini nella GUI.
 * Se il processing è partito salva nella struttura dati allocato le immagini e al raggiungimento di  #NUM_FRAME_TO_PROCESS
 * inizia il processing (calcolo della moda).
 *
 */
void AcqThread::update(){


    unsigned char header,ID;
    PixelType *ptr;

    ptr = buffer;
    header = *ptr;
    ptr += 4;
    // Immagine di sinistra
    memset(ImageSX,0,NN);
    if(header & 0x01)
    {
        ID = *ptr;
        ptr += 4;

        if(ID == 0x01)
            memcpy(ImageSX,ptr,NN);

        ptr += NN;
    }
    // Immagine di destra
    memset(ImageDX,0,NN);
    if(header & 0x02)
    {
        ID = *ptr;
        ptr += 4;

        if(ID == 0x02)
            memcpy(ImageDX,ptr,NN);

        ptr += NN;
    }
    // Immagine di disparità
    memset(ImageDSP,0,NN);
    if(header & 0x04)
    {
        ID = *ptr;
        ptr += 4;
        if(ID == 0x04)
            memcpy(ImageDSP,ptr,NN);

        ptr += NN;
    }

    // Salvo le immagini se è arrivato il comando
    mutex.lock();
    if ( startProc && datasetAllocated ){
        test_all( memcpy( datasetSx[n_imgs], ImageSX, NN ) );
        test_all( memcpy( datasetDx[n_imgs], ImageDX, NN ) );
        test_all( memcpy( datasetDsp[n_imgs], ImageDSP, NN ) );
        n_imgs++;
        emit imageNumberProgress( n_imgs );
        if ( n_imgs == NUM_FRAME_TO_PROCESS ){

            //futureThread = QtConcurrent::run( this, &AcqThread::calcModeThread);
            emit imagesMode( Utils::getModeImgs(datasetSx, datasetDx, datasetDsp) );

            // Dealloco i datasets e fermo la memorizzazione
            mutex.unlock();
            this->deallocationDatasets();
            mutex.lock();
            startProc = false;
        }
    }
    mutex.unlock();

    // Visualizzza le immagini nella GUI con il segnale imageSX
    emit imageSx( Utils::pixeltype2qimage(ImageSX) );
    emit imageDx( Utils::pixeltype2qimage(ImageDX) );
    emit imageDsp( Utils::pixeltype2qimage(ImageDSP), false );
}
/*!
 * \brief AcqThread::allocationDatasets
 * Alloca il dataset per le immagini di sinistra, destra e di disparità.
 * Le immagini salvate verrano utilizzate per il calcolo della moda.
 *
 */
void AcqThread::allocationDatasets(){
    // Inizializzazione variabili per processing
    mutex.lock();
    if ( startProc && !datasetAllocated ){

        this->datasetSx = (PixelType**) calloc(NUM_FRAME_TO_PROCESS, sizeof(PixelType*) );
        test_all( datasetSx );
        this->datasetDx = (PixelType**) calloc(NUM_FRAME_TO_PROCESS, sizeof(PixelType*) );
        test_all( datasetDx );
        this->datasetDsp = (PixelType**) calloc(NUM_FRAME_TO_PROCESS, sizeof(PixelType*) );
        test_all( datasetDsp );
        for ( int i = 0; i<NUM_FRAME_TO_PROCESS; ++i ){
            datasetSx[i] = (PixelType*) calloc(NN, sizeof(PixelType));
            test_all( datasetSx[i] );
            datasetDx[i] = (PixelType*) calloc(NN, sizeof(PixelType));
            test_all( datasetDx[i] );
            datasetDsp[i] = (PixelType*) calloc(NN, sizeof(PixelType));
            test_all( datasetDsp[i] );
        }
        datasetAllocated = true;
    }
    mutex.unlock();
}

void AcqThread::deallocationDatasets(){
    // Inizializzazione variabili per processing
    mutex.lock();
    if ( datasetAllocated ){
        for ( int i = 0; i<NUM_FRAME_TO_PROCESS; ++i ){
            free(datasetSx[i]);
            free(datasetDx[i]);
            free(datasetDsp[i]);
        }
        free(datasetSx);
        free(datasetDx);
        free(datasetDsp);
        n_imgs = 0;
        datasetAllocated = false;
    }
    mutex.unlock();
}

void AcqThread::startProcessing(){

    mutex.lock();
    startProc = true;
    mutex.unlock();
}

void AcqThread::abort(){
    mutex.lock();
    stop = true;
    mutex.unlock();
}


void AcqThread::abortAcquisition(){
    mutex.lock();
    startProc = false;
    mutex.unlock();
    deallocationDatasets();
}
