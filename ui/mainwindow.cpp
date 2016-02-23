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
