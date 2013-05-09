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
    setupUi(this);
    setWindowTitle(qApp->applicationName());
    setupActions();
    myStatusBar->setStyleSheet("padding-bottom: 15px");

    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(gotConnection()));
    if (! server->listen(QHostAddress::Any, 0))
        setStatusMsg("TCP server is not listening");
    else
        portLabel->setText(portLabel->text() + QString::number(server->serverPort()));
}

MainWindow* MainWindow::getInstance() {
    if (!instance)
        instance = (MainWindow *) qApp->activeWindow();

    return instance;
}

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
    //QPoint point = game->getItemCenterPos(0,0);
    //QString test = QString::number(point.x()) + QString::number(point.y());
        GameBoard *b = new GameBoard(g);
        this->addGame(b);
        QString str = trUtf8("Hra c.") + QString::number(games_arr.count());
        tabWidget_Games->addTab(b, str);
        tabWidget_Games->setCurrentIndex(tabWidget_Games->count() - 1);
    }
    else {
        setStatusMsg(g->getError());
    }
}

/**
 * Create local game player vs cpu - not fully implented, removed
 */
void MainWindow::createLocalCpu() {
    //Game *g = new Game(server);
    //g->gameLocal(true, true);
}

/**
 * Create network game player vs player
 */
void MainWindow::showNewNetDialog() {
    ConnectDialog *d = new ConnectDialog();
    connect(d, SIGNAL(dialogAccepted(QStringList)), this, SLOT(newNetworkGame(QStringList)));
    d->exec();

}

/**
 * Open game from file via standard OS open dialog
 */
void MainWindow::openFromFile(){
    QFileDialog openDialog(this);
    QString filename = openDialog.getOpenFileName(this, trUtf8("Open game from file"), "", "XML | ICP files(*.xml *.*)");
    if (filename == NULL)
        return;
    Game *g = new Game(server);

    if (g->gameFromFile(filename, Player::COLOR_DONT_KNOW)) {
        GameBoard *b = new GameBoard(g);
        this->addGame(b);
        QString str = trUtf8("Hra c.") + QString::number(games_arr.count());
        tabWidget_Games->addTab(b, str);
        tabWidget_Games->setCurrentIndex(tabWidget_Games->count() - 1);
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
        QString str = trUtf8("Hra c.") + QString::number(games_arr.count());
        tabWidget_Games->addTab(b, str);
        tabWidget_Games->setCurrentIndex(tabWidget_Games->count() - 1);
        toggleReplayButtons(g);
        MainWindow::setLineEditText(g->getIcpSyntaxStr(true));
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
        // icp g->getIcpSyntaxStr()
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

    if (choice == QMessageBox::Yes)
            tabWidget_Games->removeTab(index);
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
void MainWindow::setLineEditText(QString str) {
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

/**
 * Process invitation to new network game
 */

void MainWindow::gotConnection() {
    if (server->hasPendingConnections()) { 
        Game *g = new Game(server);
        GameBoard *b = new GameBoard(g);
        this->addGame(b);
        QString str = trUtf8("Hra c.") + QString::number(games_arr.count());
        tabWidget_Games->addTab(b, str);
        tabWidget_Games->setCurrentIndex(tabWidget_Games->count() - 1);
        g->gameRemote(server->nextPendingConnection());
    }
}

/**
 * Inform user about new invitation to network game
 * @param Player::color_t Player color
 */

void MainWindow::gotInviteSlot(Player::color_t color, QString str) {
    AcceptDialog * dialog = new AcceptDialog();
    //connect(dialog, SIGNAL(accepted()), this, SLOT(gotInviteSlot(color, str)));
    dialog->exec();
}

/**
 * Set status message to inform user about disconnected from game
 */

void MainWindow::gotExitSlot() {
    setStatusMsg("disconnected from game");
}

/**
 * Create new network game
 */

void MainWindow::newNetworkGame(QStringList list) {
    Game * g = new Game(server);
    QHostAddress addr;
    addr.setAddress(list.at(0));
    connect(g, SIGNAL(gotInvite(Player::color_t, QString)), this, SLOT(gotInviteSlot(Player::color_t,QString)));
    connect(g, SIGNAL(gotExit()), this, SLOT(gotExitSlot()));
    if ( !g->gameRemote(addr, list.at(1).toInt(), Player::COLOR_WHITE)) {
        setStatusMsg(g->getError());
    }
}
