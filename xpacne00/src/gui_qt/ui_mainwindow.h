/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu 9. May 02:13:43 2013
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Open;
    QAction *action_SaveIcp;
    QAction *action_SaveXml;
    QAction *action_Close;
    QAction *action_LocalLabel;
    QAction *action_NetLabel;
    QAction *action_NewLocalGame;
    QAction *action_NewNetGame;
    QAction *action_DisconnectGame;
    QAction *action_NewLocalGameVs;
    QAction *action_OpenReplay;
    QAction *action_Help;
    QWidget *centralWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QFrame *frame_Tools;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QPushButton *pushButton_Play;
    QPushButton *pushButton_Stop;
    QPushButton *pushButton_Pause;
    QPushButton *pushButton_Forward;
    QPushButton *pushButton_Rewind;
    QSpinBox *spinBox_Delay;
    QLabel *label;
    QSpinBox *spinBox_MovesCount;
    QLabel *label_2;
    QGroupBox *groupBox_3;
    QGroupBox *groupBox_2;
    QLabel *portLabel;
    QFrame *frame_Game;
    QTabWidget *tabWidget_Games;
    QFrame *frame_Moves;
    QGroupBox *groupBox_Moves;
    QTextEdit *lineEdit_Moves;
    QMenuBar *menuBar;
    QMenu *menuSoubor;
    QMenu *menuHra;
    QToolBar *mainToolBar;
    QStatusBar *myStatusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1060, 740);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QString::fromUtf8("action_Open"));
        action_SaveIcp = new QAction(MainWindow);
        action_SaveIcp->setObjectName(QString::fromUtf8("action_SaveIcp"));
        action_SaveXml = new QAction(MainWindow);
        action_SaveXml->setObjectName(QString::fromUtf8("action_SaveXml"));
        action_Close = new QAction(MainWindow);
        action_Close->setObjectName(QString::fromUtf8("action_Close"));
        action_LocalLabel = new QAction(MainWindow);
        action_LocalLabel->setObjectName(QString::fromUtf8("action_LocalLabel"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        action_LocalLabel->setFont(font);
        action_NetLabel = new QAction(MainWindow);
        action_NetLabel->setObjectName(QString::fromUtf8("action_NetLabel"));
        action_NetLabel->setFont(font);
        action_NewLocalGame = new QAction(MainWindow);
        action_NewLocalGame->setObjectName(QString::fromUtf8("action_NewLocalGame"));
        action_NewNetGame = new QAction(MainWindow);
        action_NewNetGame->setObjectName(QString::fromUtf8("action_NewNetGame"));
        action_DisconnectGame = new QAction(MainWindow);
        action_DisconnectGame->setObjectName(QString::fromUtf8("action_DisconnectGame"));
        action_NewLocalGameVs = new QAction(MainWindow);
        action_NewLocalGameVs->setObjectName(QString::fromUtf8("action_NewLocalGameVs"));
        action_OpenReplay = new QAction(MainWindow);
        action_OpenReplay->setObjectName(QString::fromUtf8("action_OpenReplay"));
        action_Help = new QAction(MainWindow);
        action_Help->setObjectName(QString::fromUtf8("action_Help"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setMaximumSize(QSize(16777215, 750));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(19, 19, 1011, 661));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame_Tools = new QFrame(horizontalLayoutWidget);
        frame_Tools->setObjectName(QString::fromUtf8("frame_Tools"));
        frame_Tools->setMaximumSize(QSize(150, 650));
        frame_Tools->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255)"));
        frame_Tools->setFrameShape(QFrame::StyledPanel);
        frame_Tools->setFrameShadow(QFrame::Raised);
        verticalLayoutWidget = new QWidget(frame_Tools);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 151, 631));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(verticalLayoutWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 250));
        groupBox->setMaximumSize(QSize(16777215, 240));
        groupBox->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255)"));
        pushButton_Play = new QPushButton(groupBox);
        pushButton_Play->setObjectName(QString::fromUtf8("pushButton_Play"));
        pushButton_Play->setEnabled(false);
        pushButton_Play->setGeometry(QRect(80, 40, 45, 60));
        pushButton_Play->setStyleSheet(QString::fromUtf8("border: none;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Play->setIcon(icon);
        pushButton_Play->setIconSize(QSize(45, 60));
        pushButton_Stop = new QPushButton(groupBox);
        pushButton_Stop->setObjectName(QString::fromUtf8("pushButton_Stop"));
        pushButton_Stop->setEnabled(false);
        pushButton_Stop->setGeometry(QRect(30, 40, 45, 60));
        pushButton_Stop->setStyleSheet(QString::fromUtf8("border : none;"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Stop->setIcon(icon1);
        pushButton_Stop->setIconSize(QSize(45, 60));
        pushButton_Pause = new QPushButton(groupBox);
        pushButton_Pause->setObjectName(QString::fromUtf8("pushButton_Pause"));
        pushButton_Pause->setEnabled(false);
        pushButton_Pause->setGeometry(QRect(50, 110, 40, 60));
        pushButton_Pause->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border: none;"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Pause->setIcon(icon2);
        pushButton_Pause->setIconSize(QSize(50, 60));
        pushButton_Forward = new QPushButton(groupBox);
        pushButton_Forward->setObjectName(QString::fromUtf8("pushButton_Forward"));
        pushButton_Forward->setEnabled(false);
        pushButton_Forward->setGeometry(QRect(90, 110, 60, 61));
        pushButton_Forward->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border: none;"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/forward.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Forward->setIcon(icon3);
        pushButton_Forward->setIconSize(QSize(60, 60));
        pushButton_Rewind = new QPushButton(groupBox);
        pushButton_Rewind->setObjectName(QString::fromUtf8("pushButton_Rewind"));
        pushButton_Rewind->setEnabled(false);
        pushButton_Rewind->setGeometry(QRect(1, 114, 52, 51));
        pushButton_Rewind->setMinimumSize(QSize(0, 0));
        pushButton_Rewind->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border: none;"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/rewind.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Rewind->setIcon(icon4);
        pushButton_Rewind->setIconSize(QSize(54, 57));
        spinBox_Delay = new QSpinBox(groupBox);
        spinBox_Delay->setObjectName(QString::fromUtf8("spinBox_Delay"));
        spinBox_Delay->setEnabled(false);
        spinBox_Delay->setGeometry(QRect(100, 180, 51, 22));
        spinBox_Delay->setMinimum(100);
        spinBox_Delay->setMaximum(32000);
        spinBox_Delay->setValue(600);
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(4, 180, 91, 20));
        spinBox_MovesCount = new QSpinBox(groupBox);
        spinBox_MovesCount->setObjectName(QString::fromUtf8("spinBox_MovesCount"));
        spinBox_MovesCount->setEnabled(false);
        spinBox_MovesCount->setGeometry(QRect(100, 210, 51, 22));
        spinBox_MovesCount->setMinimum(1);
        spinBox_MovesCount->setMaximum(32000);
        spinBox_MovesCount->setValue(1);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(5, 213, 91, 16));

        verticalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(verticalLayoutWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMaximumSize(QSize(16777215, 215));
        groupBox_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border: none;"));

        verticalLayout->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(verticalLayoutWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMaximumSize(QSize(16777215, 215));
        groupBox_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255)"));

        verticalLayout->addWidget(groupBox_2);

        portLabel = new QLabel(frame_Tools);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));
        portLabel->setGeometry(QRect(0, 630, 151, 20));

        horizontalLayout->addWidget(frame_Tools);

        frame_Game = new QFrame(horizontalLayoutWidget);
        frame_Game->setObjectName(QString::fromUtf8("frame_Game"));
        frame_Game->setMaximumSize(QSize(650, 650));
        frame_Game->setFrameShape(QFrame::StyledPanel);
        frame_Game->setFrameShadow(QFrame::Raised);
        tabWidget_Games = new QTabWidget(frame_Game);
        tabWidget_Games->setObjectName(QString::fromUtf8("tabWidget_Games"));
        tabWidget_Games->setGeometry(QRect(0, 0, 651, 650));
        tabWidget_Games->setMinimumSize(QSize(650, 650));
        tabWidget_Games->setMaximumSize(QSize(651, 630));
        tabWidget_Games->setStyleSheet(QString::fromUtf8(""));
        tabWidget_Games->setTabsClosable(true);

        horizontalLayout->addWidget(frame_Game);

        frame_Moves = new QFrame(horizontalLayoutWidget);
        frame_Moves->setObjectName(QString::fromUtf8("frame_Moves"));
        frame_Moves->setMaximumSize(QSize(210, 16777215));
        frame_Moves->setFrameShape(QFrame::StyledPanel);
        frame_Moves->setFrameShadow(QFrame::Raised);
        groupBox_Moves = new QGroupBox(frame_Moves);
        groupBox_Moves->setObjectName(QString::fromUtf8("groupBox_Moves"));
        groupBox_Moves->setGeometry(QRect(0, 0, 211, 650));
        groupBox_Moves->setMinimumSize(QSize(0, 650));
        groupBox_Moves->setMaximumSize(QSize(16777215, 650));
        groupBox_Moves->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border: none;"));
        lineEdit_Moves = new QTextEdit(groupBox_Moves);
        lineEdit_Moves->setObjectName(QString::fromUtf8("lineEdit_Moves"));
        lineEdit_Moves->setEnabled(false);
        lineEdit_Moves->setGeometry(QRect(10, 20, 191, 631));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Calibri"));
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        font1.setStyleStrategy(QFont::PreferAntialias);
        lineEdit_Moves->setFont(font1);
        lineEdit_Moves->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border: none;\n"
"padding-left: 20px;"));
        lineEdit_Moves->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        lineEdit_Moves->setTabChangesFocus(true);
        lineEdit_Moves->setOverwriteMode(true);

        horizontalLayout->addWidget(frame_Moves);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1060, 22));
        menuSoubor = new QMenu(menuBar);
        menuSoubor->setObjectName(QString::fromUtf8("menuSoubor"));
        menuHra = new QMenu(menuBar);
        menuHra->setObjectName(QString::fromUtf8("menuHra"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        myStatusBar = new QStatusBar(MainWindow);
        myStatusBar->setObjectName(QString::fromUtf8("myStatusBar"));
        MainWindow->setStatusBar(myStatusBar);

        menuBar->addAction(menuSoubor->menuAction());
        menuBar->addAction(menuHra->menuAction());
        menuSoubor->addAction(action_Open);
        menuSoubor->addAction(action_OpenReplay);
        menuSoubor->addAction(action_SaveIcp);
        menuSoubor->addAction(action_SaveXml);
        menuSoubor->addAction(action_Help);
        menuHra->addAction(action_LocalLabel);
        menuHra->addAction(action_NewLocalGame);
        menuHra->addAction(action_NewLocalGameVs);
        menuHra->addSeparator();
        menuHra->addAction(action_NetLabel);
        menuHra->addAction(action_NewNetGame);
        menuHra->addSeparator();

        retranslateUi(MainWindow);

        tabWidget_Games->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        action_Open->setText(QApplication::translate("MainWindow", "Otev\305\231\303\255t", 0, QApplication::UnicodeUTF8));
        action_SaveIcp->setText(QApplication::translate("MainWindow", "Ulo\305\276it ICP syntaxi", 0, QApplication::UnicodeUTF8));
        action_SaveXml->setText(QApplication::translate("MainWindow", "Ulo\305\276it XML", 0, QApplication::UnicodeUTF8));
        action_Close->setText(QApplication::translate("MainWindow", "Zav\305\231it", 0, QApplication::UnicodeUTF8));
        action_LocalLabel->setText(QApplication::translate("MainWindow", "Lok\303\241ln\303\255", 0, QApplication::UnicodeUTF8));
        action_NetLabel->setText(QApplication::translate("MainWindow", "S\303\255\305\245ov\303\241", 0, QApplication::UnicodeUTF8));
        action_NewLocalGame->setText(QApplication::translate("MainWindow", "Nov\303\241 hra proti CPU", 0, QApplication::UnicodeUTF8));
        action_NewNetGame->setText(QApplication::translate("MainWindow", "Zalo\305\276it hru proti hr\303\241\304\215i", 0, QApplication::UnicodeUTF8));
        action_DisconnectGame->setText(QApplication::translate("MainWindow", "Odpojit aktu\303\241ln\303\255 hru", 0, QApplication::UnicodeUTF8));
        action_NewLocalGameVs->setText(QApplication::translate("MainWindow", "Nov\303\241 hra proti hr\303\241\304\215i", 0, QApplication::UnicodeUTF8));
        action_OpenReplay->setText(QApplication::translate("MainWindow", "Otev\305\231\303\255t replay", 0, QApplication::UnicodeUTF8));
        action_Help->setText(QApplication::translate("MainWindow", "N\303\241pov\304\233da", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "  N\303\241stroje pro p\305\231ehr\303\241v\303\241n\303\255", 0, QApplication::UnicodeUTF8));
        pushButton_Play->setText(QString());
        pushButton_Stop->setText(QString());
        pushButton_Pause->setText(QString());
        pushButton_Forward->setText(QString());
        pushButton_Rewind->setText(QString());
        label->setText(QApplication::translate("MainWindow", "\304\215as mezi tahy [ms]", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "sko\304\215it o po\304\215et tah\305\257.", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "        N\303\241stroje pro hru", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QString());
        portLabel->setText(QApplication::translate("MainWindow", "server running on port: ", 0, QApplication::UnicodeUTF8));
        groupBox_Moves->setTitle(QApplication::translate("MainWindow", "Moves", 0, QApplication::UnicodeUTF8));
        menuSoubor->setTitle(QApplication::translate("MainWindow", "Soubor", 0, QApplication::UnicodeUTF8));
        menuHra->setTitle(QApplication::translate("MainWindow", "Hra", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
