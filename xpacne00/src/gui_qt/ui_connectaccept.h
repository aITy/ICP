/********************************************************************************
** Form generated from reading UI file 'connectaccept.ui'
**
** Created: Wed 8. May 21:16:18 2013
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTACCEPT_H
#define UI_CONNECTACCEPT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_acceptDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *acceptDialog)
    {
        if (acceptDialog->objectName().isEmpty())
            acceptDialog->setObjectName(QString::fromUtf8("acceptDialog"));
        acceptDialog->resize(240, 142);
        buttonBox = new QDialogButtonBox(acceptDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(40, 90, 161, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(acceptDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 30, 201, 16));
        label_2 = new QLabel(acceptDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 60, 171, 16));

        retranslateUi(acceptDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), acceptDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), acceptDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(acceptDialog);
    } // setupUi

    void retranslateUi(QDialog *acceptDialog)
    {
        acceptDialog->setWindowTitle(QApplication::translate("acceptDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("acceptDialog", "Byla p\305\231ijata pozv\303\241nka na s\303\255\305\245ovou hru.", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("acceptDialog", "P\305\231ejete si tuto pozv\303\241nku p\305\231ijmout?", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class acceptDialog: public Ui_acceptDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTACCEPT_H
