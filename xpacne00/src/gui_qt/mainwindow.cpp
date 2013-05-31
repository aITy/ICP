/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow* MainWindow::instance = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    instance = this;
    setupUi(this);
    setWindowTitle(qApp->applicationName());
    setupActions();
    myStatusBar->setStyleSheet("padding-bottom: 15px");

    /** start TCP server */
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
    if (! server->listen(QHostAddress::Any, 0))
        setStatusMsg("TCP server is not listening");
    else
        portLabel->setText(portLabel->text() + QString::number(server->serverPort()));

    //connect(this, SIGNAL(storePlayer(Player::color_t,QString, GameBoard*)), this, SLOT(savePlayer(Player::color_t, QString, GameBoard*)));
	prepared_game = NULL;
	prepared_board = NULL;
}


/**
 * reimplemented close event function that prompt user, if he really want to quit the application
 */
void MainWindow::closeEvent(QCloseEvent * event) {
        QMessageBox::StandardButton choice;
        choice = QMessageBox::question(this,
                trUtf8("Opravdu ukoncit?"),
                trUtf8("Opravdu chcete ukoncit aplikaci '%1'?<br><b>Neulozene zmeny budou ztraceny!</b>").arg(qApp->applicationName()),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No);

        if (choice == QMessageBox::Yes)
                event->accept();
        else
                event->ignore();
}

/**
 * function that set up connects on menu actions
 */

void MainWindow::setupActions() {
        // connects
        connect(action_NewLocalGame, SIGNAL(triggered()), this, SLOT(createLocalCpu()));
        connect(action_NewLocalGameVs, SIGNAL(triggered()), this, SLOT(createLocalVs()));
        connect(action_NewNetGame, SIGNAL(triggered()), this, SLOT(showNewNetDialog()));
        connect(action_Open, SIGNAL(triggered()), this, SLOT(openFromFile()));
        connect(action_OpenReplay, SIGNAL(triggered()), this, SLOT(openReplayFromFile()));
        connect(action_SaveIcp, SIGNAL(triggered()), this, SLOT(saveIcp()));
        connect(action_SaveXml, SIGNAL(triggered()), this, SLOT(saveXml()));
        connect(action_Help, SIGNAL(triggered()), this, SLOT(help()));
}

/**
 * Create local game player vs player
 */
void MainWindow::createLocalVs() {
    Game *g = new Game(server);
    g->gameLocal();

    if (g->getError().isEmpty()) {

        GameBoard *b = new GameBoard(g);
        this->addGame(b);
    }
    else {
        setStatusMsg(g->getError());
    }
}

/**
 * Create local game player vs cpu - not fully implemented
 */
void MainWindow::createLocalCpu() {
    Game *g = new Game(server);
    GameBoard *b = new GameBoard(g);
    this->addGame(b);

    g->gameLocal(false);
	if (! g->getError().isEmpty()) {
       setStatusMsg(g->getError());
 	   removeLastGame();
    }
	
}

/**
 * Create network game player vs player
 */
void MainWindow::showNewNetDialog() {
    ConnectDialog *d = new ConnectDialog();
    connect(d, SIGNAL(dialogAccepted(QStringList)), this, SLOT(newNetworkGame(QStringList)));
    d->show();
}

/**
 * Open game from file via standard OS open dialog
 */
void MainWindow::openFromFile(){
    QFileDialog openDialog(this);
    QString filename = openDialog.getOpenFileName(this, trUtf8("Open game from file"), "", "");
    if (filename == NULL)
        return;
    Game *g = new Game(server);

    if (g->gameFromFile(filename, Player::COLOR_WHITE)) {
        GameBoard *b = new GameBoard(g);
        this->addGame(b);
        b->refresh();
    }
    else {
        setStatusMsg(g->getError());
    }

}
/**
 * Open replay file via standard OS open dialog
 */
