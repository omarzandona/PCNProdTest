#include "utils.h"


/*
 * Prende solo la parte interna dell'immagine
 */
unsigned char* Utils::internalImage(unsigned char* img, const int& w, const int& h, int dim_w, int dim_h){

  if ( dim_h >= h )
    dim_h = h;
  if ( dim_w >= w )
    dim_w = w;

  int shift_w = dim_w/2;
  int shift_h = dim_h/2;
  int center_w = w/2;
  int center_h = h/2;
  int new_w = shift_w*2;
  int new_h = shift_h*2;
  unsigned char* img_out = (unsigned char*) calloc(new_w*new_h, sizeof(unsigned char));

  int start_w, start_h, end_w, end_h;
  start_w = center_w - shift_w;
  end_w = center_w + shift_w;
  start_h = center_h - shift_h;
  end_h = center_h + shift_h;

  int k = 0;
  for ( int i=start_h; i<end_h; ++i )
    for( int j=start_w; j<end_w; ++j ){
        img_out[k++] = img[i*w+j];
      }

  return img_out;
}

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
  //QPixmap pix;
  //pix.loadFromData(buff, sizeOfBuff, "JPG");
  //return QImage(img, NX, NY, QImage::Format_Indexed8);

  QImage image_result(img, NX, NY,QImage::Format_Indexed8);

  /*QRgb col;
    int gray;
    int width = image_result.width();
    int height = image_result.height();
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            qDebug() << image_result.pixel(i, j);
            col = image_result.pixel(i, j);
            gray = qGray(col);
            image_result.setPixel(i, j, qRgb(gray, gray, gray));
        }
    }*/

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
      //qDebug() << "test1" << j;
      for ( int i=0; i<NUM_FRAME_TO_PROCESS; ++i )
        temp_img[i] = dataset[i][j];
      //qDebug() << "test2" << j;
      out[j] = Utils::myMax( Utils::myCalcHist(temp_img, NUM_FRAME_TO_PROCESS) );
      //qDebug() << "test3" << j;
    }

  free( temp_img );
  return out;
}


int* Utils::myCalcHist(unsigned char* img, const int& size){

  int* hist = (int*)calloc(N_BINS, sizeof(int));

  for (int i = 0; i < size; ++i)
    if ( img[i] >= 0 && img[i] < N_BINS )
      hist[img[i]]++;
    else
      qDebug() << img[i];

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


double interpolate( double val, double y0, double x0, double y1, double x1 ) {
  return (val-x0)*(y1-y0)/(x1-x0) + y0;
}
double blue( double grayscale ) {
  if ( grayscale < 85 ) return interpolate( grayscale, 255, 0, 0.0, 85 );
  else return 0.0;
}
double green( double grayscale ) {
  if ( grayscale < 85 ) return 1.0;
  else if ( grayscale <= 1.0 ) return interpolate( grayscale, 255, 85, 0.0, 255 );
  else return 255; // unexpected grayscale value
}
double red( double grayscale ) {
  if ( grayscale < 85 ) return interpolate( grayscale, 0.0, 0, 255, 85 );
  else return 255;
}

QPixmap Utils::colorizeDepth( QImage img ){
  PixelType* _img = img.bits();

  int width = img.width();
  int height = img.height();

  QImage out(width, height, QImage::Format_RGB32 );
  int value, R, G, B;
  for (int i = 0; i < width; ++i)
    for (int j = 0; j < height; ++j)
      {
        value = (int) _img[ j*width + i ];
        if ( value == 16 )
          {
            R = 255;
            B = 0;
            G = 140;
          }else{
            if ( value == 0 )
              R = G = B = 0;
            else{
                R = (int)abs(255-value) %256 ;//(int)abs(value*1.1) %255;
                G = 0;//(int)abs(value*0.5) %255;
                B = ((int)abs(value) % 255) ;//(int)abs(255-R) %255;
              }
          }

        out.setPixel(i, j, qRgb(R, G, B));
        //out.setPixel(i,j,qRgb(31, 31 , 49));
      }


  return QPixmap::fromImage(out);
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

