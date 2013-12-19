/*!
 *\file socket_utils.h
 *
 * File header contenente le dichiarazione delle funzioni della classe SocketUtils
 *
 *\author Alessio Montagnini, Omar Zandonà(eVS - embedded Vision Systems s.r.l. www.embeddedvisionsystems.it)
*/

#ifndef _SOCKET_UTILS_H_
#define _SOCKET_UTILS_H_

#define NOMINMAX
#include <Windows.h>
#include <QImage>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include "define_file.h"
#include "Structures.h"
#include <QSettings>
#include <QXmlStreamWriter>


using namespace std;

/*!
 * \brief La classe SocketUtils
 * La classe SocketUtils provvede alla dichiarazione delle funzioni necessarie per l'invio e la ricezione di dati sui canali socket dei programmi.
 */
class SocketUtils
{

public:
    SocketUtils();  //!< Costruttore
    ~SocketUtils(); //!< Distruttore

    static int Recv(int fd,void *buf,int len);  //!< Ricezione dati
    static int SendString(int fd,char *buf);  //!< Invio stringa
    static int RecvString(int fd,char *buf,int maxlen); //!< Ricezione stringa di dimensione max predefinita
    static int RecvString( int fd, char * buf);  //!< Ricezione stringa
    static unsigned short getAcqMode(int index);  //!< Settaggio della modalit&agrave di acquisizione
    static int Send(int fd,void *buf,int len);  //!< Invio dati

};

#endif

