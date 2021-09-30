#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "app_core.h"

QString fileName, exportDir;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_userManualPButton_clicked()
{
    QFile mpdf(":/UserManual.pdf");
    mpdf.copy("/storage/emulated/0/IntelligentTimetablingApp/UserManual.pdf");
    QDesktopServices::openUrl(QUrl("/storage/emulated/0/IntelligentTimetablingApp/UserManual.pdf"));
}

void MainWindow::on_runPButton_clicked()
{
    ui->runPButton->setEnabled(false);
    bool manualInput = ui->manualInputRButton->isChecked();
    bool readableFormat = ui->readableFormatCButton->isChecked();
    bool exportResult = ui->exportCButton->isChecked();
    if (manualInput)
    {
        QString qs = ui->manualInputEdit->toPlainText();
        QFile input("/storage/emulated/0/IntelligentTimetablingApp/_in.txt");
        input.open(QIODevice::ReadWrite | QFile::Truncate);
        QTextStream os(&input);
        os << qs;
        input.close();
        fileName = "/storage/emulated/0/IntelligentTimetablingApp/_in.txt";
    }
    else if (fileName == QString())
    {
        fileName = "/storage/emulated/0/IntelligentTimetablingApp/_in.txt";
    }
    if (!exportResult || exportDir == QString())
    {
        exportDir = "/storage/emulated/0/IntelligentTimetablingApp/";
    }
    int numWarning = go(fileName, readableFormat, exportDir);
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Intelligent Timetabling App",
        "Timetable generated.\n" + QString::number(numWarning)
        + " warning(s) found.\nThe result and the warning(s) are stored in result.txt and warning.txt in "
        + exportDir + " respectively.\nDo you want to view the result?",
        QMessageBox::No | QMessageBox::Yes);
    if (reply == QMessageBox::Yes)
    {
        QDesktopServices::openUrl(QUrl(exportDir + "/result.txt"));
    }
    ui->runPButton->setEnabled(true);
}

void MainWindow::checkRButton()
{
    ui->manualInputRButton->setChecked(true);
}

void MainWindow::setInput()
{
    ui->manualInputEdit->setText("3\n"
                                 "1,Watch a movie with Sam,60,2:00’\n"
                                 "2 0\n"
                                 "Mon 14:00-16:00,Tue 12:00-14:00\n"
                                 "\n"
                                 "2,Apply for passport,10,2:00’\n"
                                 "2 0\n"
                                 "Mon 13:00-16:00,Tue 13:00-16:00\n"
                                 "\n"
                                 "3,Pick up my passport,80,2:00’\n"
                                 "2 1\n"
                                 "Mon 13:00-16:00,Tue 13:00-16:00\n"
                                 "2\n");
}

void MainWindow::setScroll()
{
    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
}

void MainWindow::on_importTxtPButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, QString(), "/storage/emulated/0/IntelligentTimetablingApp/",
                                            tr("Text Files (*.txt)"));
    if (fileName != QString())
    {
        ui->manualInputRButton->setChecked(false);
        ui->importTxtRButton->setChecked(true);
    }
    ui->fileNameLabel->setText(fileName == QString() ? "No file selected." : fileName);
}

void MainWindow::on_exportPButton_clicked()
{
    exportDir = QFileDialog::getExistingDirectory(this, QString(), "/storage/");
    if (exportDir != QString())
    {
        ui->exportCButton->setChecked(true);
    }
    ui->directoryLabel->setText(exportDir == QString() ? "No directory selected." : exportDir);
}

void MainWindow::on_copyDirPButton_clicked()
{
    QApplication::clipboard()->setText("/storage/emulated/0/IntelligentTimetablingApp/");
}
