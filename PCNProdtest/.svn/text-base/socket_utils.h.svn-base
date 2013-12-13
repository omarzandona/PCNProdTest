#ifndef _SOCKET_UTILS_H_
#define _SOCKET_UTILS_H_

#define NOMINMAX
#include <Windows.h>
//#pragma comment(lib,"Ws2_32.lib")
//#include <WinSock2.h>

#include <QImage>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include "define_file.h"
#include "Structures.h"
#include <QSettings>
#include <QXmlStreamWriter>







using namespace std;


/**
 * @brief The Utils class is a class of static methods
 */
class SocketUtils
{

public:
    SocketUtils();
    ~SocketUtils();


    static int Recv(int fd,void *buf,int len);
    static int SendString(int fd,char *buf);
    static int RecvString(int fd,char *buf,int maxlen);
    static int RecvString( int fd, char * buf);
    static unsigned short getAcqMode(int index);
    static int Send(int fd,void *buf,int len);

};

#endif
// _SOCKET_UTILS_H_
