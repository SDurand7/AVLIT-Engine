#include <QApplication>

#include "MainWindow.hpp"


int main(int argc, char *argv[]) {
    QApplication editor(argc, argv);
    MainWindow window;
    window.show();

    return editor.exec();
}