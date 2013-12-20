#include "utils.h"




/*
Crop Image
*/
unsigned char* Utils::CropImage(unsigned char* img, const int& new_w, const int& new_h)
{
  int sz_crop_image = new_w*new_h;
  unsigned char* crop_image = (unsigned char*)malloc(sz_crop_image*sizeof(unsigned char));
  unsigned char* ptr_crop_image;
  unsigned char* ptr_img;
  int cr = 0;
  for (int r = BORDER_Y; r < NY - BORDER_Y; ++r, ++cr)
    {
      ptr_img = &img[r*NX + BORDER_X];
      ptr_crop_image = &crop_image[cr*new_w];
      for (int c = BORDER_X; c < NX - BORDER_X_2; ++c, ++ptr_img, ++ptr_crop_image)
        {
          *ptr_crop_image = *ptr_img;
        }

    }
  return crop_image;
}

bool Utils::findIntInVect( std::vector<int> vect, const int& value ){
  for ( int i=0; i<vect.size(); ++i ){
      if ( vect.at(i) == value )
        return true;
    }
  return false;
}



QImage Utils::pixeltype2qimage( PixelType *img ){

  QImage image_result(img, NX, NY,QImage::Format_Indexed8);

  return image_result;
}

tModeImages Utils::getModeImgs( PixelType** datasetSx, PixelType** datasetDx, PixelType** datasetDsp ){


  tModeImages out;
  out.imgSx = Utils::getModeImg(datasetSx);
  out.imgDx = Utils::getModeImg(datasetDx);
  out.imgDsp = Utils::getModeImg(datasetDsp);
  return out;
}

PixelType* Utils::getModeImg( PixelType** dataset ){

  PixelType* out = (PixelType*) calloc( NN, sizeof(PixelType) );
  test_alloc( out );
  PixelType* temp_img = (PixelType*) calloc( NUM_FRAME_TO_PROCESS, sizeof(PixelType) );
  test_alloc( temp_img );

  for ( int j=0; j<NN; ++j ){

      for ( int i=0; i<NUM_FRAME_TO_PROCESS; ++i )
        temp_img[i] = dataset[i][j];


      out[j] = Utils::myMax(Utils::myCalcHist(temp_img, NUM_FRAME_TO_PROCESS));

    }


  free(temp_img);
  return out;
}


int* Utils::myCalcHist(unsigned char* img, const int& size){

  int* hist = (int*)calloc(N_BINS, sizeof(int));

  for (int i = 0; i < size; ++i)
    if ( img[i] >= 0 && img[i] < N_BINS )
      hist[img[i]]++;

  return hist;
}

PixelType Utils::myMax( int* vect ){
  PixelType max = 0;
  for ( int i = 0; i<N_BINS; ++i )
    if ( max < vect[i] )
      max = (PixelType) i;

  free( vect );
  return max;
}




unsigned char* Utils::rgb2gray(const QImage& image){

  QImage image_res(image);
  unsigned char* tmp = (unsigned char*) calloc(NN,sizeof(unsigned char));
  unsigned char* res = (unsigned char*) calloc(NN,sizeof(unsigned char));
  QRgb col;
  int width = image.width();
  int height = image.height();
  int k = 0;
  for (int j = 0; j < height; ++j)
    {
      for (int i = 0; i < width; ++i)
        {
          col = image.pixel(i, j);
          tmp[k++] = qGray(col);
        }
    }
  k = 0;
  for (int j = 0; j < height; ++j)
    {
      for (int i = 0; i < width; ++i)
        {
          col = image.pixel(i, j);
          res[j*width + i] =  (unsigned char) qGray(col);
          //if ( j == 8)
          //  qDebug()<< res[j*width + i];
        }
    }
  free(tmp);
  return res;
}

void Utils::saveImage(  unsigned char* img, QString& path)
{
  QPixmap::fromImage(QImage(img,160,120,QImage::Format_Indexed8)).save(path, "PNG");
}

bool Utils::readXmlFile( QIODevice& device, QSettings::SettingsMap& map )
{
  QXmlStreamReader xmlReader( &device );

  QString currentElementName;
  while( !xmlReader.atEnd() )
    {
      xmlReader.readNext();
      while( xmlReader.isStartElement() )
        {
          if( xmlReader.name() == "SettingsMap" )
            {
              xmlReader.readNext();
              continue;
            }

          if( !currentElementName.isEmpty() )
            {
              currentElementName += "/";
            }
          currentElementName += xmlReader.name().toString();
          xmlReader.readNext();
        }

      if( xmlReader.isEndElement() )
        {
          continue;
        }

      if( xmlReader.isCharacters() && !xmlReader.isWhitespace() )
        {
          QString key = currentElementName;
          QString value = xmlReader.text().toString();

          map[ key ] = value;

          currentElementName.clear();
        }
    }

  if( xmlReader.hasError() )
    {
      return false;
    }

  return true;
}

bool Utils::writeXmlFile( QIODevice& device, const QSettings::SettingsMap& map )
{
  QXmlStreamWriter xmlWriter( &device );
  xmlWriter.setAutoFormatting( true );

  xmlWriter.writeStartDocument();
  xmlWriter.writeStartElement( "SettingsMap" );

  QSettings::SettingsMap::const_iterator mi = map.begin();

  for( mi; mi != map.end(); ++mi )
    {
      std::vector< std::string > groups;

      QStringList groups_ = mi.key().split("/");
      for ( int i=0; i<groups_.size(); ++i ){
          groups.push_back(groups_.at(i).toStdString().c_str());
        }
      foreach( std::string groupName, groups )
        {
          xmlWriter.writeStartElement( groupName.c_str() );
        }

      xmlWriter.writeCharacters( mi.value().toString() );

      foreach( std::string groupName, groups )
        {
          xmlWriter.writeEndElement();
        }
    }

  xmlWriter.writeEndElement();
  xmlWriter.writeEndDocument();

  return true;
}

bool Utils::removeDir(const QString & dirName)
{
  bool result = true;
  QDir dir(dirName);

  if (dir.exists(dirName)) {
      Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
          if (info.isDir()) {
              result = removeDir(info.absoluteFilePath());
            }
          else {
              result = QFile::remove(info.absoluteFilePath());
            }

          if (!result) {
              return result;
            }
        }
      result = dir.rmdir(dirName);
    }
  return result;
}


int Utils::strcasecmp( char *s1,  char *s2, int index)
{
   char* c1 = s1;
   char* c2 = s2;
   int i = 0;
   while( i < index && *c1 != '\0')
    {
      if( *c1 == *c2)
        {
          ++c1;
          ++c2;
          ++i;
        }
      else
        return (*c2-*c1);
    }

  return 0;

}

