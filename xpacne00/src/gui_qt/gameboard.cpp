/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "gameboard.h"
#include "lightrock.h"
#include "darkrock.h"
#include "canvas.h"
#include "mainwindow.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QMessageBox>

/*
 * gameboard object constructor
 *
 */

GameBoard::GameBoard(Game *parent)
        : game(parent)
{
    setupUi(this);

    /** canvas object */
    canvas = new Canvas(this);
    canvas->setSceneRect(70,65, graphicsView->width(), graphicsView->height());
    graphicsView->setScene(canvas);

    /** init game with rocks */
    initGame();

    /** connect signal from scene and game */
    connect(game, SIGNAL(refresh()), this, SLOT(refresh()));
    connect(canvas, SIGNAL(showPossibleMoves(uint,uint)), this, SLOT(showMoves(uint, uint)));
    connect(canvas, SIGNAL(sceneUpdated(uint,uint,uint,uint)), this, SLOT(sceneUpdate(uint, uint, uint, uint)));
}

GameBoard::~GameBoard(){
    if (canvas)
        delete canvas;
}

/**
 * function initialize gameboard with rocks and set their position in scene
 * rocks can be only at black positions, top left position is white
 */

void GameBoard::initGame(){

    for (int i = 5; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (i %2 == 0) {
                if (j % 2 == 1) {
                    DarkRock *dark_rock = new DarkRock();
                    canvas->addItem(dark_rock);
                    dark_rock->setPos(67.5 + j * 70.0 + 10.0, 67.5 + i * 70.0 + 10.0);
                    dark_rocks.append(dark_rock);
                }
            }
            else {
                if (j % 2 == 0) {
                    DarkRock *dark_rock = new DarkRock();
                    canvas->addItem(dark_rock);
                    dark_rock->setPos(67.5 + j * 70.0 + 10.0, 67.5 + i * 70.0 + 10.0);
                    dark_rocks.append(dark_rock);
                }
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 8; j++) {
            if (i % 2 == 1) {
                if (j % 2 == 0) {
                    LightRock *light_rock = new LightRock();
                    canvas->addItem(light_rock);
                    light_rock->setPos(67.5 + j * 70.0 + 10.0, 67.5 + i * 70.0 + 10.0);
                    light_rocks.append(light_rock);
                }
            }
            else {
                if (j % 2 == 1) {
                    LightRock *light_rock = new LightRock();
                    canvas->addItem(light_rock);
                    light_rock->setPos(67.5 + j * 70.0 + 10.0, 67.5 + i * 70.0 + 10.0);
                    light_rocks.append(light_rock);
                }
            }
        }
    }
}

/**
 * remove rock by the given scene position and check if some rocks left
 * otherwise show message that user win
 */

void GameBoard::removeRock(QPointF position) {
    QPointF rock_pos;
    for (int i = 0; i < light_rocks.size(); i++) {
        rock_pos = light_rocks.at(i)->scenePos();
        if (rock_pos.x() == position.x() && rock_pos.y() == position.y()) {
            light_rocks.removeAt(i);
            if (light_rocks.count() == 0) {
                // black player win
                QMessageBox::information(this, "Black player won", "Cerny hrac vyhral!");
            }
            return;
        }
    }
    for (int i = 0; i < dark_rocks.size(); i++) {
        rock_pos = dark_rocks.at(i)->scenePos();
        if (rock_pos.x() == position.x() && rock_pos.y() == position.y()) {
            dark_rocks.removeAt(i);
            if (dark_rocks.count() == 0) {
                // black player win
                QMessageBox::information(this, "White player won", "Bily hrac vyhral!");
            }
            return;
        }
    }
}

/**
 * function covert given list of coordinations in board to their position in scene
 * @param list of coordinates in board
 * $return list of coordinates in scene
 */
QList<QRectF> GameBoard::convertCoords(QList<QPair<int, int> >list) {
    // it´s expected, that int is in rage between 0 and 7
    QList<QRectF> ret;
    for (int i = 0; i < list.size(); i++) {
        QPair<int, int> item = list.at(i);
        QLayoutItem *layout_item = gameGridLayout->itemAtPosition(7 - item.second + 1, item.first + 1);
        ret.append(QRectF(layout_item->geometry().x(), layout_item->geometry().y(), 50, 50));
    }
    return ret;
}
/**
 * function covert given list of coordinates in scene to their position in board
 * @param list of coordinates in scene
 * $return list of coordinates in board
 */
