#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString downloadsFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    
    ytdl = new QProcess(this);
    connect(ytdl, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(on_ytdl_finished(int, QProcess::ExitStatus)));
    ui->statusbar->showMessage("Ready.");
    ui->labelOutputFilepath->hide();
    ui->listWidget->hide();
    
    settings = new QSettings(qApp->applicationDirPath() + "/config.ini", QSettings::IniFormat);
    ui->lineEditDownloadFolder->setText(settings->value("download_dir", "").toString());
    ui->lineEditYoutubeDl->setText(settings->value("youtube_dl_path", "").toString());
    if (ui->lineEditDownloadFolder->text().isEmpty()) {
        ui->lineEditDownloadFolder->setText(downloadsFolder);
        settings->setValue("download_dir", downloadsFolder);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonDownload_clicked()
{
    QString src = ui->lineEditUrl->text().trimmed();
    if (src.isEmpty()) {
        QMessageBox::information(this, "Alert", "Set a Video URL to download!");
        return;
    }
    
    ui->statusbar->showMessage("Loading...");
    QString program = ui->lineEditYoutubeDl->text();
    QStringList arguments;
    arguments << "-F" << ui->lineEditUrl->text().trimmed();
    
    ytdl->start(program, arguments);
    parseOutput = true;
}

void MainWindow::on_listWidget_itemActivated(QListWidgetItem *item)
{
    QString targetFilename = ui->lineEditDownloadFolder->text() + "/%(title)s.%(ext)s";
    QString baris = item->text();
    QString selected = baris.left(baris.indexOf(' '));
    ui->plainTextEdit->appendPlainText(selected);
    
    ui->statusbar->showMessage("Loading...");
    QString program = ui->lineEditYoutubeDl->text();
    QStringList arguments;
    arguments << "-f" << selected 
              << "-o" << targetFilename
              << ui->lineEditUrl->text().trimmed() ;
    
    ytdl->start(program, arguments);
    downloadRunning = true;
    ui->labelOutputFilepath->hide();
    outputFilepath = "";
}

void MainWindow::on_ytdl_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->plainTextEdit->clear();
    QString resOk = QString::fromUtf8(ytdl->readAllStandardOutput());
    ui->plainTextEdit->setPlainText(resOk);
    QString resErr = ytdl->readAllStandardError();
    ui->plainTextEdit->appendPlainText(resErr);
    
    if (parseOutput) {
        QStringList ls = resOk.split("\n");
        for (int i = 0; i < ls.length(); i++) {
            if (ls[i].contains("webm") 
                    || ls[i].contains("mp4") 
                    || ls[i].contains("m4a")) 
            {
                ui->listWidget->addItem(ls[i]);
            }
        }
        ui->listWidget->show();
        parseOutput = false;
        ui->statusbar->showMessage("Done.");
        return;
    }
    else if (downloadRunning) {
        QString sbegin = "Destination: ";
        if (resOk.contains(sbegin)) {
            int pos = resOk.indexOf(sbegin) + sbegin.length();
            outputFilepath = resOk.mid(pos, resOk.indexOf("[", pos)).trimmed();
            ui->labelOutputFilepath->setText("File saved to: " + outputFilepath);
            ui->labelOutputFilepath->show();
        }
        QMessageBox::information(this, "Info", "Download completed");
        ui->statusbar->showMessage("Download completed.");
        downloadRunning = false;
        return;
    }
    
    ui->statusbar->showMessage("Done.");
}


void MainWindow::on_pushButtonBrowse_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select a download folder", ui->lineEditDownloadFolder->text());
    if (!dir.isEmpty()) {
        ui->lineEditDownloadFolder->setText(dir);
        settings->setValue("download_dir", dir);
    }
}


void MainWindow::on_pushButtonOpenFolder_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(ui->lineEditDownloadFolder->text()));
}


void MainWindow::on_pushButtonShowLog_clicked()
{
    ui->plainTextEdit->setVisible(!ui->plainTextEdit->isVisible());
}


void MainWindow::on_pushButtonOpenFile_clicked()
{
    if (!outputFilepath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(outputFilepath));
    }
}


void MainWindow::on_pushButtonUpdateYoutubeDl_clicked()
{
    ui->statusbar->showMessage("Loading...");
    QString program = ui->lineEditYoutubeDl->text();
    QStringList arguments;
    arguments << "-U";
    ytdl->start(program, arguments);
}


void MainWindow::on_pushButtonSelectYoutubeDl_clicked()
{
    QString dir = QFileDialog::getOpenFileName(this, "Select youtube-dl app");
    if (!dir.isEmpty()) {
        ui->lineEditYoutubeDl->setText(dir);
        settings->setValue("youtube_dl_path", dir);
    }
}


void MainWindow::on_pushButtonAbout_clicked()
{
    // need more general texts
    QMessageBox::about(this, "Video Downloader", "Video Downloader 1.0\n\nA simple youtude-dl gui.\n\n(c) 2021 pranatahouse.com");
}


void MainWindow::on_lineEditYoutubeDl_editingFinished()
{
    QString fpath = ui->lineEditYoutubeDl->text();
    if (!fpath.isEmpty()) {
        settings->setValue("youtube_dl_path", fpath);
    }
}


void MainWindow::on_lineEditDownloadFolder_editingFinished()
{
    QString fpath = ui->lineEditDownloadFolder->text();
    if (!fpath.isEmpty()) {
        settings->setValue("download_dir", fpath);
    }
}

