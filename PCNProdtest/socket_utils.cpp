/*! \file socket_utils.h
 *
 * Contiene  la definizione delle funzione della classe SocketUtils
 *
 *\author Alessio Montagnini, Omar Zandonà(eVS - embedded Vision Systems s.r.l. www.embeddedvisionsystems.it)
 */

#include "socket_utils.h"

/*!
 * \brief SocketUtils::SocketUtils
 *  Costruttore vuoto della classe
 */
SocketUtils::SocketUtils()
{
}

/*!
 * \brief SocketUtils::~SocketUtils
 * Distruttore della classe
 */
SocketUtils::~SocketUtils()
{
}
/*!
 * \brief SocketUtils::RecvString
 * Riceve una stringa sulla socket connessa
 * \param fd      [in] File descriptor della socket
 * \param buf     [in|out] Buffer contenente la stringa in arrivo
 * \param maxlen  [in] Massima lunghezza del messaggio in arrivo
 * \return sum, sar&agrave; un valore > 0 in caso in cui sono riuscito a ricevere qualcosa, 0 altrimenti
 */
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

/*!
 * \brief SocketUtils::RecvString
 * Riceve una stringa sulla socket connessa ( non ha limiti sulla lunghezza della stringa da ricevere) e la memorizza in un buffer
 * \param fd      [in] File descriptor della socket
 * \param buf     [in|out] Buffer contenente la stringa in arrivo
 * \return sum, sar&agrave; un valore > 0 in caso in cui sono riuscito a ricevere qualcosa, 0 altrimenti
 */
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

/*!
 * \brief SocketUtils::Recv
 * Riceve una determinato ( e non noto) tipo di dato sulla socket connessa e la memorizza in un buffer
 * \param fd      [in] File descriptor della socket
 * \param buf     [in|out] Buffer contenente i dati in arrivo
 * \param len     [in] lunghezza massima del dato in arrivo
 * \return  sum, sar&agrave; un valore > 0 in caso in cui sono riuscito a ricevere qualcosa, 0 altrimenti
 */
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

/*!
 * \brief SocketUtils::SendString
 * Invia una stringa sulla socket connessa
 * \param fd  [in] File descriptor della socket
 * \param buf [in] Stringa da inviare sulla socket
 * \return bytes > 0 se ho inviato qualcosa, <= 0 altrimenti
 */

int SocketUtils::SendString(int fd,char *buf)
{
    int bytes;
    int i = 0;
    while(buf[i++] && i < MAX_STR_LENGTH);

    bytes = send(fd,buf,i, 0);

    return bytes;
}

/*!
 * \brief SocketUtils::Send
 * Invia un determinato tipo di dato sulla socket connessa
 * \param fd   [in] File Descriptor della socket
 * \param buf  [in] Buffer contenente il dato inviare
 * \param len  [in] Lunghezza del dato da inviare
 * \return sum >0 se ho inviato qualcosa, <=0 altrimenti
 */
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

/*!
 * \brief SocketUtils::getAcqMode
 * Setta la modalità di visualizzazione delle immagini
 * \param index  [in] Indice della modalità la modalit&grave; di visualizzazione
 * \return acq_mode, modalità di visualizzazione
 */
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


