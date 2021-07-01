#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QListWidgetItem>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void on_pushButtonDownload_clicked();
    void on_listWidget_itemActivated(QListWidgetItem *item);
    void on_ytdl_finished(int exitCode, QProcess::ExitStatus exitStatus);
    
    void on_pushButtonBrowse_clicked();
    
    void on_pushButtonOpenFolder_clicked();
    
    void on_pushButtonShowLog_clicked();
    
    void on_pushButtonOpenFile_clicked();
    
    void on_pushButtonUpdateYoutubeDl_clicked();
    
    void on_pushButtonSelectYoutubeDl_clicked();
    
    void on_pushButtonAbout_clicked();
    
    void on_lineEditYoutubeDl_editingFinished();
    
    void on_lineEditDownloadFolder_editingFinished();
    
private:
    Ui::MainWindow *ui;
    QProcess *ytdl;
    bool parseOutput;
    bool downloadRunning;
    QString outputFilepath;
    QSettings *settings;
};
#endif // MAINWINDOW_H
