#include "socket_utils.h"

SocketUtils::SocketUtils()
{
}
SocketUtils::~SocketUtils()
{

}
/*****************************************************/
int SocketUtils::RecvString(int fd,char *buf,int maxlen)
{
    int sum = 0;
    int bytes = 0;
    int counter,ret;
    char a;

    strcpy(buf,"\0");
    counter = 0;
    a = '1';
    while(a && sum < maxlen)
    {
        bytes = recv(fd,&a,1, 0);
        if(bytes > 0 )
        {

            buf[counter] = a;
            counter++;
            sum += bytes;
        }
        else return bytes;
    }

    return sum;
}

int SocketUtils::RecvString( int fd, char * buf)
{
  int sum = 0;
  int counter;
  int bytes;
  char a;

  strcpy(buf,"\0");
  counter = 0;
  a = '1';
  while(a && sum < MAX_STR_LENGTH)
    {
      bytes = recv(fd,&a,1, 0);
      if(bytes > 0)
        {
          buf[counter] = a;
          counter++;
          sum += bytes;
        }
      else return bytes;
    }
  return sum;
}

/***********************************************************************/
int SocketUtils::Recv(int fd, void *buf, int len){
    int sum = 0;
    int bytes; // = 0;
    int bytesleft = len;
    char * buffer;

    buffer = (char *)buf;
    while(sum < len)
    {
        bytes = recv(fd,buffer+sum,bytesleft, 0);
        if(bytes > 0) {sum += bytes; bytesleft -= bytes;}
        else return bytes;
    }
    return sum;

}
/***********************************************************************/

int SocketUtils::SendString(int fd,char *buf)
{
    int bytes;
    int i;

    i = 0;
    while(buf[i++] && i < MAX_STR_LENGTH);

    bytes = send(fd,buf,i, 0);


    return bytes;
}

/***********************************************************************/
unsigned short SocketUtils::getAcqMode(int index)
{
    unsigned short acq_mode;

    switch(index)
    {
    case 0:
        acq_mode = MODE_FPN;
        break;
    case 1:
        acq_mode = MODE_FPN_ODC;
        break;
    case 2:       // Sx, Dx, Disparity images
        acq_mode = MODE_DISP_FPN_ODC;
        break;
    case 3:
        acq_mode = MODE_DISP_MEDIAN_FPN_ODC;
        break;
    case 4:        // tracking
        acq_mode = MODE_DISP_MEDIAN_FPN_ODC_TRACK;
        break;
    case 5:        // sobel
        acq_mode = MODE_DISP_FPN_ODC_SOBEL;
        break;
    default:
        acq_mode = MODE_FPN_ODC;
    }

    return acq_mode;
}

/****************************************************************/
int SocketUtils::Send(int fd,void *buf,int len)
{
    int sum = 0;
    int bytes; // = 0;
    int bytesleft = len;
    char * buffer;

    buffer = (char *)buf;
    while(sum < len)
    {
        bytes = send(fd,buffer+sum,bytesleft, 0);
        if(bytes > 0) {sum += bytes; bytesleft -= bytes;}
        else return bytes;
    }
    return sum;
}

