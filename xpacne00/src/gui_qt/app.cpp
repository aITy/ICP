/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "app.h"
//#include "../share/board.h"

App::App(QCoreApplication *_parent) : QObject(_parent),
  out(stdout, QIODevice::WriteOnly | QIODevice::Text), server(NULL) {}

App::~App() {
  if (server != NULL) delete server;
}

void App::run(void) {
  //QStringList sl = par->arguments();
  ////QList<QString>::iterator
  //for (QStringList::iterator i = sl.begin(); i != sl.end(); ++i) {
  //  //std::cout << i.toLocal8Bit().constData() << std::endl;
  //  std::cout << (*i).toLocal8Bit().constData() << std::endl;
  //}

  server = new QTcpServer(this);

  connect(server, SIGNAL(newConnection()), this, SLOT(gotConnection()));

  if (! server->listen(QHostAddress::Any, 0)) {
  //if (! server->listen(QHostAddress::LocalHost, 0)) {
    qDebug() << "ERR: TCP server not listening." << endl;
    Q_EMIT finished();  // this blocks unless queued connections are used
  }
  qDebug() << server->serverAddress().toString() << "port" <<
    QString::number(server->serverPort()) << endl;

  //QFile file_stdin;
  file_stdin.open(stdin, QIODevice::ReadWrite);
  //file_stdin.open(stdin, QIODevice::ReadOnly);
  QTextStream in(&file_stdin);

  QString line;

  for (;;) {
    /** read one line of user input */
    line = in.readLine();
    //QString word;
    //qtin >> word;  // read a word (separated by space)
    QStringList cmd_l = line.split(" ", QString::SkipEmptyParts);
    out << "cmd_l.size(): " << cmd_l.size() << endl;
    out << "at(0): " << cmd_l.at(0) << endl;
  }

  //file.close();  // not needed
  Q_EMIT finished();
}

void App::gotConnection() {
  // not needed in this case; only for demonstration
  QTcpServer* ser = qobject_cast<QTcpServer *>(this->sender());

  out << "gotConnection()" << endl;

  if (ser->hasPendingConnections()) {
    out << "some pending connections, connecting..." << endl;
    ser->nextPendingConnection()->deleteLater();
    out << "manually disconnected" << endl;
  }
  else {
    // this could occur when the initiator disconnects really fast
    out << "no pending connections" << endl;
  }
}
