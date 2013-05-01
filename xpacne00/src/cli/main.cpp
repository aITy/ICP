/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 *
 * Desc: Main file covering the whole CLI version of draughts game.
 */

#include "app.h"

int main(int argc, char *argv[]) {
  //QApplication();
  QCoreApplication app(argc, argv);
  App my_app(&app);

  /** end Qt event loop (i.e. the whole program) after receiving finished() */
  QObject::connect(&my_app, SIGNAL(finished()), &app, SLOT(quit()));
  /** schedule refresh to be processed in next event loop, see
      http://www.qtcentre.org/threads/29989-QTimer-singleShot(0-confusion */
  QTimer::singleShot(0, &my_app, SLOT(refresh()));
  //QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);

  /** start the main loop */
  return app.exec();
}
