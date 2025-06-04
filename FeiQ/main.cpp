#include "feiqdialog.h"
#include "ckernel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    FeiQDialog w;
//    w.show();
    CKernel kernel;
    return a.exec();
}
