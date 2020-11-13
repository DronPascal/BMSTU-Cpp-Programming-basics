#pragma once
#include <QGridLayout>

class MyMainLayout : public QGridLayout
{
    QLayoutItem* takeAt(int i)
    {
       i++;
       return nullptr;
    }
};
