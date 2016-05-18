// Copyright (c) 2012-2016 Isaac Morales Durán. All rights reserved.
// Institute of Astrophysics of Andalusia, IAA-CSIC
//
// This file is part of FMPT (Fiber MOS Positioning Tools)
//
// FMPT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//---------------------------------------------------------------------------
//File: mainwindow.h
//Content: gráfical classes and grphical functions
//Autor: Pique
//---------------------------------------------------------------------------

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QPicture>
#include <QTimer>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);


    this->timer = new QTimer(this);
    timer->start(1000/12);

    // signal->slots connections
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionAboutFMPT, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(this->timer, SIGNAL(timeout()), this, SLOT(loopFrames()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

// slots
void MainWindow::open()
{
    this->timer->stop();
    QString dir = QFileDialog::getExistingDirectory(this, "Open Directory",
                                                this->getFramesDir().absolutePath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    QDir dirSelect(dir);
    setFramesDir(dirSelect);
    this->timer->start(1000/12);

}

void MainWindow::close()
{
    this->timer->stop();
    this->close();
}

void MainWindow::about()
{
    QMessageBox::about(this, ("About FMPT Viewer"),
    ("<b>FMPT Viewer</b>"));
}


// internal getter/setter framedir
void MainWindow::setFramesDir(QDir dir)
{
    this->framesDir = dir;
    readFrames();
}

QDir MainWindow::getFramesDir()
{
    return this->framesDir;
}

// reading frames
void MainWindow::readFrames()
{
    ui->statusbar->showMessage("Reading frames from "+this->framesDir.absolutePath());

    // indexing..
    QStringList filter;
    filter << "*.fmpt";
    //..
    //filter << "*.gif";
    //..

    QDir folder = this->getFramesDir();
    QFileInfoList tmpfiles = folder.entryInfoList(filter, QDir::Files, QDir::Name);

    if (tmpfiles.size()==0) {
        QMessageBox::information(this, "Sequence Viewer",
                                 "Cannot load sequence from this folder");
    }else{
        this->ImageList = tmpfiles;
        ui->horizontalSlider->setMaximum(this->ImageList.size()-1);
        this->index = 0;
    }

}

void MainWindow::loopFrames()
{
    if (this->index<ImageList.size()){
        ui->statusbar->showMessage("Displaying frames from "+this->framesDir.absolutePath());
        displayFrame();
        ui->horizontalSlider->setValue(this->index);
        this->index++;
    }else{
        this->timer->stop();
    }
}

void MainWindow::displayFrame()
{

    QString fileName = this->ImageList[this->index].absoluteFilePath();
    //..
    //QImage image(fileName);
    //..
    QPicture image;
    image.load(fileName);

    if (image.isNull()) {
        QMessageBox::information(this, "Sequence Viewer",
                                 "Cannot load image in sequence");
        return;
    }
    ui->imageLabel->setPicture(image);
    //..
    //ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    //..

}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    this->timer->stop();
    this->index = position;
    displayFrame();
}
