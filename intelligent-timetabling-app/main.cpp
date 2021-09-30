#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir dir("/storage/emulated/0/IntelligentTimetablingApp");
    if (!dir.exists())
    {
        QDir().mkdir("/storage/emulated/0/IntelligentTimetablingApp");
        exit(0);
    }
    MainWindow w;
    w.checkRButton();
    w.setInput();
    w.setScroll();
    w.show();

    return a.exec();
}
