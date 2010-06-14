/********************************************************************************
** Form generated from reading UI file 'thermalimaging.ui'
**
** Created: Thu 10. Jun 13:13:30 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THERMALIMAGING_H
#define UI_THERMALIMAGING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "MainGLWidget.h"
#include "TestGLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_ThermalImagingClass
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    MainGLWidget *mainWid;
    QSlider *thermalVisualSlider;
    QLCDNumber *lcdNumber;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ThermalImagingClass)
    {
        if (ThermalImagingClass->objectName().isEmpty())
            ThermalImagingClass->setObjectName(QString::fromUtf8("ThermalImagingClass"));
        ThermalImagingClass->resize(1068, 773);
        centralWidget = new QWidget(ThermalImagingClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(20, 10, 991, 601));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        mainWid = new MainGLWidget(verticalLayoutWidget);
        mainWid->setObjectName(QString::fromUtf8("mainWid"));

        verticalLayout->addWidget(mainWid);

        thermalVisualSlider = new QSlider(verticalLayoutWidget);
        thermalVisualSlider->setObjectName(QString::fromUtf8("thermalVisualSlider"));
        thermalVisualSlider->setMaximum(100);
        thermalVisualSlider->setValue(100);
        thermalVisualSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(thermalVisualSlider);

        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(230, 640, 64, 23));
        ThermalImagingClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ThermalImagingClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1068, 26));
        ThermalImagingClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ThermalImagingClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ThermalImagingClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ThermalImagingClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ThermalImagingClass->setStatusBar(statusBar);

        retranslateUi(ThermalImagingClass);

        QMetaObject::connectSlotsByName(ThermalImagingClass);
    } // setupUi

    void retranslateUi(QMainWindow *ThermalImagingClass)
    {
        ThermalImagingClass->setWindowTitle(QApplication::translate("ThermalImagingClass", "ThermalImaging", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ThermalImagingClass: public Ui_ThermalImagingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THERMALIMAGING_H
