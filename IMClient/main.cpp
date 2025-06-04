#include "mychatdialog.h"
#include "ckernel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MyChatDialog w;
//    w.show();
    CKernel kernel;
    return a.exec();
}