QList<QPair<uint,uint> > GameBoard::convertCoords(QList<QPointF> list) {

    QList<QPair<uint,uint> > ret;
    for (int i = 0; i < list.size(); i++) {
        QPointF pos = list.at(i);
        uint x = (pos.x() - 10 - 67.5)/70;
        uint y = (pos.y() - 10 - 67.5)/70;

        ret.append(QPair<uint,uint>(x + 1,y + 1));
    }
    return ret;
}


/**
 * Convert coordinates from <1..8,1..8> format to pixel coordinates format, size of array is 70x70
 * @param coordinates in <1..8,1..8> format
 * @return Return the center of array in pixel coordinates
 */
QList<QPointF> GameBoard::convertCoords(QList<QPair<uint, uint> > list){
    QList<QPointF> ret;
    double x,y;

    for (int i = 0; i < list.size(); i++) {
        QPair<uint, uint> item = list.at(i);
        /** multiply position by size of array and shift to center of array */
        x = item.first*70 + 35;
        y = item.second*70 + 35;

        ret.append(QPointF(x, y));
    }
    return ret;
};
/**
 * function covert given coordinates in board to their position in scene
 * @param coordinates in one rock in the board
 * $return l the rock´s coordinates in scene
 */
QPointF GameBoard::convertCoords(uint x, uint y){
    return QPointF(x*70 + 10, y*70 + 10);
};

/**
 * Convert coordinates from <1..8,1..8> format to pixel coordinates format, size of array is 70x70
 * @param coordinates in <1..8,1..8> format
 * @return Return the center of array in pixel coordinates
 */
QList<QPointF> GameBoard::convertCoords(QPair<uint, uint> src, QPair<uint, uint> dest){
    QList<QPointF> ret;

    // multiply position by size of array and shift to center of array
    ret.append(QPointF(src.first*70 + 35, src.second*70 + 35));
    ret.append(QPointF(dest.first*70 + 35, dest.second*70 + 35));

    return ret;
};


/**
 * function that lighten given list or positions in board - possible moves
 * @param list of positions in board
 * @return Return the center of array in pixel coordinates
 */
void GameBoard::lightenPossibleMoves(QList<QPair<int, int> >positions) {
    QString str = "abcdefgh";
    for (int i = 0; i < positions.count(); i++) {
        QPair<int, int> item = positions.at(i);

        /** get item in layout */
        QLayoutItem *layout_item = gameGridLayout->itemAtPosition(item.first + 1, item.second + 1);
        //qDebug() << layout_item->geometry();
        /** return it as widget and change the stylesheet afterwards*/
        QWidget * widget = layout_item->widget();
        widget->setStyleSheet("background-color: rgba(227, 224, 119, 100)");
        /** store possible move */
        possible_moves.append(QPair<QString, int>(QString(str[item.first]), item.second));
    }
}

/**
 * function that sets area´a background color back
 */
void GameBoard::hidePossibleMoves() {
    for (int i = 0; i < gameGridLayout->rowCount(); i++) {
        for (int j = 0; j < gameGridLayout->columnCount(); j++) {
            if (game->isBlackBox(i + 1, j + 1)) {
                QLayoutItem *item = gameGridLayout->itemAtPosition(i + 1,j + 1);
                if (item != 0) {
                    QWidget * widget = item->widget();
                    widget->setStyleSheet("background-color:rgb(0, 0, 0)");
                }
            }
        }
    }
}

/**
 * function that refresh the scene
 */
