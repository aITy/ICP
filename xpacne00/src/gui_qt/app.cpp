/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "app.h"
//#include "../share/board.h"

App::App(QCoreApplication *_parent) :
  QObject(_parent),
  out(stdout, QIODevice::WriteOnly | QIODevice::Text),
  eout(stderr, QIODevice::WriteOnly | QIODevice::Text),
  in(stdin, QIODevice::ReadOnly),
  server(NULL),
  notifier(NULL) {
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(gotConnection()));

    if (! server->listen(QHostAddress::Any, 0)) {
      //if (! server->listen(QHostAddress::LocalHost, 0)) {
      eout << "ERR: TCP server not listening." << endl;
      Q_EMIT finished();  // emit blocks unless queued connections are used
    }

    out << "addr " << server->serverAddress().toString() << " port " <<
      QString::number(server->serverPort()) << endl;

    notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
    //it does line buffering after \n character => called after one line given
    //  for immediate reaction use setbuf(stdin, _IOFBF)
    connect(notifier, SIGNAL(activated(int)), SLOT(handleInput(void)));
    // not needed (notifier is set enabled by default)
    // notifier->setEnabled(true);
}

App::~App() {
  if (server != NULL) delete server;
  if (notifier != NULL) delete notifier;
}

void App::refresh(void) {
  //QStringList sl = par->arguments();
  ////QList<QString>::iterator
  //for (QStringList::iterator i = sl.begin(); i != sl.end(); ++i) {
  //  //std::cout << i.toLocal8Bit().constData() << std::endl;
  //  std::cout << (*i).toLocal8Bit().constData() << std::endl;
  //}

  /*
  //QFile file_stdin;
  file_stdin.open(stdin, QIODevice::ReadWrite);
  //file_stdin.open(stdin, QIODevice::ReadOnly);
  QTextStream in(&file_stdin);
  */

  out << "drawing the board..." << endl;
  out << "arguments: ";

  QStringListIterator it(cmd_l);
  while (it.hasNext()) {
    out << it.next().toLocal8Bit().constData() << endl;
  }

  out << "choose y/n: ";
  for (;;) {
    QString l = in.readLine();
    if (l.at(0))
  }

  cmd_l.clear(); //FIXME needed?
}

void App::handleInput(void) {
  /** read one line of user input */
  line = in.readLine();
  if (! line.isEmpty()) {
    //QString word;
    //qtin >> word;  // read a word (separated by space)
    cmd_l = line.split(" ", QString::SkipEmptyParts);
    out << "cmd_l.size(): " << cmd_l.size() << endl;
    if (cmd_l.size()) {
      out << "at(0): " << cmd_l.at(0) << endl;
      if (cmd_l.at(0) == "q") Q_EMIT finished();
    }
  }
  else {
    out << "LINE empty!" << endl;
  }
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
    eout << "no pending connections" << endl;
  }
}
