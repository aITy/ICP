/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 *
 * Desc: Main file covering the whole CLI version of draughts game.
 */

#include <QtCore>
//#include <QtCore/QCoreApplication>
#include "app.h"

int main(int argc, char *argv[]) {
  //QApplication();
  QCoreApplication app(argc, argv);

  // all the following signal-slot handling is essential - see
  // http://stackoverflow.com/questions/4180394/how-do-i-create-a-simple-qt-console-application-in-c#comment4516692_4180485
  App my_app(&app);
  // finish Qt event loop (i.e. the whole program) after receiving finished()
  QObject::connect(&my_app, SIGNAL(finished()), &app, SLOT(quit()));
  my_app.refresh();
  //QTimer::singleShot(0, &my_app, SLOT(run()));

  return app.exec();
}
