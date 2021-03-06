/*!
 *\file mainwindow.h
 * File din header della classe MainWindow.+
 * Contiene la dichiarazione delle classe, delle relative funzioni
 *
 *
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define NOMINMAX
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QColor>
#include <QDebug>
#include <QLabel>
#include <QFuture>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>
#include <QtCore>

#include <Windows.h>
#include <stdio.h>
#include "utils.h"
#include "socket_utils.h"
#include "define_file.h"
#include "structures.h"
#include "acq_thread.h"
#include "CheckPCN/GraphDisparity.h"
#include "CheckPCN/check_pcn.h"
#include "parameters_panel.h"
#include "SNP/SNP.h"

#define BUFFER_SIZE 100

//#define SET_DIGITAL_IO_WITH_PARMS

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  // Costruttore del SOCKET
  bool createSocketScheduler();   //!< Creazione della socket per la comunicazione con lo scheduler
  bool LoadDat(QString dat_filename);  //!< Caricamento del file di calibrazione in formato .dat
  bool Communication(int fd,char *buffer);  //!< Funzione per il parsing e l'esecuzione dei comandi provenienti dello scheduler

public slots:
  void showAbout();       //!< Slot per mostrare l'opzione About
  void captureError( QString text );   //!< Cattura gli errori e li stampa a video

  /** Visualizzazione delle immagini */
  void displayImageLeft( const QImage& image );  //!< Visualizza l'immagine di sinistra
  void displayImageRight( const QImage& image );  //!< Visualizza l'immagine di destra
  void displayImageDsp( const QImage& image);  //!< Visualizza l'immagine di disparità

  /** Show Info */
  void showInfo( QString text, QColor color = Qt::black, bool disableTime = false );  //!< Stampa in textEdit la stringa passata come parametro
  void clearInfo();                               //!< Pulisce Info textEdit


  /** Start and stop acquisition */
  void startAcquisition();  //!< Inizia il thread di acquisizione delle immagini e le mostra nell'interfaccia
  void stopAcquisition(); //!< Ferma il thread di acquisizione delle immagini

  void startViewSlot( const bool& choose, int fd );  //!< Gestisce lo slot che regola la visualizzazione delle imamgini

  /** Ricezione immagini moda */
  void ImgsModeReceiver( tModeImages dataset );   //!< Riceve una struttura contenente i dati moda

  /** Connect and disconnect */
  bool setConnect(QString adressIP);  //!< Connette l'applicativo al PCN
  bool setDisconnect();  //!< Disconnette l'applicativo al PCN

  /** Funzioni di check */
  bool CheckDisparity(); //!< Check della mappa di disparità
  bool CheckFocus();  //!< Check del Focus
  void showResponseComputation(std::vector<std::pair<QString, bool> > response);  //!< Mostra il risultato del test

  /** Salvataggio e caricamento dei parametri */
  //void showParametersPanel();
  void saveParameters(tParameters param); //!< Salva i parametri passati come parametro
  void saveParameters(const tParameters& param, QString fileName); //!< Salva i parametri
  //void receiverParameters(tParameters param);
  tParameters loadParameters();  //!< Carica i parametri salvati su file XML

  /** Settings panel */
  void startParametersPanel(); //!< Funzioni di utilità per la gestione dei parametri
  void receiverParameters(tParameters param); //!< Funzioni di utilità per la gestione dei parametri

  /** SaveReport */
  bool SaveReport();  //!< Salva il report relativo al check del PCN con le immagini usate per il test
  void SaveReportSlot();  //!< Slot per la gestione del salvataggio del report

  /** Test Digital I/O **/
  QString getIN0();   //!< Restituisce il valore del digital IN0
  QString getIN1();   //!< Restituisce il valore del digital IN1
  bool setOpto(unsigned char value);  //!< Setta un determinato valore al digital OUT0/1 in base al valore value
  bool setOptoFullControl(unsigned char value);  //!< Setta un determinato valore a OUTTIME0/1 in base al valore value

  /** Test Seriale **/ // modifica 11/12/2013
  bool TestSerial();  //!< Test della porta seriale in caso di PCN ethernet (una sola seriale)
  bool TestSerialLoop();  //!< Test le porte seriali nel caso di PCN USB (ha due seriale)

  /** Restore PCN */
  bool restore();  //!< Comando per ripristinare il PCN alle condizioni di fabbrica

  bool getFirmwareVersion(QString & version);
  bool getImgserverVersion(QString & version);

  /** Add permanent Widget */
  void setPermanentWidget();  //!< Funzioni per la gestione dei Widget
  void clearPermanentWidget();  //!< Funzioni per la gestione dei Widget
  void slotAddPermanentWidgets();  //!< Funzioni per la gestione dei Widget

  /** Support Function*/
  unsigned short BuildValueToSet(char* buffer,int start_index);  //!< Crea il valore da passare di tipo unsgned short da passare come parametro
  char* BuildSerialPortAddress(char* buffer,int start_index);  //!< Crea la stringa che rappresenta la porta seriale del PC su cui gira l'applicativo in base al comando TestSerial######


