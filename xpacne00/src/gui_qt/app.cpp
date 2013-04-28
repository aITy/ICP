/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "app.h"
//#include "../share/board.h"

App::App(QCoreApplication *_parent) : QObject(_parent) {}

void App::run(void) {
  //QStringList sl = par->arguments();
  ////QList<QString>::iterator
  //for (QStringList::iterator i = sl.begin(); i != sl.end(); ++i) {
  //  //std::cout << i.toLocal8Bit().constData() << std::endl;
  //  std::cout << (*i).toLocal8Bit().constData() << std::endl;
  //}

  QTcpServer server;

  if (! server.listen(QHostAddress::Any, 0)) {
    qterr << "ERR: TCP server not listening."  << endl;
    Q_EMIT finished();
  }

  connect(socket, SIGNAL(connected()), this, SLOT(gotConnected()));

  //QFile file_stdin;
  file_stdin.open(stdin, QIODevice::ReadWrite);
  //file_stdin.open(stdin, QIODevice::ReadOnly);
  QTextStream in(&file_stdin);

  QFile file_stdout;
  file_stdout.open(stdout, QIODevice::WriteOnly | QIODevice::Text);
  QTextStream out(&file_stdout);

  QString line;

  for (;;) {
    /** read one line of user input */
    line = in.readLine();
    //QString word;
    //qtin >> word;  // read a word (separated by space)
    QStringList cmd_l = line.split(" ", QString::SkipEmptyParts);

    //std::cout << "at(0): " << cmd_l.at(0) << std::endl;
    out << QString("at(0): ") << endl;
  }

  //file.close();  // not needed
  Q_EMIT finished();
}

void App::gotConnected() {
  qDebug() << t
  if (server.hasPendingConnections()) {
    server.nextPendingConnection()->deleteLater();
  }
}
