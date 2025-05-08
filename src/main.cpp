#include "MainWindow.hpp"

#include <QApplication>
#include <QThread>
#include <QRandomGenerator>

int main(int argc, char* argv[])
{
    auto app = QApplication{argc, argv};
    app.setStyle("fusion");

    auto mainWindow = MainWindow{};
    mainWindow.show();

    return app.exec();
}