/********************************************************************************
** Form generated from reading UI file 'help.ui'
**
** Created: Thu 9. May 01:05:29 2013
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELP_H
#define UI_HELP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_helpDialog
{
public:
    QLabel *label_2;
    QLabel *label;

    void setupUi(QDialog *helpDialog)
    {
        if (helpDialog->objectName().isEmpty())
            helpDialog->setObjectName(QString::fromUtf8("helpDialog"));
        helpDialog->resize(406, 412);
        label_2 = new QLabel(helpDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 40, 361, 371));
        label_2->setWordWrap(true);
        label = new QLabel(helpDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 10, 281, 31));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        retranslateUi(helpDialog);

        QMetaObject::connectSlotsByName(helpDialog);
    } // setupUi

    void retranslateUi(QDialog *helpDialog)
    {
        helpDialog->setWindowTitle(QApplication::translate("helpDialog", "N\303\241pov\304\233da", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("helpDialog", "Princip hry:\n"
"D\303\241ma je deskov\303\241 hra pro dva hr\303\241\304\215e. Hraje se na \305\241achovnici o velikosti 8 x 8 pol\303\255. Soupe\305\231i maj\303\255 na za\304\215\303\241tku hry 12 kamen\305\257 a diagon\303\241ln\303\255mi pohyby dop\305\231edu se sna\305\276\303\255 p\305\231esko\304\215en\303\255m vy\305\231adit protihr\303\241\304\215ovy kameny.\n"
"Speci\303\241ln\303\255 figurou ve h\305\231e je d\303\241ma, kterou hr\303\241\304\215 z\303\255sk\303\241 pokud dojde s kamenem na konec \305\241achovnice na soupe\305\231ov\304\233 stran\304\233. D\303\241ma se m\305\257\305\276e pohybovat v\305\241emi diagon\303\241ln\303\255mi sm\304\233ry o libovoln\303\275 po\304\215et m\303\255st.\n"
"C\303\255lem hry je vy\305\231adit v\305\241echny soupe\305\231ovy kameny. Hru za\304\215\303\255n\303\241 b\303\255l\303\275 hr\303\241\304\215.\n"
"\n"
"Vlastnosti hry:\n"
"Nov\303\241 hra se spou\305\241t\303\255 v hlavn\303\255 nab\303\255dce pomoc\303\255 z\303\241lo\305\276ky Hra.\n"
"Hru je mo\305"
                        "\276no hr\303\241t lok\303\241ln\304\233 proti soupe\305\231i nebo proti po\304\215\303\255ta\304\215i (um\304\233l\303\241 inteligence po\304\215\303\255ta\304\215e, nen\303\255 rozvinuta a slou\305\276\303\255 sp\303\255\305\241e jako uk\303\241zka).\n"
"Hru je mo\305\276no hr\303\241t i p\305\231es s\303\255\305\245. V tomto p\305\231\303\255pad\304\233 jeden z hr\303\241\304\215\305\257 je inici\303\241torem a mus\303\255 vyplnit s\303\255\305\245ov\303\251 nastaven\303\255. Druh\303\275 hr\303\241\304\215 nab\303\255dku hry mus\303\255 potvrdit.\n"
"\n"
"Hra se d\303\241 ulo\305\276it, a to do form\303\241tu XML nebo ICP syntaxe, pro pozd\304\233j\305\241\303\255 dohr\303\241n\303\255 nebo p\305\231ehr\303\241n\303\255.\n"
"Ulo\305\276enou hru je mo\305\276n\303\251 otev\305\231\303\255t v hlavn\303\255m menu v z\303\241lo\305\276ce Soubor.\n"
"\n"
"Hra nab\303\255z\303\255 m\303\263d p\305\231ehr\303\241n\303\255 (replay) ulo\305\276en\303\251 hry. V lev\303\251m menu hlavn\303\255ho okna jsou pro p\305\231"
                        "ehr\303\241n\303\255 p\305\231ichyst\303\241ny b\304\233\305\276n\303\251 n\303\241stroje (p\305\231ehr\303\241vat, pause, stop, dop\305\231edu, dozadu). Lze nastavit rychlost p\305\231ehr\303\241v\303\241n\303\255 tah\305\257 v milisekund\303\241ch (ms) a pro funkce dop\305\231edu a zp\304\233t lze nastavit po\304\215et krok\305\257.", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("helpDialog", "N\303\241pov\304\233da ke h\305\231e D\303\241ma ICP 2013", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class helpDialog: public Ui_helpDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELP_H
