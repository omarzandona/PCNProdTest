#include "acq_thread.h"

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

void AcqThread::setParameters( tAcqThreadParam param ){
    this->imgfd = param.imgfd;
    this->my_addr_data = param.my_addr_data;
    this->serv_addr_data = param.serv_addr_data;
    this->serv_addr = param.serv_addr;

    mutex.lock();
    stop = false;
    mutex.unlock();
}

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



void AcqThread::update(){


    unsigned char header,ID;
    PixelType *ptr;

    ptr = buffer;
    header = *ptr;
    ptr += 4;

    memset(ImageSX,0,NN);
    if(header & 0x01)
    {
        ID = *ptr;
        ptr += 4;

        if(ID == 0x01)
            memcpy(ImageSX,ptr,NN);

        ptr += NN;
    }

    memset(ImageDX,0,NN);
    if(header & 0x02)
    {
        ID = *ptr;
        ptr += 4;

        if(ID == 0x02)
            memcpy(ImageDX,ptr,NN);

        ptr += NN;
    }


    memset(ImageDSP,0,NN);
    if(header & 0x04)
    {
        ID = *ptr;
        ptr += 4;
        if(ID == 0x04)
            memcpy(ImageDSP,ptr,NN);

        ptr += NN;
    }
    mutex.lock();

    if ( startProc && datasetAllocated ){
        test_all( memcpy( datasetSx[n_imgs], ImageSX, NN ) );
        test_all( memcpy( datasetDx[n_imgs], ImageDX, NN ) );
        test_all( memcpy( datasetDsp[n_imgs], ImageDSP, NN ) );
        n_imgs++;
        //qDebug() << " immagine: " << n_imgs;
        emit imageNumberProgress( n_imgs );
        //qDebug() << n_imgs;
        if ( n_imgs == NUM_FRAME_TO_PROCESS ){

            //futureThread = QtConcurrent::run( this, &AcqThread::calcModeThread);
            emit imagesMode( Utils::getModeImgs(datasetSx, datasetDx, datasetDsp) );

            // Dealloco i datasets e fermo la memorizzazione
            mutex.unlock();
            this->deallocationDatasets();
            mutex.lock();
            //qDebug() << " ho terminato l'acquisizione";
            startProc = false;
        }
    }
    mutex.unlock();

    emit imageSx( Utils::pixeltype2qimage(ImageSX) );
    emit imageDx( Utils::pixeltype2qimage(ImageDX) );
    emit imageDsp( Utils::pixeltype2qimage(ImageDSP), false );


}

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
    //qDebug() << " Start del processing ";
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