void MainWindow::openReplayFromFile() {
    QFileDialog openDialog(this);
    QString filename = openDialog.getOpenFileName(this, trUtf8("Open replay from file"), "", "XML files(*.xml)");
    if (filename == NULL)
        return;
    Game *g = new Game(server);
    g->gameFromFile(filename);
    if (g->getError().isEmpty()) {

        GameBoard *b = new GameBoard(g);
        this->addGame(b);
        toggleReplayButtons(g);
        setLineEditText(g->getIcpSyntaxStr(true));
        b->refresh();
    }
    else {
        setStatusMsg(g->getError());
    }

}

/**
 * Save game to file with icp syntax
 */
bool MainWindow::saveIcp() {
    QString fn = QFileDialog::getSaveFileName(NULL,
                                   trUtf8("Ulozit jako ICP syntax"),
                                   QDir::homePath(),
                                   trUtf8("Soubory ICP syntaxe (%1)").arg("*.*"));

    GameBoard *b = games_arr.at(tabWidget_Games->currentIndex());
    Game *g = b->getGame();
    if (!fn.isEmpty())
    {
        QFile f(fn);
        /** overwrite file */
        f.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream fs(&f);
        fs << g->getIcpSyntaxStr();
        return true;
    }

    return false;
}

/**
 * Save game to file with xml syntax
 */
bool MainWindow::saveXml() {
    QString fn = QFileDialog::getSaveFileName(NULL,
                                   trUtf8("Ulozit jako XML"),
                                   QDir::homePath(),
                                   trUtf8("Soubory XML (%1)").arg("*.xml"));

    GameBoard *b = games_arr.at(tabWidget_Games->currentIndex());
    Game *g = b->getGame();
    if (!fn.isEmpty())
    {
        QFile f(fn);
        /** overwrite file */
        f.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream fs(&f);
        fs << g->getXmlStr();
        return true;
    }
    return false;
}

/**
 * Close tab with running game and warning user before closing
 */
void MainWindow::on_tabWidget_Games_tabCloseRequested(int index)
{
    QMessageBox::StandardButton choice;
    choice = QMessageBox::question(this,
            trUtf8("Opravdu zavrit tuto hru?"),
            trUtf8("Opravdu chcete ukoncit hru '%1'?<br><b>Neulozene zmeny budou ztraceny!</b>").arg(tabWidget_Games->tabText(index)),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No);

    if (choice == QMessageBox::Yes) {
			delete games_arr.at(index)->getGame();
            delete games_arr.at(index);
			games_arr.removeAt(index);
			moves_str.removeAt(index);
			tabWidget_Games->removeTab(index);
	}
    else
		return;
}

/**
 * Add new tab and new game
 */
void MainWindow::addGame(GameBoard * board) {
    games_arr.append(board);
    moves_str.append(QPair<GameBoard*, QString>(board, lineEdit_Moves->toPlainText()));
    QString str = trUtf8("Hra c.") + QString::number(games_arr.count());
    tabWidget_Games->addTab(board, str);
    tabWidget_Games->setCurrentIndex(tabWidget_Games->count() - 1);
    pushButton_Advice->setEnabled(true);
}

void MainWindow::removeLastGame() {
	qDebug() << "removeLastGame";
	delete games_arr.last()->getGame();
	delete games_arr.last();
	games_arr.removeLast();
	moves_str.removeLast();
	tabWidget_Games->removeTab(tabWidget_Games->count() - 1);  
}

void MainWindow::removeGame(int i) {
	qDebug() << "removeGame";
	delete games_arr.at(i)->getGame();
	delete games_arr.at(i);
	games_arr.removeAt(i);
	moves_str.removeAt(i);
	tabWidget_Games->removeTab(i);
}


/**
 * Set status mesage for 2,5s
 */
void MainWindow::setStatusMsg(const char * str) {
    myStatusBar->showMessage(QString(str), 2500);
}

/**
 * Overloaded Set status mesage for 2,5s
 */
void MainWindow::setStatusMsg(QString str) {
    myStatusBar->showMessage(str, 2500);
}

/**
 * Set moves record to right menu
 * @param Recorded moves
 */
