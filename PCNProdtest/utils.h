#ifndef UTILS_H
#define UTILS_H

#define NOMINMAX
#include <Windows.h>
//#pragma comment(lib,"Ws2_32.lib")
//#include <WinSock2.h>

#include <QImage>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QSettings>
#include <QXmlStreamWriter>
#include "CheckPCN/check_pcn.h"


using namespace std;
#include "define_file.h"
#include "structures.h"

#include "connected.h"
#include <iostream>
#include <windows.h>
#include <stdlib.h>




/** La classe Utils contiene funzioni statiche di uso comune */
class Utils{
public:
	Utils(){}
	~Utils(){}

	/****** Test Graph ******/
	//!< Test Componenti connesse

    static unsigned char* internalImage(unsigned char* img, const int& w, const int& h, int dim_w, int dim_h);
	static unsigned char* CropImage(unsigned char* img, const int& new_w, const int& new_h);
	static bool findIntInVect( std::vector<int> vect, const int& value );


	static QImage pixeltype2qimage( PixelType *img );
	static PixelType myMax( int* vect );
	static int* myCalcHist(unsigned char* img, const int& size);
	static QPixmap colorizeDepth( QImage img );
	static int strcasecmp( char *s1,  char *s2, int index);
	// Mode imgs
	static tModeImages getModeImgs( PixelType** datasetSx, PixelType** datasetDx, PixelType** datasetDsp );
	static PixelType* getModeImg( PixelType** dataset );

    static unsigned char* rgb2gray(const QImage& image);
    static void saveImage(  unsigned char* img, QString& path);

    // Save and load setting with xml
    static bool readXmlFile( QIODevice& device, QSettings::SettingsMap& map );
    static bool writeXmlFile( QIODevice& device, const QSettings::SettingsMap& map );
    static bool removeDir(const QString & dirName);

};



#endif
