/*!
 *\file mainwindow.cpp
 *\brief Sorgente principale dell'applicazione PCNProdTest.
 *
 * Contiene la definizione delle funzioni, delle variabile, dei segnali e delle socket per la gestione del test in produzione.
 * In particolare, è presenta la gestione della comunicazione via socket con lo scheduler e la comunicazione socket con il PCN.
 * Si tratta del file principale del progetto dove risiede il main all'interno del quale viene fatto quanto segue:
 * - Inizializzazione interfaccia e variabili per la comunicazioni socket.
 * - Caricamento parametri di setting, per il salvataggio del report
 * - Gestione degli eventi:
 *    - Visualizzazione informazioni sul Log e cattura degli errori
 *    - Visualizzazione delle immagini provenienti dal PCN
 *    - Modifica/Salvataggio dei parametri di setting dell'applicativo
 *    - Salvataggio del report
 * - Gestione dei comandi socket Scheduler to PCNProdTest e PCNProdTest to PCN
 *
 *Per ulteriori dettagli vedere la doumentazione delle singole funzioni
 *
 *\author Alessio Montagnini, Omar Zandon&agrave (eVS - embedded Vision Systems s.r.l. www.embeddedvisionsystems.it)
 *
 *\mainpage
 *Questo documento ipertestuale descrive il codice del software PCNProdTest.
 *
 * Questo applicativo nasce dall'esigenza di Eurotech di adottare in fase di produzione del PCN
 * un protocollo di test adeguato.
 * Per fare ciò si è deciso di non adottare più il precedente strumento di collaudo ( un antico Win_Client ad hoc)
 * di creare un nuovo strumento che permetta, interfacciandosi sia con il PCN e con uno scheduler(fornito da Eurotech) che ne
 * lancia i comandi ad collaudare il dispositivo.
 * L'applicativo quindi gestisce tre comunicazioni socket, una svolgendo il ruolo di lato server con lo scheduler,
 * e le altre due svolgendo il ruolo di  client con il PCN (si comporta come un client comandi e immmagini)
 *
 * Per quanto riguarda il lato server qui descritto, possiamo riassumerne
 * le caratteristiche andando ad illustrare brevemente il contenuto dei vari file:
 * - mainwindow.cpp : Sorgente principale, contiene  e le funzioni per la gestione delle comunicazione socket e dell'interfaccia
 * - acq_thread.cpp : Definizione della classe per la gestione del thread di acquisione delle immagini proveniente dal PCN
 * - main.cpp : Lancia il mainwindow
 * - parameters_panel.cpp : Gestisce la configurazione dei parametri per il test
 * - socket_utils.cpp : contiene la definizioni delle funzioni per la comunicazione socket
 * - utils.cpp : Contiene funzioni di utilità di tipo generale
 * - check_pcn.cpp : Contiene le definizione delle funzioni per il check della mappa di disparità e del fuoco delle immagini ottiche
 *                 più altre funzioni non utilizzate ma create in fase di studio di fattibilità
 * - GraphDisparity.cpp : Contiene la definizione della classe per il check della disparità
 *
 *  <b>Note per la consultazione</b><br>
 *   Per meglio consultare questa documentazione si tenga presente che:
 *    - Si puo' sfruttare il motore di ricerca in alto a destra.
 *    - I commenti dei singoli sottoprogrammi sono completati da due voci "Riferimenti/References" e "Referenziato
 *      da/Referenced by": la prima corrisponde a un elenco di tutte le variabili, funzioni e costanti usate
 *      all'interno della funzione in esame; la seconda corrisponde a un elenco di altri sottoprogrammi
 *      che richiamano al loro interno il sottoprogramma in esame. Data la grande quantita' di
 *      variabili globali e la scarsa modularita' del codice, questi due elenchi risultano
 *      molto comodi per poter, per esempio, "ricostruire la storia" di una variabile o capire l'uso di
 *      un particolare sottoprogramma in base a dove viene chiamato.
 *
\author Alessio Montagnini, Omar Zandon&agrave (eVS - embedded Vision Systems s.r.l. www.embeddedvisionsystems.it)
\note Per info e dettagli contattare Omar Zandonà (omar.zandona@evsys.it)
*/
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{

  ui->setupUi(this);

  /** Inizializzazioni **/
  stop_scheduler = false;
  fd_copy = -1;
  this->isConnect = false;
  isConnectScheduler = false;
  startView = false;
  ACQL = new  AcqThread();  // Inizializza il thread di acquisizione
  this->removeToolBar( this->ui->mainToolBar );
  this->_parametersPanel = new ParametersPanel(this);
  n_modeSaved = 1;

  /** Caricamento parametri di setting **/
  _param = loadParameters();
  setParameters( _param.paramCheck );
  GraphDisparity::setParameters( _param.paramCheck );

  /** Variabili per il salvataggio del report **/
  startMemLog = false;
  MemLog = "";
  dat_filename = "";

  /** Nome report **/
  name_report = "";

  this->setWindowIcon(QIcon(":/oxygen/WindowIcon/eurologo.png"));

  /** Gestione degli eventi **/
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
  connect(this->ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
  connect(this, SIGNAL(errorMessage(QString)), this, SLOT(captureError(QString)));
  connect(this->ACQL, SIGNAL(imageSx(const QImage&)), this, SLOT(displayImageLeft(const QImage&)));
  connect(this->ACQL, SIGNAL(imageDx(const QImage&)), this, SLOT(displayImageRight(const QImage&)));
  connect(this->ACQL, SIGNAL(imageDsp(const QImage&,bool)), this, SLOT(displayImageDsp(const QImage&)));
  connect(this->ACQL, SIGNAL(imagesMode(tModeImages)), this, SLOT(ImgsModeReceiver(tModeImages)));
  connect(this, SIGNAL(messageInfo(QString, QColor, bool)), this, SLOT(showInfo(QString, QColor, bool)));
  connect(this, SIGNAL(startViewSignal(const bool&, int)), this, SLOT(startViewSlot(const bool&, int)));
  connect(this->ui->actionSettings, SIGNAL(triggered()), this, SLOT(startParametersPanel()));
  connect(this->_parametersPanel, SIGNAL(sendParameters(tParameters)), this, SLOT(receiverParameters(tParameters)));
  connect(this, SIGNAL(saveReportSignal()), this, SLOT(SaveReportSlot()));
  connect(this, SIGNAL(addPermanentWidgets()), this, SLOT(slotAddPermanentWidgets()));
  connect(this, SIGNAL(signalClearPermanentWidget()), this, SLOT(clearPermanentWidget()));

  future = QtConcurrent::run( this, &MainWindow::createSocketScheduler);

}

/*!
 * \brief MainWindow::~MainWindow
 * Distruttore della classe MainWindow: elimina l'ui
 */

MainWindow::~MainWindow()
{
  delete ui;
}
/*!
 * \brief MainWindow::createSocketScheduler
 *
 *  Crea la socket per la comunicazione con lo scheduler e resta in attesa dei comandi per effettuare il test
 *\0eturn
 */
bool MainWindow::createSocketScheduler(){
  fd_set master;   			//master file descriptor list
  fd_set read_fds; 			//temp file descriptor list
  char buf[MAX_STR_LENGTH];
  int fdmax;
  int newfd;
  int sockfd = 0;
  int nbytes = 0;
  //char addresses[255][16];
  const char yes=1;
  int addrlen = sizeof(struct sockaddr);
  FD_ZERO(&master); //// This function initializes the file descriptor set to contain no file descriptors.
  FD_ZERO(&read_fds);

  WSAStartup(MAKEWORD(2,2),&scheduler);
  //u_long iMode = 0;   //0 bloccante, 1 non bloccante
  //ioctlsocket(sock_scheduler,FIONBIO,&iMode);
  struct timeval timeout;
  timeout.tv_sec =  3;
  sock_scheduler =  INVALID_SOCKET;
  sock_scheduler = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock_scheduler == SOCKET_ERROR)
    {
      emit messageInfo("socket failed with error: "+ QString::number(WSAGetLastError()), Qt::red);
      perror("socket");
      //emit messageInfo("Exit (socket)",Qt::red,true);
    }

  if (setsockopt(sock_scheduler, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1)
    {
      perror("setsockopt");
      emit messageInfo("Exit (socketopt)" ,Qt::red,true);
    }

  if (setsockopt(sock_scheduler, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout,sizeof(timeout)) == -1)
    {
      perror("setsockopt");
      emit messageInfo("Exit (socketopt)" ,Qt::red,true);
    }

  my_addr_data.sin_family = AF_INET;
  my_addr_data.sin_addr.s_addr = INADDR_ANY;
  my_addr_data.sin_port = htons(PORT);
  memset(&(my_addr_data.sin_zero), '\0', 8);

  if (bind(sock_scheduler, (struct sockaddr *)&my_addr_data, sizeof(my_addr_data)) == -1)
    {
      perror("bind");
      emit messageInfo("Exit (bind)" ,Qt::red,true);
    }
  if (listen(sock_scheduler, 10) == -1)
    {
      perror("listen");
      emit messageInfo("Exit (listen)" ,Qt::red,true);
    }

  FD_SET(sock_scheduler, &master);
  fdmax = sock_scheduler;

  emit messageInfo("Waiting scheduler connection...", Qt::blue);

  // Ciclo infinito per restare in ascolto dei comandi da parte dello scheduler
  for(;;)
    {

      read_fds = master;
      if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
        {
          //perror("select");
          emit messageInfo("Exit (select)" ,Qt::red,true);
          //exit(1);
        }

      mutex.lock();
      if ( stop_scheduler )
        {
          mutex.unlock();
          return true;
        }
      mutex.unlock();

      //qDebug() << fdmax;
      for(int i = 0; i <= fdmax; i++)
        {
          if (FD_ISSET(i, &read_fds))
            {

              if (i == sock_scheduler && !isConnectScheduler)		//new connection
                {
                  addrlen = sizeof(remoteaddr);
                  if ((newfd = accept(sock_scheduler, (struct sockaddr *)&remoteaddr,&addrlen)) == -1)
                    {
                      //perror("accept");
                    }
                  else
                    {

                      FD_SET(newfd, &master); // add to master set

                      if (newfd > fdmax)
                        fdmax = newfd;


                      emit messageInfo("", Qt::black, true);

                      // Memorizzazione del log
                      startMemLog = true;
                      mutex.lock();
                      MemLog.clear();
                      mutex.unlock();

                      emit messageInfo("Connection with the scheduler " + QString(inet_ntoa(remoteaddr.sin_addr)) +" "+ QString::number(newfd), Qt::blue);
                      sockfd = newfd;
                      isConnectScheduler = true;
                    }
                }
              else if(i != sock_scheduler && isConnectScheduler)
                {
                  if((nbytes = SocketUtils::RecvString(i, buf,sizeof(buf))) <= 0)
                    {

                      emit messageInfo("Disconnection with the scheduler", Qt::blue);
                      startMemLog = false;
                      mutex.lock();
                      MemLog.clear();
                      mutex.unlock();
                      //if (nbytes < 0) perror("recv");

                      //images_enabled = 0;
                      sockfd = 0;
                      closesocket(i);
                      this->ACQL->abortAcquisition();
                      this->ACQL->abort();
                      startView = false;
                      setDisconnect();
                      this->ui->label_imgL->clear();
                      this->ui->label_imgR->clear();
                      this->ui->label_imgDSP->clear();

                      // This function removes a file descriptor from the file descriptor set.
                      FD_CLR(i, &master);


                      isConnectScheduler = false;
                    }

                  else
                    {
                      if(Communication(i,buf) < 0)
                        printf("PCNProdTest: error executing %s command!\n",buf);
                    }
                }
            }
        } // end internal for


    } // end of for(;;)
  return true;
}

/*!
 * \brief MainWindow::createSocket
 *  Crea la socket per la connessione con il PCN. Implementa quindi il lato client della comunicazione
 *  Si collega al PCN con indirizzo IP passato come parametro
 * \param IP_ADRESS_PCN_
 * \return true se la connessione è andato a buon fine, false altrimenti
 */
bool MainWindow::createSocket(QString IP_ADRESS_PCN_){


  WSAStartup(MAKEWORD(2,2),&dat);

  imgfd = socket(AF_INET,SOCK_DGRAM,0);

  my_addr_data.sin_family = AF_INET;
  my_addr_data.sin_port = htons(UDP_PORT);
  my_addr_data.sin_addr.s_addr = INADDR_ANY;
  memset ((char *)&(my_addr_data.sin_zero),0,8);

  bind(imgfd,(struct sockaddr *)&my_addr_data,sizeof(struct sockaddr));

  sock = socket(AF_INET,SOCK_STREAM,0);
  memset ((char *)&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port   = htons(PORT);
  serv_addr.sin_addr.s_addr = inet_addr(IP_ADRESS_PCN_.toStdString().c_str());

  if( ::connect(sock,(struct sockaddr *)&serv_addr, sizeof(sockaddr)) == -1)
    {
      emit messageInfo("Unable to connect to " + IP_ADRESS_PCN_ , Qt::red);
      return false;
    }


  int view_modality = 2;
  unsigned short acq_mode = SocketUtils::getAcqMode( view_modality );
  SocketUtils::SendString(sock,"smode");
  SocketUtils::Send(sock,(char *)&acq_mode,sizeof(short));

  //Sleep(100);

  unsigned char val = 1;
  SocketUtils::SendString(sock,"start");
  SocketUtils::Send(sock,&val,sizeof(val));

  if(!ACQL->isRunning())
    {
      tAcqThreadParam param;
      param.imgfd = &this->imgfd;
      param.my_addr_data = &this->my_addr_data;
      param.serv_addr = &this->serv_addr;
      param.serv_addr_data = &this->serv_addr_data;

      ACQL->setParameters( param );
      ACQL->start();

    }

  // Quando un nuovo PCN viene connesso si cancellano le immagini precedenti, se ne esistono
  if ( this->imagesMode.imgDsp != 0 ){
      free(imagesMode.imgSx);
      free(imagesMode.imgDx);
      free(imagesMode.imgDsp);
      imagesMode.imgSx = 0;
      imagesMode.imgDx = 0;
      imagesMode.imgDsp = 0;
    }

  emit messageInfo("Connect to " + IP_ADRESS_PCN_ , Qt::blue);
  return true;
}

/*!
 * \brief MainWindow::captureError
 *        Cattura l'errore
 *
 * \param text  [in] Stringa da mostrare a video
 */

void MainWindow::captureError( QString text ){
  QMessageBox::critical(this, windowTitle(), text);
}

/*!
 * \brief MainWindow::showInfo
 * Mostra le informazioni passati come stringa nella textEdit
 *
 * \param text        [in] Stringa da stampare
 * \param color       [in] Colore della stringa
 * \param disableTime [in] Flag per la stampa dell'ora corrente
 */

void MainWindow::showInfo(QString text, QColor color, bool disableTime){

  // Stampa l'ora corrente se richiesto
  if ( !disableTime )
    text = QTime::currentTime().toString() + " - " + text;


  mutex.lock();
  if ( startMemLog ){
      MemLog += QString(text).remove(QRegExp("<[^>]*>")).remove(QRegExp("&[^;]*;")) + "\n";
    }
  mutex.unlock();

  // Scelta colore corretto
  if ( color == Qt::red )
    text = "<FONT COLOR=red>" + text + "</FONT>";
  else
    if ( color == Qt::blue )
      text = "<FONT COLOR=blue>" + text + "</FONT>";
    else
      if ( color == Qt::green )
        text = "<FONT COLOR=green>" + text + "</FONT>";
      else
        text = "<FONT COLOR=black>" + text + "</FONT>";


  // Stampa sulla textEdit
  if ( this->ui->textEdit_Log->toPlainText().compare("") == 0 )
    this->ui->textEdit_Log->setText( text );
  else
    this->ui->textEdit_Log->append( "\n" + text );

}

/*!
 * \brief MainWindow::clearInfo
 *
 * Pulisce la textEdit
 */
void MainWindow::clearInfo(){
  this->ui->textEdit_Log->clear();
}

/*!
 * \brief MainWindow::showAbout
 *
 * Mostra le informazioni del sistema
 */
void MainWindow::showAbout(){

  QMessageBox::about(this, _TITLE_WINDOW_, _TITLE_WINDOW_ " - Version " _VERSION_PROGRAM_
                     "\n\n\n\nGUI developed with Qt5\n\n");
  /*"Copyright 2013 eVS - embedded Vision Systems\n"
                       "All rights reserved.");*/
}

/*!
 * \brief MainWindow::setConnect
 *  Connette il PCNProdTest al PCN di indirizzo IP passato come parametro
 *
 * \param adressIP [in] Indirizzo IP del PCN a cui connettersi
 * \return true se la connessione ha buon fine, false altrimenti
 */
bool MainWindow::setConnect(QString adressIP){


  if ( this->createSocket( adressIP ) )
    {
      //this->setPermanentWidget();
      emit this->addPermanentWidgets();
      IP_ADRESS_PCN_connected = adressIP ;
      this->isConnect = true;
      return true;
    }
  this->isConnect = false;
  return false;
}

/*!
 * \brief MainWindow::setDisconnect
 *  Effettua la disconnessione dal PCN
 *
 * \return true se la disconnessione è andata a buon fine, false altrimenti
 */
bool MainWindow::setDisconnect(){
  if ( this->isConnect ){

      SocketUtils::SendString(sock,"disconnect");
      shutdown(sock,2);
      Sleep(100);

      if ( !(closesocket(imgfd) && closesocket(sock)) ){
          this->isConnect = false;
          messageInfo("Disconnect to " + IP_ADRESS_PCN_connected, Qt::blue);
          this->IP_ADRESS_PCN_connected = "";
          emit signalClearPermanentWidget();
          // Chiudo il thread di acquisizione
          ACQL->abort();
          while( ACQL->isRunning() );

          return true;
        }else{
          messageInfo("ERROR: Unable to disconnect to " + IP_ADRESS_PCN_connected , Qt::red);
          return false;
        }

    }else
    return true;
}

/*!
 * \brief MainWindow::displayImageLeft
 *  Mostra l'immagine di sinistra nell'apposita label
 *
 * \param image [in] Immagine di sinistra da visualizzare
 */
void MainWindow::displayImageLeft( const QImage& image ){
  if ( !startView )
    return;

  pixSx = QPixmap::fromImage(image);
  const QSize size = ui->label_imgL->size();
  this->ui->label_imgL->setPixmap( pixSx.scaled(size,Qt::KeepAspectRatio) );
}

/*!
 * \brief MainWindow::displayImageRight
 * Mostra l'immagine di destra nell'apposita Label
 *
 * \param image [in] Immagine di destra da visualizzare
 */
void MainWindow::displayImageRight( const QImage& image ){

  if ( !startView )
    return;

  pixDx = QPixmap::fromImage(image);
  const QSize size = ui->label_imgR->size();
  this->ui->label_imgR->setPixmap( pixDx.scaled(size,Qt::KeepAspectRatio) );
}

/*!
 * \brief MainWindow::displayImageDsp
 * Mostra l'immagine di destra nell'apposita Label
 *
 * \param image[in] Immagine di disparità da visualizzare
 */
void MainWindow::displayImageDsp( const QImage& image ){

  if ( !startView )
    return;

  pixDsp = QPixmap::fromImage(image);
  const QSize size = ui->label_imgDSP->size();
  this->ui->label_imgDSP->setPixmap( pixDsp.scaled(size,Qt::KeepAspectRatio) );
}

/*!
 * \brief MainWindow::startAcquisition
 *  Fa partire l'acquisizione dell'immagini richiamando la funzione startProcessing() del thread ACQL
 */
void MainWindow::startAcquisition(){

  //emit messageInfo("", Qt::black, true);
  emit messageInfo("Images acquisition...");
  this->ACQL->startProcessing();

}

/*!
 * \brief MainWindow::stopAcquisition
 * Stampa mediante messageInfo la stringa "Stop acquisition
 */
void MainWindow::stopAcquisition(){
  this->messageInfo("Stop acquisition");
}

/*!
 * \brief MainWindow::ImgsModeReceiver
 * \param dataset
 */
void MainWindow::ImgsModeReceiver( tModeImages dataset ){

  imagesMode = dataset;
  bool ret = (imagesMode.imgSx != 0 && imagesMode.imgDx != 0 && imagesMode.imgDsp != 0 );

  if(ret)
    {
      emit messageInfo( "Ready for check" );
      SocketUtils::SendString(this->fd_copy,"PASSED: calc mode\0");
    }else
    SocketUtils::SendString(this->fd_copy,"FAILED: calc mode\0");
  // Processing
  //this->showInfo( "*** End check ***" );
}

/*!
 * \brief MainWindow::closeEvent
 *  Chiude gli eventi aperti, cioè le socket sia con il PCN che con lo scheduler e salva i parametri di set-up
 */
void MainWindow::closeEvent( QCloseEvent * ){

  closesocket(this->sock);
  closesocket(this->imgfd);
  closesocket(this->sock_scheduler);
  mutex.lock();
  this->stop_scheduler = true;
  mutex.unlock();
  this->future.waitForFinished();
  WSACleanup();

  saveParameters(_param);
}
/*!
 * \brief MainWindow::LoadDat
 * Caricamento dei file di calibrazione del PCN contenuti in un file in formato .dat il cui nome &egrave specificato come parametro.
 * Le operazioni compiuti dalla funzione sono i seguenti:
 * 1)- Erase Flash
 * 2)- Write FPN
 * 3)- Write ODC
 * 4)- Write Steps
 * 5)- Restore Factory Setting
 * 6)- Reset Counter
 * 7)- Remove records file
 *
 * \param dat_filename [in] Nome del file .dat da caricare
 * \return true se il caricamento è andato a buon fine , else altrimenti
 */
bool MainWindow::LoadDat(QString dat_filename)
{
  emit messageInfo("Start write calibration params");
  QString file_name = this->_param.paramGeneral.pathCalibrationDat + "/" + dat_filename;

  this->name_report = dat_filename.split(".").at(0);

  FILE *fdat;
  char xx;
  char x;
  int res;

  unsigned char val;
  unsigned short Vref_L,Vref_R;

  short *ODCbuf = new short[NN*8];
  char LeftOffsetX[NY*NX];
  char LeftOffsetY[NY*NX];
  char LeftXs[NY*NX];
  char LeftYs[NY*NX];
  char RightOffsetX[NY*NX];
  char RightOffsetY[NY*NX];
  char RightXs[NY*NX];
  char RightYs[NY*NX];
  char FpnBkg[2*NY*NX];

  unsigned char stepseq225[NUM_STEP],stepseq240[NUM_STEP];

  fdat = fopen(file_name.toStdString().c_str(), "rb");
  if( fdat <= 0 ){
      emit messageInfo( "Error file does not exist", Qt::red );
      return false;
    }
  fseek(fdat, 0, SEEK_END);
  int dim = ftell(fdat);
  if((dim!=192032)&&(dim!=192036))
    {
      val = 1;
      SocketUtils::SendString(sock,"start");
      SocketUtils::Send(sock,&val,sizeof(val));
      fclose(fdat);
      return false;
    }

  rewind(fdat);

  for( int b=0; b<2*NX*NY; ++b ){
      res = fread( &xx, sizeof(char), 1, fdat );
      if ( res == -1 ){
          emit messageInfo("ERROR: read load dat", Qt::red);
          return false;
        }
      FpnBkg[b] = xx;
    }

  int ODCbufindex=0;

  for(int u=0;u<(NX*NY);u++)
    {
      bool ret(true);
      ret &= createStructFileDat(ODCbuf, LeftOffsetX, u, ODCbufindex, fdat, x) == 1;

      ret &= createStructFileDat(ODCbuf, RightOffsetX , u, ODCbufindex, fdat, x) == 1;

      ret &= createStructFileDat(ODCbuf, LeftOffsetY, u, ODCbufindex, fdat, x) == 1;

      ret &= createStructFileDat(ODCbuf, RightOffsetY, u, ODCbufindex, fdat, x) == 1;

      ret &= createStructFileDat(ODCbuf, LeftXs, u, ODCbufindex, fdat, x) == 1;

      ret &= createStructFileDat(ODCbuf, RightXs, u, ODCbufindex, fdat, x) == 1;

      ret &= createStructFileDat(ODCbuf, LeftYs, u, ODCbufindex, fdat, x) == 1;

      ret &= createStructFileDat(ODCbuf, RightYs, u, ODCbufindex, fdat, x) == 1;

      if(!ret)
        {
          emit messageInfo("ERROR: read ODC distorsion parameters " + QString::number(u), Qt::red);
          return false;
        }

    }

  for(int u=0;u<16;u++)
    {
      res=fread(&x, sizeof(char), 1, fdat);
      stepseq225[u]=x;
      if( res!=1 )
        {
          emit messageInfo("ERROR: -225", Qt::red);
          return false;
        }
    }
  for(int u=0;u<16;u++)
    {
      res=fread(&x, sizeof(char), 1, fdat);
      stepseq240[u]=x;
      if( res!=1 )
        if( res!=1 )
          {
            emit messageInfo("ERROR: -240", Qt::red);
            return false;
          }
    }

  if( dim == 192036 )
    {
      res=fread(&Vref_L, sizeof(unsigned short), 1, fdat);
      if( res!=1 )
        {
          emit messageInfo("ERROR: read vref left");
          return false;
        }
      res=fread(&Vref_R, sizeof(unsigned short), 1, fdat);
      if( res!=1 )
        {
          emit messageInfo("ERROR: read vref right", Qt::red);
          return false;
        }
      //WrFlashMemo->Lines->Add(IntToStr(Vref_L));
      //WrFlashMemo->Lines->Add(IntToStr(Vref_R));
    }
  else
    {
      //WrFlashMemo->Lines->Add("No Vref data in parameters file!!");
      emit messageInfo("ERROR: no Vref data in parameters file", Qt::red);
      fclose(fdat);
      return false;
    }

  fclose(fdat);




  val = 0;
  SocketUtils::SendString(sock,"start");
  SocketUtils::Send(sock,&val,sizeof(val));

  SocketUtils::SendString(sock,"erase");
  SocketUtils::Recv(sock,&val, sizeof(val));
  if(val==0xFF)
    {
      emit messageInfo("ERROR: to erase flash", Qt::red);
      return false;
    }
  emit messageInfo("&nbsp;&nbsp; (1/7) Erase Flash concluse");


  unsigned char pos=0;
  SocketUtils::SendString(sock,"upfpn");
  SocketUtils::Send(sock,(char *)&FpnBkg[0],sizeof(char)*(2*NN));
  do
    {
      SocketUtils::Recv(sock,(unsigned char *)&pos,sizeof(pos));
    }while(pos < 136);   // Numero della barra

  emit messageInfo("&nbsp;&nbsp; (2/7) Write FPN concluse");

  ////////////////////////////////////////////////////

  val=0x02;
  SocketUtils::SendString(sock,"setwrsel");
  SocketUtils::Send(sock,&val, sizeof(val));

  SocketUtils::SendString(sock,"odc");
  SocketUtils::Send(sock,(char *)&ODCbuf[0],sizeof(short)*NN*8);

  do
    {
      SocketUtils::Recv(sock,(unsigned char *)&pos,sizeof(pos));
    }
  while(pos < 136);
  val=0x03;
  SocketUtils::SendString(sock,"setwrsel");
  SocketUtils::Send(sock,&val, sizeof(val));
  emit messageInfo("&nbsp;&nbsp; (3/7) Write ODC concluse");
  /////////////////////////

  int ret;
  SocketUtils::SendString(sock,"steps225");
  SocketUtils::Send(sock,&stepseq225, sizeof(stepseq225));
  ret=SocketUtils::Recv(sock,&ret,sizeof(ret));

  SocketUtils::SendString(sock,"steps240");
  SocketUtils::Send(sock,&stepseq240, sizeof(stepseq240));
  ret=SocketUtils::Recv(sock,&ret,sizeof(ret));
  emit messageInfo("&nbsp;&nbsp; (4/7) Write Steps concluse");

  for(int i=0;i<NN;i++)
    {
      FpnBkg[i]=0;
      FpnBkg[i+NN]=0;
      LeftOffsetX[i]=0;
      LeftOffsetY[i]=0;
      LeftXs[i]=0;
      LeftYs[i]=0;
      RightOffsetX[i]=0;
      RightOffsetY[i]=0;
      RightXs[i]=0;
      RightYs[i]=0;
    }

  for(int i=0;i<(2*NN);i++)
    FpnBkg[i]=0;

  //emit messageInfo("Write Flash concluse");

  if(dim==192036)
    {
      SocketUtils::SendString(sock,"dac_vref2");
      SocketUtils::Send(sock,&Vref_L,sizeof(Vref_L));
      SocketUtils::SendString(sock,"dac_vref1");
      SocketUtils::Send(sock,&Vref_R,sizeof(Vref_R));
      //emit messageInfo("Write CMos Vref concluse");
    }

  val = 1;
  SocketUtils::SendString(sock,"start");
  SocketUtils::Send(sock,&val,sizeof(val));

  SocketUtils::SendString(sock,"restore");
  emit messageInfo("&nbsp;&nbsp; (5/7) Restore Factory Setting concluse");
  SocketUtils::SendString(sock,"reset");

  emit messageInfo("&nbsp;&nbsp; (6/7) Reset Counter concluse");
  SocketUtils::SendString(sock,"rddelete");
  emit messageInfo("&nbsp;&nbsp; (7/7) Remove records file concluse");
  emit messageInfo("End write calibration params");

  return true;

}

/*!
 * \brief MainWindow::createStructFileDat
 *  Funzione per creare senza duplicazione di codice il buffer dell'Optical Distorsion Correction
 * \param ODCBuff       [in|out] Buffer contenente i dati dell'ODC
 * \param offset        [in|out] Offset
 * \param index_offset  [in] Index
 * \param index_buffer  [in] Indice del buffer ODC su cui salvare
 * \param fd            [in] File descriptor del file
 * \param value         [in] Valore da salvare
 * \return
 */
int MainWindow::createStructFileDat( short* ODCBuff, char* offset, int&  index_offset, int& index_buffer, FILE* fd, char& value ){
  int res = fread(&value, sizeof(char), 1, fd);
  offset[index_offset] = value;
  ODCBuff[index_buffer] = (char)value;
  index_buffer++;

  return res;
}
/*!
 * \brief MainWindow::startViewSlot
 *  Fa partire la visualizzazione a video delle immagini di destra, sinistra e disparità
 * \param choose [in] Modalità di visualizzazione
 * \param fd     [in] File descriptor della socket
 */

void MainWindow::startViewSlot( const bool& choose, int fd ){

  this->startView = choose;

  // Pulisci le label
  if( !this->startView ){
      this->ui->label_imgL->clear();
      this->ui->label_imgR->clear();
      this->ui->label_imgDSP->clear();
    }

  if ( choose  )
    if(startView == choose && isConnect )
      {
        emit this->messageInfo("Start view");
        SocketUtils::SendString(fd,"PASSED: start view\0");
      }else
      SocketUtils::SendString(fd,"FAILED: start view\0");
  else
    if(startView == choose && isConnect){
        emit this->messageInfo("Stop view");
        SocketUtils::SendString(fd,"PASSED: stop view\0");
      }else
      SocketUtils::SendString(fd,"FAILED: stop view\0");
}
/*!
 * \brief MainWindow::CheckDisparity
 * Effettua il check della disparità utilizzando la classe graph
 * \see GraphDisparity
 * \return  true se il check è andato a buon fine, false altrimenti
 */
bool MainWindow::CheckDisparity(){
  if ( imagesMode.imgDsp == 0 )
    return false;

  GraphDisparity graph;	//!< Classe per il testing della disparit?
  graph.setParameters( _param.paramCheck );
  vector< tResultsCheckGraph > testDisp = graph.check(Utils::CropImage(imagesMode.imgDsp, W_CROP, H_CROP), W_CROP, H_CROP);
  bool ret(true);
  for( unsigned int i=0; i<testDisp.size(); ++i )
    ret = ret && testDisp.at(i).response;
  return ret;
}

/*!
 * \brief MainWindow::showResponseComputation
 *
 * Funzione per stampare nella textEdit il risultato dei check
 * \param response [in] Vector che contiene il risultato del check
 */
void MainWindow::showResponseComputation(std::vector<std::pair<QString, bool> > response){
  for ( unsigned int i=0; i<response.size(); ++i ){
      if ( response.at(i).second )
        emit messageInfo(response.at(i).first + "<FONT COLOR=green> PASSED </FONT>");
      else
        emit messageInfo(response.at(i).first + "<FONT COLOR=red> FAILED </FONT>");
    }
}

bool MainWindow::CheckFocus(){
  if ( this->imagesMode.imgSx == 0 || this->imagesMode.imgDx == 0 )
    return false;
  return check_focus(this->imagesMode.imgDx,this->imagesMode.imgSx, NX, NY);
}

void MainWindow::saveParameters(tParameters param){
  this->saveParameters(param, QCoreApplication::applicationDirPath() + "/settings.xml");
}

void MainWindow::saveParameters(const tParameters& param, QString fileName){

  QSettings::SettingsMap parameters;
  // General
  parameters.insert("PathCalibration", param.paramGeneral.pathCalibrationDat );
  parameters.insert("PathReport", param.paramGeneral.pathReport );
  //parameters.insert("SerialString", param.paramGeneral.serialString );
  // Advanced
  parameters.insert("FocusMeasurament", param.paramCheck.FOCUS_MEASURAMENT);
  parameters.insert("MaxRowDelta", param.paramCheck.max_row_delta);
  parameters.insert("MinNumPixelForCentroid", param.paramCheck.min_num_pixel_for_centroid);
  parameters.insert("MinNumPixelForNode", param.paramCheck.min_num_pixel_for_node);

  QFile file( fileName );
  if (file.open(QIODevice::WriteOnly))
    Utils::writeXmlFile(file, parameters);
}

tParameters MainWindow::loadParameters(){

  tParameters param;
  QSettings::SettingsMap parameters;
  QFile file(QCoreApplication::applicationDirPath() +"/settings.xml");
  if ( file.exists() ){
      if (file.open(QIODevice::ReadOnly))
        Utils::readXmlFile(file, parameters);
      // General
      param.paramGeneral.pathCalibrationDat = parameters.find("PathCalibration").value().toString();
      param.paramGeneral.pathReport = parameters.find("PathReport").value().toString();
      //param.paramGeneral.serialString = parameters.find("SerialString").value().toString();

      if ( param.paramGeneral.pathCalibrationDat.compare("true") == 0 )
        param.paramGeneral.pathCalibrationDat = "";
      if ( param.paramGeneral.pathReport.compare("true") == 0 )
        param.paramGeneral.pathReport = "";
      //if ( param.paramGeneral.serialString.compare("true") == 0 )
      //    param.paramGeneral.serialString = "";

      // Advanced
      param.paramCheck.FOCUS_MEASURAMENT = parameters.find("FocusMeasurament").value().toFloat();
      param.paramCheck.max_row_delta = parameters.find("MaxRowDelta").value().toInt();
      param.paramCheck.min_num_pixel_for_centroid = parameters.find("MinNumPixelForCentroid").value().toInt();
      param.paramCheck.min_num_pixel_for_node = parameters.find("MinNumPixelForNode").value().toInt();

      return param;
    }
  return param;
}

void MainWindow::startParametersPanel(){
  this->_parametersPanel->setParameters( this->_param );
  this->_parametersPanel->exec();
}

void MainWindow::receiverParameters(tParameters param){
  setParameters( param.paramCheck );
  this->_param = param;
  this->saveParameters( _param );
}

bool MainWindow::Communication(int fd,char *buffer)
{


  bool ret = false;
  u_long iMode = 0;   //0 bloccante, 1 non bloccante
  ioctlsocket(fd,FIONBIO,&iMode);

  /** Connessione  if(Utils::strcasecmp(buffer,"ConnectTo",9)==0)  if(strcmp(buffer,"ConnectTo")==0)*/
  if(Utils::strcasecmp(buffer,"ConnectTo",9)==0)
    {

      char address[16];
      memcpy(address,&buffer[10],16*sizeof(char));
      //SocketUtils::RecvString(fd, address, 50);
      QString ip_address = QString(address);
      ret = MainWindow::setConnect(ip_address);

      if(ret)
        SocketUtils::SendString(fd,"PASSED: connection\0");
      else
        SocketUtils::SendString(fd,"FAILED: connection\0");

      //if(!ret)
      // closesocket(fd); eVS modifica 12/12/2013
    }

  /** Disconnessione */
  if(strcmp(buffer,"Disconnect\0")==0)
    {
      if(isConnect)
        ret = setDisconnect();

      if(ret)
        SocketUtils::SendString(fd,"PASSED: disconnection\0");
      else
        SocketUtils::SendString(fd,"FAILED: disconnection\0");

    }

  /** StartView */
  if(strcmp(buffer,"StartView\0") == 0)
    emit startViewSignal(true, fd);

  /** StopView */
  if(strcmp(buffer,"StopView\0") == 0)
    emit startViewSignal(false, fd);

  /** Load Dat */
  if(Utils::strcasecmp(buffer,"LoadDat",7)==0)
    {

      // Riceve nome file seriale
      const int size_namefile = 50;
      char* namefile = (char*) calloc(size_namefile, sizeof(char));
      memcpy(namefile,&buffer[8],size_namefile*sizeof(char));
      //SocketUtils::RecvString(fd, (char *)namefile, sizeof(char)*size_namefile);

      QString dat_filename(namefile);

      if(isConnect)
        ret = LoadDat(dat_filename);

      if(ret)
        SocketUtils::SendString(fd,"PASSED: load file dat\0");
      else
        SocketUtils::SendString(fd,"FAILED: load file dat\0");
    }

  /** CalcMode */
  if(strcmp(buffer,"CalcMode\0")==0)
    {
      if ( !isConnect )
        {
          SocketUtils::SendString(fd,"FAILED: calc mode\0");
          emit messageInfo("Image acquisition failure: no connection!", Qt::red );
        }else{
          fd_copy = fd;
          startAcquisition();
        }
    }

  /** CheckDisparity */
  if(strcmp(buffer,"CheckDisparity\0")==0)
    {
      if ( !isConnect )
        {
          SocketUtils::SendString(fd,"FAILED: calc mode\0");
          emit messageInfo("Check disparity failure: no connection!", Qt::red );
        }else{
          ret = CheckDisparity();

          if(ret){
              emit messageInfo("Check Disparity: <FONT COLOR=green> PASSED </FONT>");
              SocketUtils::SendString(fd,"PASSED: check disparity\0");
            }else{
              emit messageInfo("Check Disparity: <FONT COLOR=red> FAILED </FONT>");
              SocketUtils::SendString(fd,"FAILED: check disparity\0");
            }
        }
    }

  /** CheckFocus */
  if(strcmp(buffer,"CheckFocus\0")==0)
    {
      if ( !isConnect )
        {
          SocketUtils::SendString(fd,"FAILED: calc mode\0");
          emit messageInfo("Check focus failure: no connection!", Qt::red );
        }else{
          ret = CheckFocus();
          if(ret){
              emit messageInfo("Check Focus: <FONT COLOR=green> PASSED </FONT>");
              SocketUtils::SendString(fd,"PASSED: check focus\0");
            }else{
              emit messageInfo("Check Focus: <FONT COLOR=red> FAILED </FONT>");
              SocketUtils::SendString(fd,"FAILED: check focus\0");
            }
        }
    }

  /** SaveReport */
  if(strcmp(buffer,"SaveReport\0")==0)
    {

      fd_copy = fd;
      emit saveReportSignal();
    }


  if(Utils::strcasecmp(buffer,"Set_IN_1",8)==0)
    {
      if(isConnect)
        {
#ifdef SET_DIGITAL_IO_WITH_PARMS
          unsigned short val_test = BuildValueToSet(buffer,9);
          in1_current_val = val_test;
          ret =  this->setIN1(val_test);
#else
          ret =  this->setIN1();
#endif
        }

      if(ret)
        {
          emit messageInfo("Check set port IN 1: <FONT COLOR=green> PASSED </FONT>");
          SocketUtils::SendString(fd,"PASSED: set IN_1\0");
        }else{
          emit messageInfo("Check set port IN 1: <FONT COLOR=red> FAILED </FONT>");
          SocketUtils::SendString(fd,"FAILED: set IN_1\0");
        }
    }



  if(Utils::strcasecmp(buffer,"Set_IN_2",8)==0)
    {
      if(isConnect)
        {
#ifdef SET_DIGITAL_IO_WITH_PARMS
          unsigned short val_test = BuildValueToSet(buffer,9);
          in2_current_val = val_test;
          ret =  this->setIN2(val_test);
#else
          ret =  this->setIN2();
#endif
        }

      if(ret){
          emit messageInfo("Check set port IN 2: <FONT COLOR=green> PASSED </FONT>");
          SocketUtils::SendString(fd,"PASSED: set IN_2\0");
        }else{
          emit messageInfo("Check set port IN 2: <FONT COLOR=red> FAILED </FONT>");
          SocketUtils::SendString(fd,"FAILED: set IN_2\0");}
    }

  if(Utils::strcasecmp(buffer,"Set_OUT_1\0",9)==0)
    {
      if(isConnect)
        {
#ifdef SET_DIGITAL_IO_WITH_PARMS
          unsigned short val_test = BuildValueToSet(buffer,10);
          out1_current_val = val_test;
          ret =  this->setOUT1(val_test);
#else
          ret =  this->setOUT1();
#endif
        }
      if(ret){
          emit messageInfo("Check set port OUT 1: <FONT COLOR=green> PASSED </FONT>");
          SocketUtils::SendString(fd,"PASSED: set OUT_1\0");
        }else{
          emit messageInfo("Check set port OUT 1: <FONT COLOR=red> FAILED </FONT>");
          SocketUtils::SendString(fd,"FAILED: set OUT_1\0");}
    }

  if(Utils::strcasecmp(buffer,"Set_OUT_2\0",9)==0)
    {
      if(isConnect)
        {
#ifdef SET_DIGITAL_IO_WITH_PARMS
          unsigned short val_test = BuildValueToSet(buffer,10);
          out2_current_val = val_test;
          ret =  this->setOUT2(val_test);
#else
          ret =  this->setOUT2();
#endif
        }

      if(ret ){
          emit messageInfo("Check set port OUT 2: <FONT COLOR=green> PASSED </FONT>");
          SocketUtils::SendString(fd,"PASSED: set OUT_2\0");
        }else{
          emit messageInfo("Check set port OUT 2: <FONT COLOR=red> FAILED </FONT>");
          SocketUtils::SendString(fd,"FAILED: set OUT_2\0");}
    }


  if(strcmp(buffer,"Get_IN_1\0")==0)
    {
      if(isConnect)
        ret =  this->getIN1();

      if(ret){
          emit messageInfo("Check get port IN 1: <FONT COLOR=green> PASSED </FONT>");
          SocketUtils::SendString(fd,"PASSED: get IN_1\0");
        }else{
          emit messageInfo("Check get port IN 1: <FONT COLOR=red> FAILED </FONT>");
          SocketUtils::SendString(fd,"FAILED: get IN_1\0");}
    }

  if(strcmp(buffer,"Get_IN_2\0")==0)
    {
      if(isConnect)
        ret =  this->getIN2();

      if(ret ){
          emit messageInfo("Check get port IN 2: <FONT COLOR=green> PASSED </FONT>");
          SocketUtils::SendString(fd,"PASSED: get IN_2\0");
        }else{
          emit messageInfo("Check get port IN 2: <FONT COLOR=red> FAILED </FONT>");
          SocketUtils::SendString(fd,"FAILED: get IN_2\0");}
    }

  if(strcmp(buffer,"Get_OUT_1\0")==0)
    {
      if(isConnect)
        ret =  this->getOUT1();

      if(ret){
          emit messageInfo("Check get port OUT 1: <FONT COLOR=green> PASSED </FONT>");
          SocketUtils::SendString(fd,"PASSED: get OUT_1\0");
        }else{
          emit messageInfo("Check get port OUT 1: <FONT COLOR=red> FAILED </FONT>");
          SocketUtils::SendString(fd,"FAILED: get OUT_1\0");}
    }

  if(strcmp(buffer,"Get_OUT_2\0")==0)
    {
      if(isConnect)
        ret =  this->getOUT2();

      if(ret){
          emit messageInfo("Check get port OUT 2: <FONT COLOR=green> PASSED </FONT>");
          SocketUtils::SendString(fd,"PASSED: get OUT_2\0");
        }else{
          emit messageInfo("Check get port OUT 2: <FONT COLOR=red> FAILED </FONT>");
          SocketUtils::SendString(fd,"FAILED: get OUT_2\0");}
    }
  if(strcmp(buffer,"TestSerialLoop\0")==0)
    {
      if(isConnect)
        ret =  this->TestSerialLoop();

      if(ret ){
          emit messageInfo("Check TestSerialLoop: <FONT COLOR=green> PASSED </FONT>");
          SocketUtils::SendString(fd,"PASSED: TestSerialLoop\0");
        }else{
          emit messageInfo("Check TestSerialLoop: <FONT COLOR=red> FAILED </FONT>");
          SocketUtils::SendString(fd,"FAILED: TestSerialLoop\0");}
    }

  if(Utils::strcasecmp(buffer,"TestSerial ",11)==0)
    {
      char* serial_port = BuildSerialPortAddress(buffer,11);
      ret =  this->TestSerial(serial_port);
      if(ret ){
          emit messageInfo("Check TestSerial port : <FONT COLOR=green> PASSED </FONT>");
          SocketUtils::SendString(fd,"PASSED: test serial\0");
        }else{
          emit messageInfo("Check TestSerial port : <FONT COLOR=red> FAILED </FONT>");
          SocketUtils::SendString(fd,"FAILED: test serial\0");

          free(serial_port);
        }
    }

  if(strcmp(buffer,"Restore\0")==0)
    {
      if(isConnect)
        ret =  this->restore();

      if(ret ){
          emit messageInfo("Restore PCN");
          SocketUtils::SendString(fd,"PASSED: Restore\0");
        }else{
          emit messageInfo("Restore PCN <FONT COLOR=red> FAILED </FONT>");
          SocketUtils::SendString(fd,"FAILED: Restore\0");
        }
    }

  /** Test */
  if(strcmp(buffer,"Test\0")==0)
    {
      emit messageInfo("Test ricezione comando");
      SocketUtils::SendString(fd,"PASSED messaggio ricevuto\0");
    }


  return ret;
}

bool MainWindow::SaveReport(){

  //  if(!isConnect)
  //    {
  //      emit messageInfo( "Error: connection are missing or lost! No report has been created", Qt::red );
  //      return false;
  //  }

  QString dir = this->_param.paramGeneral.pathReport;
  if (!QFile(dir).exists())
    {
      emit messageInfo( "Error: the save path does not exist", Qt::red );
      return false;
    }

  if ( name_report.isEmpty() )
    {
      dir = dir + (QDir::separator() + QString::number(n_modeSaved) + QString("_PCN"));
      n_modeSaved++;
    }else{
      dir = dir + (QDir::separator() + name_report + QString("_PCN") );
    }

  QDir directory;
  if (!directory.mkdir( dir ))
    {
      int res = QMessageBox::warning(this, "PCNProdTest", QFileInfo(dir).fileName() + " is already exists. You want to overwrite it?", QMessageBox::Yes, QMessageBox::No);
      if ( res == QMessageBox::No )
        return false;
      else
        {
          if (!Utils::removeDir( dir ) || !directory.mkdir( dir ))
            {
              emit messageInfo( "Error: save report", Qt::red );
              return false;
            }
        }
    }

  if ( this->imagesMode.imgSx != 0 ){
      QFile fileSx(dir + "/imgSx.png");
      fileSx.open(QIODevice::WriteOnly);
      QPixmap::fromImage( Utils::pixeltype2qimage(this->imagesMode.imgSx) ).save(&fileSx, "PNG");
    }
  if ( this->imagesMode.imgDx != 0 ){
      QFile fileDx(dir + "/imgDx.png");
      fileDx.open(QIODevice::WriteOnly);
      QPixmap::fromImage( Utils::pixeltype2qimage(this->imagesMode.imgDx) ).save(&fileDx, "PNG");
    }
  if ( this->imagesMode.imgDsp != 0 ){
      QFile fileDsp(dir + "/imgDsp.png");
      fileDsp.open(QIODevice::WriteOnly);
      QPixmap::fromImage( Utils::pixeltype2qimage(this->imagesMode.imgDsp) ).save(&fileDsp, "PNG");
    }


  QFile file(dir + "/report.txt");
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream out(&file);
  //out << "This file is a report\n";
  out << "**** PCN Check Report ****\n\n";
  out << "Date: " << QDate::currentDate().toString("dd.MM.yyyy") << "\n";
  out << "Time: " << QTime::currentTime().toString("hh:mm:ss") << "\n\n";
  out << "Log:\n";
  mutex.lock();
  out << MemLog;
  mutex.unlock();
  /*for ( uint i=0; i<response.size(); ++i ){
        out << response.at(i).first;
        if ( response.at(i).second == 1 )
            out << "TRUE\n";
        else
            out << "FALSE\n";
    }*/
  file.close();

  saveParameters(_param, dir + "/settings.xml");

  emit messageInfo("Report saved");
  return true;

}

void MainWindow::SaveReportSlot(){
  bool ret = SaveReport();
  if(ret)
    SocketUtils::SendString(fd_copy,"PASSED: save report");
  else
    SocketUtils::SendString(fd_copy,"FAILED: save report");
}

#ifdef SET_DIGITAL_IO_WITH_PARMS
bool MainWindow::setIN1( unsigned short val_test){
  SocketUtils::SendString(sock,"input0");
  SocketUtils::Send(sock,&val_test,sizeof(val_test));
  return true;
}
bool MainWindow::setIN2( unsigned short val_test){
  SocketUtils::SendString(sock,"input1");
  SocketUtils::Send(sock,&val_test,sizeof(val_test));
  return true;
}
bool MainWindow::setOUT1(unsigned short val_test){
  SocketUtils::SendString(sock,"outtime0");
  SocketUtils::Send(sock,&val_test,sizeof(val_test));
  return true;
}
bool MainWindow::setOUT2( unsigned short val_test){
  SocketUtils::SendString(sock,"outtime1");
  SocketUtils::Send(sock,&val_test,sizeof(val_test));
  return true;
}
bool MainWindow::getIN1(){
  unsigned char val = 0;
  SocketUtils::SendString(sock,"testin0");
  SocketUtils::Recv(sock,&val,sizeof(val));

  if(val ==  in1_current_val)
    return true;
  else
    return false;
}

bool MainWindow::getIN2(){
  unsigned char val = 0;
  SocketUtils::SendString(sock,"testin1");
  SocketUtils::Recv(sock,&val,sizeof(val));

  if(val ==  in2_current_val)
    return true;
  else
    return false;
}

bool MainWindow::getOUT1(){
  unsigned short parameters[256];
  SocketUtils::SendString(sock,"gparms");
  SocketUtils::Recv(sock,parameters,sizeof(parameters));

  if ( parameters[0] == out1_current_val )
    return true;
  else
    return false;
}

bool MainWindow::getOUT2(){
  unsigned short parameters[256];
  SocketUtils::SendString(sock,"gparms");
  SocketUtils::Recv(sock,parameters,sizeof(parameters));
  if ( parameters[1] == out2_current_val )
    return true;
  else
    return false;
}
#else
bool MainWindow::setIN1(){
  unsigned short val_test = 0;
  SocketUtils::SendString(sock,"input0");
  SocketUtils::Send(sock,&val_test,sizeof(val_test));
  return true;
}
bool MainWindow::setIN2(){
  unsigned short val_test = 200;
  SocketUtils::SendString(sock,"input1");
  SocketUtils::Send(sock,&val_test,sizeof(val_test));
  return true;
}
bool MainWindow::setOUT1(){

  unsigned short val_test = 200;
  SocketUtils::SendString(sock,"outtime0");
  SocketUtils::Send(sock,&val_test,sizeof(val_test));
  return true;
}
bool MainWindow::setOUT2(){
  unsigned short val_test = 200;
  SocketUtils::SendString(sock,"outtime1");
  SocketUtils::Send(sock,&val_test,sizeof(val_test));
  return true;
}
bool MainWindow::getIN1(){
  unsigned char val = 0;
  SocketUtils::SendString(sock,"testin0");
  SocketUtils::Recv(sock,&val,sizeof(val));
  if(val == 0)
    return true;
  else
    return false;
}

bool MainWindow::getIN2(){
  unsigned char val = 0;
  SocketUtils::SendString(sock,"testin1");
  SocketUtils::Recv(sock,&val,sizeof(val));
  if(val == 0)
    return true;
  else
    return false;
}

bool MainWindow::getOUT1(){
  unsigned short parameters[256];
  SocketUtils::SendString(sock,"gparms");
  SocketUtils::Recv(sock,parameters,sizeof(parameters));
  if ( parameters[0] == 200 )
    return true;
  else
    return false;
}

bool MainWindow::getOUT2(){
  unsigned short parameters[256];
  SocketUtils::SendString(sock,"gparms");
  SocketUtils::Recv(sock,parameters,sizeof(parameters));
  if ( parameters[1] == 200 )
    return true;
  else
    return false;
}
#endif

bool MainWindow::TestSerialLoop(){

  unsigned char val;
  SocketUtils::SendString(sock,"test_serial_port");
  SocketUtils::Recv(sock,&val,sizeof(val));

  if(val == 1)
    return true;
  else
    return false;

}

bool MainWindow::TestSerial(char* serial_port){

  char serial_buffer[BUFFER_SIZE];
  char serial_port_1[BUFFER_SIZE];
  bool ret;
  strcpy(serial_port_1,serial_port);
  //qDebug() <<serial_port;
  /* Inizializza la comunicazione (controllare che i parametri siano corretti */
  serial_buffer[0] = '\0';
  QString tmp(serial_port);

  if (SNP_initCommunication(serial_port, 115200, 8, NOPARITY, ONESTOPBIT) >= 0)
    {
      if (SNP_getKernelVersion(2, serial_buffer, BUFFER_SIZE) >= 0)
        ret =  true;
      else
        ret = false;
      /* chiudi la comunicazione */
      SNP_endCommunication();
    }
  else
    ret =  false;

  return ret;
}


bool MainWindow::restore(){
  SocketUtils::SendString(sock,"restore");
  return true;
}

void MainWindow::slotAddPermanentWidgets(){
  setPermanentWidget();
}

void MainWindow::setPermanentWidget(){
  /*fps.setText("fps: ---");
    this->ui->statusBar->addPermanentWidget(&fps);
    fps.show();*/

  char version[32];

  SocketUtils::SendString(sock,"ker_version");
  SocketUtils::RecvString(sock,version);
  kernel.setText(" kernel v. " + QString(version));
  this->ui->statusBar->addPermanentWidget(&kernel);
  kernel.show();

  SocketUtils::SendString(sock,"sys_version");
  SocketUtils::RecvString(sock,version);
  system.setText(" system v. " + QString(version));
  system.show();
  this->ui->statusBar->addPermanentWidget(&system);

  SocketUtils::SendString(sock,"fw_version");

  SocketUtils::RecvString(sock,version);
  firmware.setText(" firmware v. " + QString(version));
  firmware.show();
  this->ui->statusBar->addPermanentWidget(&firmware);

  SocketUtils::SendString(sock,"version");
  SocketUtils::RecvString(sock,version);
  imgserver.setText(" imgserver v. " + QString(version));
  imgserver.show();
  this->ui->statusBar->addPermanentWidget(&imgserver);
}

void MainWindow::resizeEvent(QResizeEvent *){
  /*this->ACQL->disconnect();
    connect(this->ACQL, SIGNAL(imageSx(const QImage&)), this, SLOT(displayImageLeft(const QImage&)));
    connect(this->ACQL, SIGNAL(imageDx(const QImage&)), this, SLOT(displayImageRight(const QImage&)));
    connect(this->ACQL, SIGNAL(imageDsp(const QImage&,bool)), this, SLOT(displayImageDsp(const QImage&)));
    connect(this->ACQL, SIGNAL(imagesMode(tModeImages)), this, SLOT(ImgsModeReceiver(tModeImages)));*/
}

void MainWindow::clearPermanentWidget(){
  this->ui->statusBar->removeWidget(&kernel);
  this->ui->statusBar->removeWidget(&system);
  this->ui->statusBar->removeWidget(&firmware);
  this->ui->statusBar->removeWidget(&imgserver);
}

unsigned short MainWindow::BuildValueToSet(char* buffer,int start_index)
{

  unsigned short val_test = 0;
  // build the value to test
  QString q_buffer(buffer);

  const int size_val_str = q_buffer.length() - start_index; // devo considerare anche lo spazio
  char* val_str = (char*) calloc(size_val_str, sizeof(char));
  memcpy(val_str,&buffer[start_index],size_val_str*sizeof(char));
  QString q_val_str(val_str);
  val_test = (unsigned short) q_val_str.toShort(0,10);
  free(val_str);

  qDebug()<<" BuildValuetoSet" <<val_test << val_str;
  return   val_test;

}

char* MainWindow::BuildSerialPortAddress(char* buffer,int start_index)
{
  QString buf_str(buffer);
  const int dim_serial_port = buf_str.length() - start_index - 2; // il -1 per considerare il numero della seriale e lo spazio seguente a TestSerial
  char* serial_port = (char*) malloc(dim_serial_port*sizeof(char));
  int i = 0;
  for (; i < dim_serial_port;++i )
    serial_port[i] = buffer[start_index + 2 + i];

  serial_port[i] = '\0';

  qDebug() << dim_serial_port << buf_str << serial_port;

  return  serial_port;

}
