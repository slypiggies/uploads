#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFile>
#include <QUrl>
#include <QTextStream>
#include <QApplication>
#include <QDir>
#include <QScroller>
#include <QMessageBox>
#include <QClipboard>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void checkRButton();

    void setInput();

    void setScroll();

private slots:
    void on_userManualPButton_clicked();

    void on_runPButton_clicked();

    void on_importTxtPButton_clicked();

    void on_exportPButton_clicked();

    void on_copyDirPButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
