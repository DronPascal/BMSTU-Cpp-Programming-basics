#pragma once

#include <QWidget>
#include <QStack>
#include <QPushButton>
#include <QGridLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QtMath>
#include <cmath>

#include <MyMainLayout.h>

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

    QPushButton* createButton(const QString& str);
    void calculate(QString txtResult = "");

private:
    QLineEdit*      m_ptxt;
    QStack<QString> m_stk;
    QString         m_strDisplay = "0";

    QWidget*        pTopWidgets;
    QWidget*        pCommonButtonsWidget;
    QWidget*        pEngineeringButtonsWidget;

    MyMainLayout*   pMainLayout=nullptr;

public slots:
    void slotCommonClicked();
    void slotEngineeringClicked();
    void slotButtonClicked();

};

