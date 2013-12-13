/*!
\file main.cpp
\brief Crea l'applicazione e lancia l'interfaccia.

Per i dettagli vedere la \see mainwindow.cpp.

\author Alessio Montagnini, Omar Zandon&agrave (eVS - embedded Vision Systems s.r.l. www.embeddedvisionsystems.it)
*/


#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  
  return a.exec();
}