void MainWindow::setLineEditText(const QString & str) {
    lineEdit_Moves->setPlainText(str);
}

void MainWindow::clearMoves() {
    moves.clear();
    lineEdit_Moves->clear();
}

/**
 * when current the tab is changed, notification area is filled up with proper content
 * if the game is replay, the replay tools are set
 */

void MainWindow::on_tabWidget_Games_currentChanged(int index)
{
    
	if (index != -1) {
        lineEdit_Moves->setPlainText(games_arr.at(index)->lineedit_moves);
        Game * g = games_arr.at(index)->getGame();
        toggleReplayButtons(g);
    }
    else {
        lineEdit_Moves->setPlainText(" ");
        pushButton_Advice->setEnabled(false);
    }
}

/**
 * Return number of running games
 * @return number of running games
 */
int MainWindow::getGamesCount() {
    return games_arr.count();
}

/**
 * Stop replaying
 */
void MainWindow::on_pushButton_Stop_clicked()
{
    GameBoard * b = games_arr.at(tabWidget_Games->currentIndex());
    Game * g = b->getGame();
    g->replayMoveStop();
}

/**
 * toggle replaying
 */
void MainWindow::on_pushButton_Play_clicked()
{
    GameBoard * b = games_arr.at(tabWidget_Games->currentIndex());
    Game * g = b->getGame();
    int time = spinBox_Delay->value();
    g->setReplayTimeout(time);
    g->replayMoveToggle();
}

/**
 * Rewind replaying
 */
void MainWindow::on_pushButton_Rewind_clicked()
{
    GameBoard * b = games_arr.at(tabWidget_Games->currentIndex());
    Game * g = b->getGame();
    int step = spinBox_MovesCount->value();
    g->replayMove(step, false);
}

/**
 * Pause replaying
 */
void MainWindow::on_pushButton_Pause_clicked()
{
    GameBoard * b = games_arr.at(tabWidget_Games->currentIndex());
    Game * g = b->getGame();
    int time = spinBox_Delay->value();
    g->setReplayTimeout(time);
    g->replayMoveToggle();
}

/**
 * Forward replaying
 */
void MainWindow::on_pushButton_Forward_clicked()
{
    GameBoard * b = games_arr.at(tabWidget_Games->currentIndex());
    Game * g = b->getGame();
    int step = spinBox_MovesCount->value();
    g->replayMove(step, true);
}

/**
 * Change speed of replaying
 */
void MainWindow::on_spinBox_Delay_valueChanged(int)
{
    GameBoard * b = games_arr.at(tabWidget_Games->currentIndex());
    Game * g = b->getGame();
    int time = spinBox_Delay->value();
    g->setReplayTimeout(time);

}

/**
 * Toggle replaying buttons if the game is replay
 * @param Instation of game
 */
void MainWindow::toggleReplayButtons(Game * g) {
    bool flag = g->isReplaying();
    pushButton_Stop->setEnabled(flag);
    pushButton_Play->setEnabled(flag);
    pushButton_Pause->setEnabled(flag);
    pushButton_Rewind->setEnabled(flag);
    pushButton_Forward->setEnabled(flag);
    spinBox_Delay->setEnabled(flag);
    spinBox_MovesCount->setEnabled(flag);
}

/**
 * Show help dialog
 */
void MainWindow::help() {
    Help * dialog = new Help();
    dialog->show();
}


void MainWindow::incomingConnection() {
	Game *g = new Game(server);
	gotConnection(g);
}


/**
 * Process invitation to new network game
 */

