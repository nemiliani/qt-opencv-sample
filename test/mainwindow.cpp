#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    capWebcam.open(0);
    if(capWebcam.isOpened() == false){
        ui->txtXYRadius->appendPlainText("Error: webcam not accessed successfully");
        return;
    }
    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));
    tmrTimer->start(20);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processFrameAndUpdateGUI()
{
    capWebcam.read(matOriginal);
    if(matOriginal.empty() == true) return;

    cv::inRange(matOriginal, cv::Scalar(0,0,175), cv::Scalar(100,100,256), matProcessed);
    cv::GaussianBlur(matProcessed, matProcessed, cv::Size(9, 9), 1.5);
    cv::HoughCircles(matProcessed, vecCircles, CV_HOUGH_GRADIENT, 2, matProcessed.rows/4, 100, 50, 10, 400);

    for(itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++){
        ui->txtXYRadius->appendPlainText(QString("Ball position x =") +
                                         QString::number((*itrCircles)[0]).rightJustified(4, ' ') +
                                         QString(", y =") +
                                         QString::number((*itrCircles)[1]).rightJustified(4, ' ') +
                                         QString(", radius =") +
                                         QString::number((*itrCircles)[2], 'f', 3).rightJustified(7, ' '));
        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), 3, cv::Scalar(0, 255, 0), CV_FILLED);
        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), (int)(*itrCircles)[2], cv::Scalar(0, 0, 255), 3);
    }
    cv::cvtColor(matOriginal, matOriginal, CV_BGR2RGB);
    QImage qimgOriginal((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    QImage qimgProcessed((uchar*)matProcessed.data, matProcessed.cols, matProcessed.rows, matProcessed.step, QImage::Format_Indexed8);

    ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));
    ui->lblProcessed->setPixmap(QPixmap::fromImage(qimgProcessed));
}

void MainWindow::on_btnPauseOnResume_clicked()
{
    if(tmrTimer->isActive() == true){
        tmrTimer->stop();
        ui->btnPauseOnResume->setText("Resume");
    }else{
        tmrTimer->start(20);
        ui->btnPauseOnResume->setText("Pause");
    }

}
