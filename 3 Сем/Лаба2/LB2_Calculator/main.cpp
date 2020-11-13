#include "Calculator.h"
#include <QGridLayout>
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Calculator calculator;


    calculator.show();

    //design
    return a.exec();
}
