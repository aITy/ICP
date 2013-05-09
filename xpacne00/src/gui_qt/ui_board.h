/********************************************************************************
** Form generated from reading UI file 'board.ui'
**
** Created: Thu 9. May 02:58:44 2013
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOARD_H
#define UI_BOARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form_Board
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gameGridLayout;
    QLabel *label_nothing;
    QLabel *label_col_a;
    QLabel *label_col_b;
    QLabel *label_col_c;
    QLabel *label_col_d;
    QLabel *label_col_e;
    QLabel *label_col_f;
    QLabel *label_col_g;
    QLabel *label_col_h;
    QLabel *label_row_8;
    QLabel *label_row_7;
    QLabel *label_raw_6;
    QLabel *label_row_5;
    QLabel *label_row_4;
    QLabel *label_row_3;
    QLabel *label_row_2;
    QLabel *label_row_1;
    QWidget *widget_a8;
    QWidget *widget_b8;
    QWidget *widget_c8;
    QWidget *widget_d8;
    QWidget *widget_e8;
    QWidget *widget_f8;
    QWidget *widget_g8;
    QWidget *widget_h8;
    QWidget *widget_a7;
    QWidget *widget_b7;
    QWidget *widget_c7;
    QWidget *widget_e7;
    QWidget *widget_f7;
    QWidget *widget_g7;
    QWidget *widget_h7;
    QWidget *widget_a6;
    QWidget *widget_b6;
    QWidget *widget_c6;
    QWidget *widget_d6;
    QWidget *widget_h5;
    QWidget *widget_a1;
    QWidget *widget_a2;
    QWidget *widget_a3;
    QWidget *widget_a4;
    QWidget *widget_a5;
    QWidget *widget_b1;
    QWidget *widget_b2;
    QWidget *widget_b3;
    QWidget *widget_b4;
    QWidget *widget_b5;
    QWidget *widget_c2;
    QWidget *widget_c3;
    QWidget *widget_c5;
    QWidget *widget_c4;
    QWidget *widget_d1;
    QWidget *widget_d2;
    QWidget *widget_d3;
    QWidget *widget_d5;
    QWidget *widget_d4;
    QWidget *widget_e1;
    QWidget *widget_e2;
    QWidget *widget_e3;
    QWidget *widget_e4;
    QWidget *widget_e5;
    QWidget *widget_f1;
    QWidget *widget_e6;
    QWidget *widget_f2;
    QWidget *widget_f3;
    QWidget *widget_f4;
    QWidget *widget_f5;
    QWidget *widget_f6;
    QWidget *widget_g1;
    QWidget *widget_g3;
    QWidget *widget_g2;
    QWidget *widget_g4;
    QWidget *widget_g5;
    QWidget *widget_g6;
    QWidget *widget_h1;
    QWidget *widget_h2;
    QWidget *widget_h3;
    QWidget *widget_h4;
    QWidget *widget_h6;
    QWidget *widget_d7;
    QWidget *widget_c1;
    QGraphicsView *graphicsView;

    void setupUi(QWidget *Form_Board)
    {
        if (Form_Board->objectName().isEmpty())
            Form_Board->setObjectName(QString::fromUtf8("Form_Board"));
        Form_Board->resize(630, 630);
        Form_Board->setMinimumSize(QSize(630, 630));
        Form_Board->setMaximumSize(QSize(630, 630));
        gridLayoutWidget = new QWidget(Form_Board);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(-1, -11, 632, 641));
        gameGridLayout = new QGridLayout(gridLayoutWidget);
        gameGridLayout->setSpacing(0);
        gameGridLayout->setObjectName(QString::fromUtf8("gameGridLayout"));
        gameGridLayout->setContentsMargins(0, 0, 0, 0);
        label_nothing = new QLabel(gridLayoutWidget);
        label_nothing->setObjectName(QString::fromUtf8("label_nothing"));
        label_nothing->setMinimumSize(QSize(70, 70));
        label_nothing->setMaximumSize(QSize(70, 70));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_nothing->setFont(font);
        label_nothing->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_nothing, 0, 0, 1, 1);

        label_col_a = new QLabel(gridLayoutWidget);
        label_col_a->setObjectName(QString::fromUtf8("label_col_a"));
        label_col_a->setMinimumSize(QSize(70, 70));
        label_col_a->setMaximumSize(QSize(70, 70));
        label_col_a->setFont(font);
        label_col_a->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_col_a, 0, 1, 1, 1);

        label_col_b = new QLabel(gridLayoutWidget);
        label_col_b->setObjectName(QString::fromUtf8("label_col_b"));
        label_col_b->setMinimumSize(QSize(70, 70));
        label_col_b->setMaximumSize(QSize(70, 70));
        label_col_b->setFont(font);
        label_col_b->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_col_b, 0, 2, 1, 1);

        label_col_c = new QLabel(gridLayoutWidget);
        label_col_c->setObjectName(QString::fromUtf8("label_col_c"));
        label_col_c->setMinimumSize(QSize(70, 70));
        label_col_c->setMaximumSize(QSize(70, 70));
        label_col_c->setFont(font);
        label_col_c->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_col_c, 0, 3, 1, 1);

        label_col_d = new QLabel(gridLayoutWidget);
        label_col_d->setObjectName(QString::fromUtf8("label_col_d"));
        label_col_d->setMinimumSize(QSize(70, 70));
        label_col_d->setMaximumSize(QSize(70, 70));
        label_col_d->setFont(font);
        label_col_d->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_col_d, 0, 4, 1, 1);

        label_col_e = new QLabel(gridLayoutWidget);
        label_col_e->setObjectName(QString::fromUtf8("label_col_e"));
        label_col_e->setMinimumSize(QSize(70, 70));
        label_col_e->setMaximumSize(QSize(70, 70));
        label_col_e->setFont(font);
        label_col_e->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_col_e, 0, 5, 1, 1);

        label_col_f = new QLabel(gridLayoutWidget);
        label_col_f->setObjectName(QString::fromUtf8("label_col_f"));
        label_col_f->setMinimumSize(QSize(70, 70));
        label_col_f->setMaximumSize(QSize(70, 70));
        label_col_f->setFont(font);
        label_col_f->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_col_f, 0, 6, 1, 1);

        label_col_g = new QLabel(gridLayoutWidget);
        label_col_g->setObjectName(QString::fromUtf8("label_col_g"));
        label_col_g->setMinimumSize(QSize(70, 70));
        label_col_g->setMaximumSize(QSize(70, 70));
        label_col_g->setFont(font);
        label_col_g->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_col_g, 0, 7, 1, 1);

        label_col_h = new QLabel(gridLayoutWidget);
        label_col_h->setObjectName(QString::fromUtf8("label_col_h"));
        label_col_h->setMinimumSize(QSize(70, 70));
        label_col_h->setMaximumSize(QSize(70, 70));
        label_col_h->setFont(font);
        label_col_h->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_col_h, 0, 8, 1, 1);

        label_row_8 = new QLabel(gridLayoutWidget);
        label_row_8->setObjectName(QString::fromUtf8("label_row_8"));
        label_row_8->setMinimumSize(QSize(70, 70));
        label_row_8->setMaximumSize(QSize(70, 70));
        label_row_8->setFont(font);
        label_row_8->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_row_8, 1, 0, 1, 1);

        label_row_7 = new QLabel(gridLayoutWidget);
        label_row_7->setObjectName(QString::fromUtf8("label_row_7"));
        label_row_7->setMinimumSize(QSize(70, 70));
        label_row_7->setMaximumSize(QSize(70, 70));
        label_row_7->setFont(font);
        label_row_7->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_row_7, 2, 0, 1, 1);

        label_raw_6 = new QLabel(gridLayoutWidget);
        label_raw_6->setObjectName(QString::fromUtf8("label_raw_6"));
        label_raw_6->setMinimumSize(QSize(70, 70));
        label_raw_6->setMaximumSize(QSize(70, 70));
        label_raw_6->setFont(font);
        label_raw_6->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_raw_6, 3, 0, 1, 1);

        label_row_5 = new QLabel(gridLayoutWidget);
        label_row_5->setObjectName(QString::fromUtf8("label_row_5"));
        label_row_5->setMinimumSize(QSize(70, 70));
        label_row_5->setMaximumSize(QSize(70, 70));
        label_row_5->setFont(font);
        label_row_5->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_row_5, 4, 0, 1, 1);

        label_row_4 = new QLabel(gridLayoutWidget);
        label_row_4->setObjectName(QString::fromUtf8("label_row_4"));
        label_row_4->setMinimumSize(QSize(70, 70));
        label_row_4->setMaximumSize(QSize(70, 70));
        label_row_4->setFont(font);
        label_row_4->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_row_4, 5, 0, 1, 1);

        label_row_3 = new QLabel(gridLayoutWidget);
        label_row_3->setObjectName(QString::fromUtf8("label_row_3"));
        label_row_3->setMinimumSize(QSize(70, 70));
        label_row_3->setMaximumSize(QSize(70, 70));
        label_row_3->setFont(font);
        label_row_3->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_row_3, 6, 0, 1, 1);

        label_row_2 = new QLabel(gridLayoutWidget);
        label_row_2->setObjectName(QString::fromUtf8("label_row_2"));
        label_row_2->setMinimumSize(QSize(70, 70));
        label_row_2->setMaximumSize(QSize(70, 70));
        label_row_2->setFont(font);
        label_row_2->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_row_2, 7, 0, 1, 1);

        label_row_1 = new QLabel(gridLayoutWidget);
        label_row_1->setObjectName(QString::fromUtf8("label_row_1"));
        label_row_1->setMinimumSize(QSize(70, 70));
        label_row_1->setMaximumSize(QSize(70, 70));
        label_row_1->setFont(font);
        label_row_1->setAlignment(Qt::AlignCenter);

        gameGridLayout->addWidget(label_row_1, 8, 0, 1, 1);

        widget_a8 = new QWidget(gridLayoutWidget);
        widget_a8->setObjectName(QString::fromUtf8("widget_a8"));
        widget_a8->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_a8, 1, 1, 1, 1);

        widget_b8 = new QWidget(gridLayoutWidget);
        widget_b8->setObjectName(QString::fromUtf8("widget_b8"));
        widget_b8->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_b8, 1, 2, 1, 1);

        widget_c8 = new QWidget(gridLayoutWidget);
        widget_c8->setObjectName(QString::fromUtf8("widget_c8"));
        widget_c8->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_c8, 1, 3, 1, 1);

        widget_d8 = new QWidget(gridLayoutWidget);
        widget_d8->setObjectName(QString::fromUtf8("widget_d8"));
        widget_d8->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_d8, 1, 4, 1, 1);

        widget_e8 = new QWidget(gridLayoutWidget);
        widget_e8->setObjectName(QString::fromUtf8("widget_e8"));
        widget_e8->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_e8, 1, 5, 1, 1);

        widget_f8 = new QWidget(gridLayoutWidget);
        widget_f8->setObjectName(QString::fromUtf8("widget_f8"));
        widget_f8->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_f8, 1, 6, 1, 1);

        widget_g8 = new QWidget(gridLayoutWidget);
        widget_g8->setObjectName(QString::fromUtf8("widget_g8"));
        widget_g8->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_g8, 1, 7, 1, 1);

        widget_h8 = new QWidget(gridLayoutWidget);
        widget_h8->setObjectName(QString::fromUtf8("widget_h8"));
        widget_h8->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_h8, 1, 8, 1, 1);

        widget_a7 = new QWidget(gridLayoutWidget);
        widget_a7->setObjectName(QString::fromUtf8("widget_a7"));
        widget_a7->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_a7, 2, 1, 1, 1);

        widget_b7 = new QWidget(gridLayoutWidget);
        widget_b7->setObjectName(QString::fromUtf8("widget_b7"));
        widget_b7->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_b7, 2, 2, 1, 1);

        widget_c7 = new QWidget(gridLayoutWidget);
        widget_c7->setObjectName(QString::fromUtf8("widget_c7"));
        widget_c7->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_c7, 2, 3, 1, 1);

        widget_e7 = new QWidget(gridLayoutWidget);
        widget_e7->setObjectName(QString::fromUtf8("widget_e7"));
        widget_e7->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_e7, 2, 5, 1, 1);

        widget_f7 = new QWidget(gridLayoutWidget);
        widget_f7->setObjectName(QString::fromUtf8("widget_f7"));
        widget_f7->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_f7, 2, 6, 1, 1);

        widget_g7 = new QWidget(gridLayoutWidget);
        widget_g7->setObjectName(QString::fromUtf8("widget_g7"));
        widget_g7->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_g7, 2, 7, 1, 1);

        widget_h7 = new QWidget(gridLayoutWidget);
        widget_h7->setObjectName(QString::fromUtf8("widget_h7"));
        widget_h7->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_h7, 2, 8, 1, 1);

        widget_a6 = new QWidget(gridLayoutWidget);
        widget_a6->setObjectName(QString::fromUtf8("widget_a6"));
        widget_a6->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_a6, 3, 1, 1, 1);

        widget_b6 = new QWidget(gridLayoutWidget);
        widget_b6->setObjectName(QString::fromUtf8("widget_b6"));
        widget_b6->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_b6, 3, 2, 1, 1);

        widget_c6 = new QWidget(gridLayoutWidget);
        widget_c6->setObjectName(QString::fromUtf8("widget_c6"));
        widget_c6->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_c6, 3, 3, 1, 1);

        widget_d6 = new QWidget(gridLayoutWidget);
        widget_d6->setObjectName(QString::fromUtf8("widget_d6"));
        widget_d6->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_d6, 3, 4, 1, 1);

        widget_h5 = new QWidget(gridLayoutWidget);
        widget_h5->setObjectName(QString::fromUtf8("widget_h5"));
        widget_h5->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_h5, 4, 8, 1, 1);

        widget_a1 = new QWidget(gridLayoutWidget);
        widget_a1->setObjectName(QString::fromUtf8("widget_a1"));
        widget_a1->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_a1, 8, 1, 1, 1);

        widget_a2 = new QWidget(gridLayoutWidget);
        widget_a2->setObjectName(QString::fromUtf8("widget_a2"));
        widget_a2->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_a2, 7, 1, 1, 1);

        widget_a3 = new QWidget(gridLayoutWidget);
        widget_a3->setObjectName(QString::fromUtf8("widget_a3"));
        widget_a3->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_a3, 6, 1, 1, 1);

        widget_a4 = new QWidget(gridLayoutWidget);
        widget_a4->setObjectName(QString::fromUtf8("widget_a4"));
        widget_a4->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_a4, 5, 1, 1, 1);

        widget_a5 = new QWidget(gridLayoutWidget);
        widget_a5->setObjectName(QString::fromUtf8("widget_a5"));
        widget_a5->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_a5, 4, 1, 1, 1);

        widget_b1 = new QWidget(gridLayoutWidget);
        widget_b1->setObjectName(QString::fromUtf8("widget_b1"));
        widget_b1->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_b1, 8, 2, 1, 1);

        widget_b2 = new QWidget(gridLayoutWidget);
        widget_b2->setObjectName(QString::fromUtf8("widget_b2"));
        widget_b2->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_b2, 7, 2, 1, 1);

        widget_b3 = new QWidget(gridLayoutWidget);
        widget_b3->setObjectName(QString::fromUtf8("widget_b3"));
        widget_b3->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_b3, 6, 2, 1, 1);

        widget_b4 = new QWidget(gridLayoutWidget);
        widget_b4->setObjectName(QString::fromUtf8("widget_b4"));
        widget_b4->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_b4, 5, 2, 1, 1);

        widget_b5 = new QWidget(gridLayoutWidget);
        widget_b5->setObjectName(QString::fromUtf8("widget_b5"));
        widget_b5->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_b5, 4, 2, 1, 1);

        widget_c2 = new QWidget(gridLayoutWidget);
        widget_c2->setObjectName(QString::fromUtf8("widget_c2"));
        widget_c2->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_c2, 7, 3, 1, 1);

        widget_c3 = new QWidget(gridLayoutWidget);
        widget_c3->setObjectName(QString::fromUtf8("widget_c3"));
        widget_c3->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_c3, 6, 3, 1, 1);

        widget_c5 = new QWidget(gridLayoutWidget);
        widget_c5->setObjectName(QString::fromUtf8("widget_c5"));
        widget_c5->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_c5, 4, 3, 1, 1);

        widget_c4 = new QWidget(gridLayoutWidget);
        widget_c4->setObjectName(QString::fromUtf8("widget_c4"));
        widget_c4->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_c4, 5, 3, 1, 1);

        widget_d1 = new QWidget(gridLayoutWidget);
        widget_d1->setObjectName(QString::fromUtf8("widget_d1"));
        widget_d1->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_d1, 8, 4, 1, 1);

        widget_d2 = new QWidget(gridLayoutWidget);
        widget_d2->setObjectName(QString::fromUtf8("widget_d2"));
        widget_d2->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_d2, 7, 4, 1, 1);

        widget_d3 = new QWidget(gridLayoutWidget);
        widget_d3->setObjectName(QString::fromUtf8("widget_d3"));
        widget_d3->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_d3, 6, 4, 1, 1);

        widget_d5 = new QWidget(gridLayoutWidget);
        widget_d5->setObjectName(QString::fromUtf8("widget_d5"));
        widget_d5->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_d5, 4, 4, 1, 1);

        widget_d4 = new QWidget(gridLayoutWidget);
        widget_d4->setObjectName(QString::fromUtf8("widget_d4"));
        widget_d4->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_d4, 5, 4, 1, 1);

        widget_e1 = new QWidget(gridLayoutWidget);
        widget_e1->setObjectName(QString::fromUtf8("widget_e1"));
        widget_e1->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_e1, 8, 5, 1, 1);

        widget_e2 = new QWidget(gridLayoutWidget);
        widget_e2->setObjectName(QString::fromUtf8("widget_e2"));
        widget_e2->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_e2, 7, 5, 1, 1);

        widget_e3 = new QWidget(gridLayoutWidget);
        widget_e3->setObjectName(QString::fromUtf8("widget_e3"));
        widget_e3->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_e3, 6, 5, 1, 1);

        widget_e4 = new QWidget(gridLayoutWidget);
        widget_e4->setObjectName(QString::fromUtf8("widget_e4"));
        widget_e4->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_e4, 5, 5, 1, 1);

        widget_e5 = new QWidget(gridLayoutWidget);
        widget_e5->setObjectName(QString::fromUtf8("widget_e5"));
        widget_e5->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_e5, 4, 5, 1, 1);

        widget_f1 = new QWidget(gridLayoutWidget);
        widget_f1->setObjectName(QString::fromUtf8("widget_f1"));
        widget_f1->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_f1, 8, 6, 1, 1);

        widget_e6 = new QWidget(gridLayoutWidget);
        widget_e6->setObjectName(QString::fromUtf8("widget_e6"));
        widget_e6->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_e6, 3, 5, 1, 1);

        widget_f2 = new QWidget(gridLayoutWidget);
        widget_f2->setObjectName(QString::fromUtf8("widget_f2"));
        widget_f2->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_f2, 7, 6, 1, 1);

        widget_f3 = new QWidget(gridLayoutWidget);
        widget_f3->setObjectName(QString::fromUtf8("widget_f3"));
        widget_f3->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_f3, 6, 6, 1, 1);

        widget_f4 = new QWidget(gridLayoutWidget);
        widget_f4->setObjectName(QString::fromUtf8("widget_f4"));
        widget_f4->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_f4, 5, 6, 1, 1);

        widget_f5 = new QWidget(gridLayoutWidget);
        widget_f5->setObjectName(QString::fromUtf8("widget_f5"));
        widget_f5->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_f5, 4, 6, 1, 1);

        widget_f6 = new QWidget(gridLayoutWidget);
        widget_f6->setObjectName(QString::fromUtf8("widget_f6"));
        widget_f6->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_f6, 3, 6, 1, 1);

        widget_g1 = new QWidget(gridLayoutWidget);
        widget_g1->setObjectName(QString::fromUtf8("widget_g1"));
        widget_g1->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_g1, 8, 7, 1, 1);

        widget_g3 = new QWidget(gridLayoutWidget);
        widget_g3->setObjectName(QString::fromUtf8("widget_g3"));
        widget_g3->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_g3, 6, 7, 1, 1);

        widget_g2 = new QWidget(gridLayoutWidget);
        widget_g2->setObjectName(QString::fromUtf8("widget_g2"));
        widget_g2->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_g2, 7, 7, 1, 1);

        widget_g4 = new QWidget(gridLayoutWidget);
        widget_g4->setObjectName(QString::fromUtf8("widget_g4"));
        widget_g4->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_g4, 5, 7, 1, 1);

        widget_g5 = new QWidget(gridLayoutWidget);
        widget_g5->setObjectName(QString::fromUtf8("widget_g5"));
        widget_g5->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_g5, 4, 7, 1, 1);

        widget_g6 = new QWidget(gridLayoutWidget);
        widget_g6->setObjectName(QString::fromUtf8("widget_g6"));
        widget_g6->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_g6, 3, 7, 1, 1);

        widget_h1 = new QWidget(gridLayoutWidget);
        widget_h1->setObjectName(QString::fromUtf8("widget_h1"));
        widget_h1->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_h1, 8, 8, 1, 1);

        widget_h2 = new QWidget(gridLayoutWidget);
        widget_h2->setObjectName(QString::fromUtf8("widget_h2"));
        widget_h2->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_h2, 7, 8, 1, 1);

        widget_h3 = new QWidget(gridLayoutWidget);
        widget_h3->setObjectName(QString::fromUtf8("widget_h3"));
        widget_h3->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_h3, 6, 8, 1, 1);

        widget_h4 = new QWidget(gridLayoutWidget);
        widget_h4->setObjectName(QString::fromUtf8("widget_h4"));
        widget_h4->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_h4, 5, 8, 1, 1);

        widget_h6 = new QWidget(gridLayoutWidget);
        widget_h6->setObjectName(QString::fromUtf8("widget_h6"));
        widget_h6->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_h6, 3, 8, 1, 1);

        widget_d7 = new QWidget(gridLayoutWidget);
        widget_d7->setObjectName(QString::fromUtf8("widget_d7"));
        widget_d7->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255)"));

        gameGridLayout->addWidget(widget_d7, 2, 4, 1, 1);

        widget_c1 = new QWidget(gridLayoutWidget);
        widget_c1->setObjectName(QString::fromUtf8("widget_c1"));
        widget_c1->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0)"));

        gameGridLayout->addWidget(widget_c1, 8, 3, 1, 1);

        graphicsView = new QGraphicsView(Form_Board);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(70, 60, 561, 561));
        graphicsView->setStyleSheet(QString::fromUtf8("background-color: rgba(255,255,255,0)"));
        graphicsView->setFrameShape(QFrame::NoFrame);
        graphicsView->setFrameShadow(QFrame::Plain);
        graphicsView->setLineWidth(0);
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::NoBrush);
        graphicsView->setBackgroundBrush(brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::NoBrush);
        graphicsView->setForegroundBrush(brush1);
        graphicsView->setInteractive(true);
        graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

        retranslateUi(Form_Board);

        QMetaObject::connectSlotsByName(Form_Board);
    } // setupUi

    void retranslateUi(QWidget *Form_Board)
    {
        Form_Board->setWindowTitle(QApplication::translate("Form_Board", "Form", 0, QApplication::UnicodeUTF8));
        label_nothing->setText(QString());
        label_col_a->setText(QApplication::translate("Form_Board", "a", 0, QApplication::UnicodeUTF8));
        label_col_b->setText(QApplication::translate("Form_Board", "b", 0, QApplication::UnicodeUTF8));
        label_col_c->setText(QApplication::translate("Form_Board", "c", 0, QApplication::UnicodeUTF8));
        label_col_d->setText(QApplication::translate("Form_Board", "d", 0, QApplication::UnicodeUTF8));
        label_col_e->setText(QApplication::translate("Form_Board", "e", 0, QApplication::UnicodeUTF8));
        label_col_f->setText(QApplication::translate("Form_Board", "f", 0, QApplication::UnicodeUTF8));
        label_col_g->setText(QApplication::translate("Form_Board", "g", 0, QApplication::UnicodeUTF8));
        label_col_h->setText(QApplication::translate("Form_Board", "h", 0, QApplication::UnicodeUTF8));
        label_row_8->setText(QApplication::translate("Form_Board", "1", 0, QApplication::UnicodeUTF8));
        label_row_7->setText(QApplication::translate("Form_Board", "2", 0, QApplication::UnicodeUTF8));
        label_raw_6->setText(QApplication::translate("Form_Board", "3", 0, QApplication::UnicodeUTF8));
        label_row_5->setText(QApplication::translate("Form_Board", "4", 0, QApplication::UnicodeUTF8));
        label_row_4->setText(QApplication::translate("Form_Board", "5", 0, QApplication::UnicodeUTF8));
        label_row_3->setText(QApplication::translate("Form_Board", "6", 0, QApplication::UnicodeUTF8));
        label_row_2->setText(QApplication::translate("Form_Board", "7", 0, QApplication::UnicodeUTF8));
        label_row_1->setText(QApplication::translate("Form_Board", "8", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form_Board: public Ui_Form_Board {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOARD_H
