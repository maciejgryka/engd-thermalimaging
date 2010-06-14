/********************************************************************************
** Form generated from reading ui file 'thermalimaging.ui'
**
** Created: Fri 11. Jun 11:07:30 2010
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
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
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "TestGLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_ThermalImagingClass
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    TestGLWidget *mainWid;
    QSlider *thermalVisualSlider;
    QLCDNumber *lcdNumber;
    QPushButton *ransacButton;
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
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        mainWid = new TestGLWidget(verticalLayoutWidget);
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
        ransacButton = new QPushButton(centralWidget);
        ransacButton->setObjectName(QString::fromUtf8("ransacButton"));
        ransacButton->setGeometry(QRect(840, 630, 75, 25));
        ThermalImagingClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ThermalImagingClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1068, 27));
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
        ransacButton->setText(QApplication::translate("ThermalImagingClass", "RANSAC", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ThermalImagingClass);
    } // retranslateUi

};

namespace Ui {
    class ThermalImagingClass: public Ui_ThermalImagingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THERMALIMAGING_H
