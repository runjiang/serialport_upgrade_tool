/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QLabel *label_2;
    QPushButton *closeBtn;
    QLabel *label;
    QTextBrowser *textBrowser;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->resize(530, 420);
        AboutDialog->setMinimumSize(QSize(530, 420));
        AboutDialog->setMaximumSize(QSize(530, 420));
        AboutDialog->setStyleSheet(QString::fromUtf8("QPushButton{background:url(:/images/header.bmp)}"));
        label_2 = new QLabel(AboutDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(90, 80, 411, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial Black"));
        font.setPointSize(16);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8("font: 16pt \"Arial Black\";"));
        closeBtn = new QPushButton(AboutDialog);
        closeBtn->setObjectName(QString::fromUtf8("closeBtn"));
        closeBtn->setGeometry(QRect(210, 370, 93, 23));
        closeBtn->setMinimumSize(QSize(0, 23));
        closeBtn->setMaximumSize(QSize(16777215, 23));
        label = new QLabel(AboutDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 121, 71));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/logo129-66.png")));
        textBrowser = new QTextBrowser(AboutDialog);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(10, 150, 471, 171));

        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "\345\205\263\344\272\216QCom 1.0", nullptr));
        label_2->setText(QApplication::translate("AboutDialog", "MCU\345\215\207\347\272\247\350\260\203\350\257\225\345\267\245\345\205\267\357\274\210\347\211\210\346\234\254\357\274\2321.1\357\274\211", nullptr));
        closeBtn->setText(QApplication::translate("AboutDialog", "\345\205\263\351\227\255(C)", nullptr));
        label->setText(QString());
        textBrowser->setHtml(QApplication::translate("AboutDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table border=\"0\" style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\345\256\213\344\275\223'; font-size:12pt; font-weight:600;\">\350\256\270\345\217\257\345\243\260\346\230\216</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'\345\256\213\344\275\223'; color:#008000;\"><br /></p>\n"
"<p style=\"-qt-paragraph-typ"
                        "e:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Ubuntu'; font-size:11pt;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\345\256\213\344\275\223'; font-weight:600; color:#00aa00;\">         \346\202\250\345\217\257\344\273\245\346\214\211\347\205\247\350\207\252\347\224\261\350\275\257\344\273\266\345\237\272\351\207\221\344\274\232\346\211\200\345\217\221\350\241\250\347\232\204 GNU GPL \345\215\217\350\256\256\350\207\252\347\224\261\345\217\221\346\224\276\345\222\214/\346\210\226\344\277\256\346\224\271\345\256\203\357\274\233\351\207\207\347\224\250\345\215\217\350\256\256\347\232\204\347\254\254 3 \347\211\210\357\274\214\346\210\226\350\200\205(\346\240\271\346\215\256\346\202\250\347\232\204\351\200\211\346\213\251)\344\273\273\344\275\225\346\233\264\351\253\230\347\211\210\346\234\254\343\200\202</span><"
                        "/p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Courier New,courier'; font-weight:600; color:#00aa00;\">    \345\217\221\350\241\250\347\232\204\347\233\256\347\232\204\346\230\257\345\270\214\346\234\233\345\256\203\350\203\275\345\244\237\345\257\271\346\202\250\346\234\211\347\224\250\357\274\214\344\275\206\346\210\221\344\273\254\346\262\241\346\234\211\344\273\273\344\275\225\344\277\235\350\257\201\357\274\233\345\257\271\344\272\216\344\273\245\344\273\273\344\275\225\347\224\250\351\200\224\344\275\277\347\224\250\345\256\203\346\211\200\351\200\240\346\210\220\347\232\204\344\273\273\344\275\225\347\233\264\346\216\245\346\210\226\351\227\264\346\216\245\345\220\216\346\236\234\351\203\275\344\270\215\346\211\277\346\213\205\344\273\273\344\275\225\350\264\243\344\273\273\343\200\202\350\257\267\345\217\202\347\234\213 GNU GPL \345\215\217\350\256\256\344\270\255\347\232\204\347\273\206\350\212"
                        "\202\343\200\202</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Courier New,courier'; font-weight:600; color:#00aa00;\">    \346\202\250\345\272\224\350\257\245\345\234\250\346\224\266\345\210\260\350\257\245\347\250\213\345\272\217\347\232\204\345\220\214\346\227\266\346\224\266\345\210\260\344\272\206GNU GPL\345\215\217\350\256\256\347\232\204\345\211\257\346\234\254\343\200\202\345\246\202\346\236\234\346\262\241\346\234\211\347\232\204\350\257\235\357\274\214\350\257\267\345\217\202\350\247\201\357\274\210http://www.gnu.org/licenses \357\274\211\343\200\202</span></p></td></tr></table></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
