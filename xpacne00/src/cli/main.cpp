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

  // the following is essential, see
  // http://stackoverflow.com/questions/4180394/how-do-i-create-a-simple-qt-console-application-in-c#comment4516692_4180485

  // my implementation
  App my_app(app);
  //App *my_app = new App(&app);

  // finish Qt event loop (i.e. the whole program) after receiving finished()
  QObject::connect(&my_app, SIGNAL(finished()), &app, SLOT(quit()));
  QTimer::singleShot(0, &my_app, SLOT(run()));

  return app.exec();
}