void GameBoard::refresh() {
    //qDebug() << "gameboard refreshed";
    QList<QPair<uint, uint> >list;
    QString debug_str;
    QPointF point;
    QList<QPair<int,int> > pos_moves;
    clearBoard();
    /** if some possible moves is showed , hide them... */
    if (possible_moves.count() != 0)
        hidePossibleMoves();


    /** if the move is valid */
    //if (game->getError().isEmpty()) {
        /** check if some possible moves are on the core board */
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (game->board[i][j] == Game::MEN_POSSIBLE_MOVE) {
                    pos_moves.append(QPair<int,int>(i,j));
                }
            }
        }
        /** if there are some possible moves */
        if (pos_moves.count() != 0) {
            /** ligthen them up and set timer on darkening */
            lightenPossibleMoves(pos_moves);
            //QTimer::singleShot(2500, this, SLOT(toutExpired()));
        }
        /** pass the entire board */
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
               /** light rock detected */
                if (game->board[i][j] == Game::MEN_WHITE) {
                    debug_str += "white rock\t";
                    LightRock * light = new LightRock();
                    light_rocks.append(light);
                    point = this->convertCoords(j,i);
                    canvas->addItem(light);
                    light->setPos(67.5 + point.x(), 67.5 + point.y());

                }
                /** dark rock detected */
                else if (game->board[i][j] == Game::MEN_BLACK) {
                    debug_str += "black rock\t";
                    DarkRock * dark = new DarkRock();
                    dark_rocks.append(dark);
                    point = this->convertCoords(j,i);
                    canvas->addItem(dark);
                    dark->setPos(67.5 + point.x(), 67.5 + point.y());
                }
                /** light king */
                else if (game->board[i][j] == Game::MEN_WHITE_KING) {
                    debug_str += "white king\t";
                    LightRock * light = new LightRock();
                    light_rocks.append(light);
                    point = this->convertCoords(j,i);
                    canvas->addItem(light);
                    light->setPos(67.5 + point.x(), 67.5 + point.y());
                    light->becomeKing();

                }
                /** dark king */
                else if (game->board[i][j] == Game::MEN_BLACK_KING) {
                    debug_str += "black king\t";
                    DarkRock * dark = new DarkRock();
                    dark_rocks.append(dark);
                    point = this->convertCoords(j,i);
                    canvas->addItem(dark);
                    dark->setPos(67.5 + point.x(), 67.5 + point.y());
                    dark->becomeKing();
                }
                /** possible moves are already processed */
                else if (game->board[i][j] == Game::MEN_POSSIBLE_MOVE) {
                    debug_str += "possible move\t";
                }
                else {
                    debug_str += "nothing\t";
                }
            }
            //qDebug() << debug_str;
            debug_str = "";
        }
        /** set content of moves notification area */
        lineedit_moves = game->getIcpSyntaxStr(false);
        MainWindow::getInstance()->setLineEditText(lineedit_moves);
    //}
	//checkForEmptyBoard();
}

/**
 * function that clear the board from rocks
 */
void GameBoard::clearBoard() {
    for (int i = 0; i < light_rocks.count(); i++) {
        delete light_rocks.at(i);
    }
    light_rocks.clear();
    for (int i = 0; i <dark_rocks.count(); i++) {
        delete dark_rocks.at(i);
    }
    dark_rocks.clear();
}

/**
 * function that shift coordinates from layout to board
 */
void GameBoard::showMoves(uint x, uint y) {
    game->showPossibleMoves(x - 1,y - 1);
}

/**
 * store source and destination of move
 */
void GameBoard::storeMove(uint srcx, uint srcy, uint destx, uint desty) {
    stored_src = QPair<uint,uint>(srcx, srcy);
    stored_dest = QPair<uint,uint>(destx, desty);
}

/**
 * function that compares current and previous board state
 * @return return true is states are equal
 */
bool GameBoard::compareWithLastState() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ( stored_board_state[i][j] != game->board[i][j]) {
                return false;
            }
        }
    }
    return true;
}

/**
 * slot called when the scene updates coordinates of user clicks
 */
void GameBoard::sceneUpdate(uint srcx, uint srcy, uint destx, uint desty) {
    /** proceed the move */
    Game::err_t ret = game->move(srcx, srcy, destx, desty);
    /** if move is not valid, set the status message to its error */
    if (ret != Game::ERR_OK) {
        MainWindow::getInstance()->setStatusMsg(game->getError());
        //MainWindow::getInstance()->setStatusMsg("error");
    }
}
/**
 * slot called when the timeout expire
 */
void GameBoard::toutExpired() {
    game->hidePossibleMoves();
    hidePossibleMoves();
}

void GameBoard::checkForEmptyBoard() {
	if (canvas->items().count() == 0) {
		this->refresh();
		qDebug() << "prazdno";
	}
}
