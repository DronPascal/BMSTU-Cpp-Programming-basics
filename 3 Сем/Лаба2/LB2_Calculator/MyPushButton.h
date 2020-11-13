#pragma once

#include <QtCore>
#include <QtGui>
#include <QPushButton>

class MyPushButton : public QPushButton{
    Q_OBJECT
public:
    QFont font;

    MyPushButton(QPushButton* parent = nullptr) :
        QPushButton(parent)
    {
        setFont(font);
        setText("1");
    }

    MyPushButton(QString const &str, QPushButton* parent = nullptr) :
        QPushButton(parent)
    {
        setFont(font);
        setText(str);
    }

    void resizeEvent(QResizeEvent *event){
        font.setPixelSize(this->size().height() / 2.5);
        setFont(font);
    }
};
