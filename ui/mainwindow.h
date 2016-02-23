#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDir>
#include <QList>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setFramesDir(QDir framesDir);
    QDir getFramesDir();
    QTimer *timer;

    void readFrames();
    ~MainWindow();

    void generatePPFrames();
    void generateDPFrames();

private slots:
    void open();
    void close();
    void about();
    void loopFrames();
    void displayFrame();    

    void on_horizontalSlider_sliderMoved(int position);

private:

    Ui::MainWindow *ui;

    QDir framesDir;
    QFileInfoList ImageList;
    int index;

    QAction *actionOpen;
    QAction *actionClose;
    QAction *actionAboutFMPT;
    QAction *actionAboutQt;

};

#endif // MAINWINDOW_H