void MainWindow::gotConnection(Game * g) {
  qDebug() << "beggening of gotConnection() " << g->getState();

  connect(g, SIGNAL(gotInvite(Player::color_t, QString)), this, SLOT(gotInviteSlot(Player::color_t, QString)));
  connect(g, SIGNAL(gotExit()), this, SLOT(gotExitSlot()));

  if (server->hasPendingConnections()) {
	if (! g->gameRemote(server->nextPendingConnection())) {
	  qDebug() << "gameRemote fail";
	  setStatusMsg(g->getError());
	  return;
	}
	qDebug() << "after game remote" << g->getState();

	AcceptDialog *dialog = new AcceptDialog(g);
	connect(dialog, SIGNAL(userAccept(Game*)), this, SLOT(inviteAccepted(Game*)));
	connect(dialog, SIGNAL(userReject(Game*)), this, SLOT(inviteRejected(Game*)));
	dialog->show();
  }
  else {
	qDebug() << "hasPendingConnection fail " << g->getState();
  }
}

void MainWindow::inviteAccepted(Game * g) {
	GameBoard * b = new GameBoard(g);
	this->addGame(b);
	g->dispatchUserResponseInvite(true);
}

void MainWindow::inviteRejected(Game * g) {
	g->dispatchUserResponseInvite(false);
	delete g;
}

/**
 * Inform user about new invitation to network game
 * @param Player::color_t Player color
 */

void MainWindow::gotInviteSlot(Player::color_t color, QString str) {
    //GameBoard *b = games_arr.at(tabWidget_Games->currentIndex());
    //emit(storePlayer(color, str, b));
	
	qDebug() <<prepared_game << "gotInviteSlot " << QString::number(prepared_game == NULL).toLocal8Bit();
	if (prepared_board != NULL) {
		
		this->addGame(prepared_board);
		qDebug() << "prisel inviteSlot";
	}

	qDebug() << "prisel inviteslot";
	setStatusMsg("User accepted your invite on network game");
}

/**
 * Set status message to inform user about disconnect from game
 */
void MainWindow::gotExitSlot() {
	qDebug() << "gotExitslot";
    setStatusMsg("disconnected from remote game");
	int i = 0;
	bool found = false;
	Game * g = qobject_cast<Game *>( this->sender() );
	if (g->getState() != Game::STATE_END) {
		while(!found) {
			QWidget * w = tabWidget_Games->widget(i);
			GameBoard * b = qobject_cast<GameBoard *>(w);
			if (b->getGame() == g)
				found = true;
		}
		if (found) {
			this->removeGame(i + 1);
		}
	}
}

/**
 * Create new network game
 */

void MainWindow::newNetworkGame(QStringList list) {

    //prepared_game = new Game(server);
	Game * g = new Game(server);
	GameBoard * b  = new GameBoard(g);
	this->addGame(b);

	//qDebug() <<prepared_game << "new network game before gameRemote" << QString::number(prepared_game == NULL).toLocal8Bit();

    QHostAddress addr;
    addr.setAddress(list.at(0));

    //connect(prepared_game, SIGNAL(gotInvite(Player::color_t, QString)), this, SLOT(gotInviteSlot(Player::color_t, QString)));
    //connect(prepared_game, SIGNAL(gotExit()), this, SLOT(gotExitSlot()));

    connect(g, SIGNAL(gotInvite(Player::color_t, QString)), this, SLOT(gotInviteSlot(Player::color_t, QString)));
	connect(g, SIGNAL(gotExit()), this, SLOT(gotExitSlot()));
    
	//if (! (prepared_game->gameRemote(addr, list.at(1).toUInt(), Player::COLOR_WHITE))) {
    //    setStatusMsg(prepared_game->getError());

	if (! (g->gameRemote(addr, list.at(1).toUInt(), Player::COLOR_WHITE))) {
        setStatusMsg(g->getError());
		qDebug() << "prepared game game remote fail";
    }
	//qDebug() << prepared_game << "new netwrk game after gameRemote" << QString::number(prepared_game == NULL).toLocal8Bit();
	
}


void MainWindow::savePlayer(Player::color_t color, QString alias, GameBoard * b) {
    b->player_alias = alias;
    b->player_color = color;
}

void MainWindow::on_pushButton_Advice_clicked()
{
    Game *g = games_arr.at(tabWidget_Games->currentIndex())->getGame();
    g->adviceMove();
}
