/********************************************************************************
** Form generated from reading ui file 'pointviewer.ui'
**
** Created: Wed 16. Jun 16:13:59 2010
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_POINTVIEWER_H
#define UI_POINTVIEWER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "MyEdit.h"
#include "pointshower.h"

QT_BEGIN_NAMESPACE

class Ui_PointViewerClass
{
public:
    QWidget *centralWidget;
    PointShower *mainWid;
    MyEdit *pointShow;
    QPushButton *showButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PointViewerClass)
    {
        if (PointViewerClass->objectName().isEmpty())
            PointViewerClass->setObjectName(QString::fromUtf8("PointViewerClass"));
        PointViewerClass->resize(1050, 799);
        centralWidget = new QWidget(PointViewerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        mainWid = new PointShower(centralWidget);
        mainWid->setObjectName(QString::fromUtf8("mainWid"));
        mainWid->setGeometry(QRect(10, 10, 1031, 631));
        pointShow = new MyEdit(centralWidget);
        pointShow->setObjectName(QString::fromUtf8("pointShow"));
        pointShow->setGeometry(QRect(10, 670, 1021, 23));
        showButton = new QPushButton(centralWidget);
        showButton->setObjectName(QString::fromUtf8("showButton"));
        showButton->setGeometry(QRect(800, 700, 75, 25));
        PointViewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(PointViewerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1050, 27));
        PointViewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PointViewerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        PointViewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(PointViewerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        PointViewerClass->setStatusBar(statusBar);

        retranslateUi(PointViewerClass);

        QMetaObject::connectSlotsByName(PointViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *PointViewerClass)
    {
        PointViewerClass->setWindowTitle(QApplication::translate("PointViewerClass", "PointViewer", 0, QApplication::UnicodeUTF8));
        showButton->setText(QApplication::translate("PointViewerClass", "Show", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(PointViewerClass);
    } // retranslateUi

};

namespace Ui {
    class PointViewerClass: public Ui_PointViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POINTVIEWER_H
