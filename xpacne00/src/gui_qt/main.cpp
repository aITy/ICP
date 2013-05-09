/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 *
 */

//#include <QtCore>
#include <QtCore/QCoreApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  //QApplication();
  QApplication app(argc, argv);
  // all the following signal-slot handling is essential - see
  // http://stackoverflow.com/questions/4180394/how-do-i-create-a-simple-qt-console-application-in-c#comment4516692_4180485
  MainWindow m;
  m.show();

  return app.exec();
}
