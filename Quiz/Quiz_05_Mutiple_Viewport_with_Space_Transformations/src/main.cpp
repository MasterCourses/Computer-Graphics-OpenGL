#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    window w;
    w.resize(w.sizeHint());
    w.setWindowTitle("05_Quiz_Mutiple_Viewport_with_Transformations");
    w.show();
    return a.exec();
}
