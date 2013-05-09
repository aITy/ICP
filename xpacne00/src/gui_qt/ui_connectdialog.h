/********************************************************************************
** Form generated from reading UI file 'connectdialog.ui'
**
** Created: Thu 9. May 01:05:29 2013
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTDIALOG_H
#define UI_CONNECTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_connectDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *adressLabel;
    QLineEdit *LineEdit_Addr;
    QLabel *portLabel;
    QLineEdit *LineEdit_Port;
    QPushButton *button_Send;

    void setupUi(QDialog *connectDialog)
    {
        if (connectDialog->objectName().isEmpty())
            connectDialog->setObjectName(QString::fromUtf8("connectDialog"));
        connectDialog->setWindowModality(Qt::ApplicationModal);
        connectDialog->resize(223, 100);
        verticalLayout = new QVBoxLayout(connectDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        adressLabel = new QLabel(connectDialog);
        adressLabel->setObjectName(QString::fromUtf8("adressLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, adressLabel);

        LineEdit_Addr = new QLineEdit(connectDialog);
        LineEdit_Addr->setObjectName(QString::fromUtf8("LineEdit_Addr"));

        formLayout->setWidget(0, QFormLayout::FieldRole, LineEdit_Addr);

        portLabel = new QLabel(connectDialog);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, portLabel);

        LineEdit_Port = new QLineEdit(connectDialog);
        LineEdit_Port->setObjectName(QString::fromUtf8("LineEdit_Port"));

        formLayout->setWidget(1, QFormLayout::FieldRole, LineEdit_Port);

        button_Send = new QPushButton(connectDialog);
        button_Send->setObjectName(QString::fromUtf8("button_Send"));
        button_Send->setEnabled(true);
        button_Send->setMaximumSize(QSize(100, 30));
        button_Send->setLayoutDirection(Qt::LeftToRight);
        button_Send->setAutoExclusive(false);
        button_Send->setAutoDefault(false);
        button_Send->setDefault(true);

        formLayout->setWidget(2, QFormLayout::FieldRole, button_Send);


        verticalLayout->addLayout(formLayout);

#ifndef QT_NO_SHORTCUT
        adressLabel->setBuddy(LineEdit_Addr);
        portLabel->setBuddy(LineEdit_Port);
#endif // QT_NO_SHORTCUT

        retranslateUi(connectDialog);
        QObject::connect(button_Send, SIGNAL(clicked()), connectDialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(connectDialog);
    } // setupUi

    void retranslateUi(QDialog *connectDialog)
    {
        connectDialog->setWindowTitle(QApplication::translate("connectDialog", "P\305\231ipojit k serveru", 0, QApplication::UnicodeUTF8));
        adressLabel->setText(QApplication::translate("connectDialog", "&IP adresa:", 0, QApplication::UnicodeUTF8));
        portLabel->setText(QApplication::translate("connectDialog", "Port:", 0, QApplication::UnicodeUTF8));
        button_Send->setText(QApplication::translate("connectDialog", "Odeslat pozv\303\241nku", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class connectDialog: public Ui_connectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTDIALOG_H