private:
  int createStructFileDat( short* ODCBuff, char* offset, int&  index_offset, int& index_buffer, FILE* fd, char& value );

protected:
  bool createSocket(QString IP_ADRESS_PCN_);  //!< Crea la socket di comunicazione con il PCN

protected slots:
  void resizeEvent(QResizeEvent *);          //!< Resize della finestra
  void closeEvent(QCloseEvent *);            //!< Operazioni durante chiusura programma

signals:
  void errorMessage( QString text );      //!< Emette segnale di errore
  void messageInfo( QString text, QColor color = Qt::black, bool disableTime = false );       //!< Info da stampare nella TextEdit
  void startViewSignal( const bool& choose, int fd );               //!< Fa partire la visualizzazione delle immagini
  void saveReportSignal();
  void addPermanentWidgets();
  void signalClearPermanentWidget();

private:
  Ui::MainWindow *ui;

  /** Socket variables **/
  struct sockaddr_in my_addr_data,serv_addr_data;   //!< Struttura per la gestione della socket
  struct sockaddr_in serv_addr,remoteaddr;          //!< Struttura per la gestione della socket
  WSADATA dat;
  WSADATA scheduler;
  SOCKET imgfd;                                     //!< Socket per l'acquisizione delle immagini
  SOCKET sock;                                      //!< Socket per comunicazione con PCN
  SOCKET sock_scheduler;                            //!< Socket comunicazione con lo Scheduler
  QFuture<void> future;                             //!< Gestisce il thread per lo scheduler

  /** Thread acquisizione **/
  AcqThread* ACQL;                                  //!< Thread di acquisizione
  QPixmap pixSx;                                    //!< Immagine Sx
  QPixmap pixDx;                                    //!< Immagine Dx
  QPixmap pixDsp;                                   //!< Immagine Dsp

  tModeImages imagesMode;                           //!< mode dei datasets

  QImage imgSx_;
  QImage imgDx_;
  QImage imgDsp_;


  /** Parametri per la connessione **/
  bool isConnect;                     //!< Flag per lo stato di connessione con il PCN
  bool isConnectScheduler;            //!< Flag per lo stato di connessione con lo scheduler
  QString IP_ADRESS_PCN_connected;    //!< Indirizzo IP del PCN connesso

  /** Parametri scheduler **/
  QMutex mutex;
  //QMutex mutex_startView;
  bool stop_scheduler;
  bool startView;
  int fd_copy;    //Copia dell'fd

  /** Parametri esterni **/
  tParameters _param;
  ParametersPanel *_parametersPanel;

  /** Salvataggio report **/
  QString dat_filename;
  int n_modeSaved;
  bool startMemLog;
  QString MemLog;

  /** Name report **/
  QString name_report;

  /** Label per la visualizzazione dell'informazioni di sistema **/
  QLabel kernel;
  QLabel system;
  QLabel firmware;
  QLabel imgserver;
  QTime time_fps;

  unsigned short in1_current_val;
  unsigned short in0_current_val;
  unsigned short out1_current_val;
  unsigned short out0_current_val;

};

#endif // MAINWINDOW_H
