#include "Calculator.h"

#define PX_SIZE 70
QString fact(const double x);

QPushButton* Calculator::createButton(const QString &str)
{
    QPushButton* pcmd = new QPushButton(str);

    if (str == "C")
        pcmd->setFixedSize(QSize(PX_SIZE, PX_SIZE));

    connect(pcmd, SIGNAL(clicked()), SLOT(slotButtonClicked()));
    pcmd->setMinimumSize(QSize(PX_SIZE, PX_SIZE));
    pcmd->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    pcmd->setStyleSheet("QPushButton {"
                            "font-size: 24px;"
                            "background-color: #E6E6E6; "
                            "border: 1px outset darkgray;"
                            "border-radius: 6px; } "
                        "QPushButton:hover { "
                            "background-color: lightgray; } "
                        "QPushButton:pressed {"
                            "background-color: #C0C0C0; }");
    return pcmd;
}

Calculator::Calculator(QWidget *parent) : QWidget(parent)
{
    //BASIC LAYOUTS
    QGridLayout* pTopLayout = new QGridLayout;  //лсд и мод
    QGridLayout* pCommonButtonsLayout = new QGridLayout; //обычные кнопки
    QGridLayout* pEngineeringButtonsLayout = new QGridLayout;    //инженерные кнопки

    //BASIC WIDGETS
    pTopWidgets = new QWidget;  pTopWidgets->setLayout(pTopLayout);
    pCommonButtonsWidget = new QWidget;  pCommonButtonsWidget->setLayout(pCommonButtonsLayout);
    pEngineeringButtonsWidget = new QWidget;  pEngineeringButtonsWidget->setLayout(pEngineeringButtonsLayout);

        //LCD
        m_ptxt = new QLineEdit();

        m_ptxt->setFixedHeight(PX_SIZE);
        m_ptxt->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        m_ptxt->setAlignment(Qt::AlignRight);
        m_ptxt->setMaxLength(12);
        m_ptxt->setReadOnly(true);
        m_ptxt->setStyleSheet("background: rgba(0, 0, 0, 0%);  "
                              "border-width: 1px;              "
                              "border-style: solid;            "
                              "border-color: lightgray;        "
                              "font-size: 48px;                "
                              "selection-background-color: rgb(100, 100, 100);"
                              "");
        //MOD RADIO BUTTONS
        QGroupBox* pModGroupBox = new QGroupBox;

        pModGroupBox->setFixedHeight(PX_SIZE);
        QRadioButton* pb1 = new QRadioButton("Обычный");
        QRadioButton* pb2 = new QRadioButton("Инженерный");
        QVBoxLayout *pRadioLayout = new QVBoxLayout;
        pRadioLayout->addWidget(pb1);
        pRadioLayout->addWidget(pb2);
        pModGroupBox->setLayout(pRadioLayout);

        //CONNECTS
        connect(pb1, SIGNAL(clicked()), SLOT(slotCommonClicked()));
        connect(pb2, SIGNAL(clicked()), SLOT(slotEngineeringClicked()));

    //TOP WIDGETS LAYOUT
    pTopLayout->addWidget(m_ptxt, 0, 0, 1, 2);
    pTopLayout->addWidget(pModGroupBox, 1, 0, 1, 1);
    pTopLayout->addWidget(createButton("C"), 1, 1, 1, 1);

    //COMMON BUTTONS LAYOUT
    QString commonButtons[4][5] = {{"7", "8", "9", QChar(0x00F7)/*div*/, QChar(0x221A)+'x'/*x^(1/2)*/},
                                   {"4", "5", "6", QChar(0x00D7)/*multi*/, "1/x"},
                                   {"1", "2", "3", "-", " = "},
                                   {"0", "0", ".", "+", " = "}
                                  };
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 5; ++j)
        {
            if (i == 2 && j == 4)
                pCommonButtonsLayout->addWidget(createButton(commonButtons[i][j]), i, j, 2, 1);
            else if (i == 3 && j == 0)
            {
                pCommonButtonsLayout->addWidget(createButton(commonButtons[i][j]), i, j, 1, 2);
                j++;
            }
            else if (!(i == 3 && j == 4))
                pCommonButtonsLayout->addWidget(createButton(commonButtons[i][j]), i, j);
        }

    //ENGINEERING(ADVANCED) BUTTONS LAYOUT
    QString engineeringButtons[4][5] = {{"sinh", "sin", "exp", 'x'+QChar(0x00B2)/*x^2*/},
                                        {"cosh", "cos", "ln", 'x'+QChar(0x00B3)/*x^3*/},
                                        {"tanh", "tan", "log", 'x'+QChar(0x02B8)/*x^y*/},
                                        {"n!", QChar(0x03C0)/*pi*/, QChar(0x221B)+'x'/*x^(1/3)*/, QString(QChar(0x02B8)) + QChar(0x221A)+'x'/*ysqrt*/}
                                       };
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
        {
            pEngineeringButtonsLayout->addWidget(createButton(engineeringButtons[i][j]), i, j);
        }

    //STYLE
    pCommonButtonsLayout->setContentsMargins(0, 0, 0, 0);
    pEngineeringButtonsLayout->setContentsMargins(0, 0, 0, 0);
    pTopLayout->setContentsMargins(0, 0, 0, 0);

    //START
    pb1->click();
    m_ptxt->setText(m_strDisplay);
}

Calculator::~Calculator()
{
    delete pTopWidgets;
    delete pCommonButtonsWidget;
    delete pEngineeringButtonsWidget;
};

void Calculator::slotCommonClicked()
{
    setWindowTitle("Калькулятор (обычный)");
    if (pMainLayout)
    {
        pEngineeringButtonsWidget->setParent(nullptr);
        delete pMainLayout;
    }
    pMainLayout = new MyMainLayout;
    pMainLayout->addWidget(pTopWidgets, 0, 0);
    pMainLayout->addWidget(pCommonButtonsWidget, 1, 0);
    pMainLayout->setHorizontalSpacing(PX_SIZE/3);
    setLayout(pMainLayout);
    adjustSize();
    setFixedSize(minimumSize());
}

void Calculator::slotEngineeringClicked()
{
    setWindowTitle("Калькулятор (инженерный)");
    if (pMainLayout)
        delete pMainLayout;
    pMainLayout = new MyMainLayout;
    pMainLayout->addWidget(pTopWidgets, 0, 0, 1, 2);
    pMainLayout->addWidget(pEngineeringButtonsWidget, 1, 0);
    pMainLayout->addWidget(pCommonButtonsWidget, 1, 1);
    pMainLayout->setHorizontalSpacing(PX_SIZE/3);
    setLayout(pMainLayout);
    setFixedSize(minimumSize());
}

void Calculator::slotButtonClicked()
{
    QString str = (dynamic_cast<QPushButton*>(sender()))->text();
    static bool prevIsOperation = false;
    static QRegExp number("[0-9]");

    if (str == "C")
    {
        m_stk.clear();
        m_ptxt->clear();
        m_strDisplay = "0";
        m_ptxt->setText(m_strDisplay);
        return;
    }
    else if (number.exactMatch(str))
    {
        if (m_strDisplay == "0")
            m_strDisplay = "";
        m_strDisplay+= str;
        m_ptxt->setText(m_strDisplay);
    }
    else if (str == ".")
    {
        if (prevIsOperation)
            m_stk.clear();
        if (!m_strDisplay.contains("."))
        {
            m_strDisplay+= str;
            m_ptxt->setText(m_strDisplay);
        }
        return;
    }

    //UNARY OPERATORS
    else if (str == QChar(0x221A)+'x'/*x^(1/2)*/)
        calculate(QString::number(sqrt(m_ptxt->text().toDouble()), 'g', 12));
    else if (str == "1/x")
        calculate(QString::number(1/(m_ptxt->text().toDouble()), 'g', 12));

    else if (str == "sinh")
        calculate(QString::number(sinh(m_ptxt->text().toDouble()), 'g', 12));
    else if (str == "cosh")
        calculate(QString::number(cosh(m_ptxt->text().toDouble()), 'g', 12));
    else if (str == "tanh")
        calculate(QString::number(tanh(m_ptxt->text().toDouble()), 'g', 12));
    else if (str == "n!")
        calculate(fact(m_ptxt->text().toDouble()));

    else if (str == "sin")
        calculate(QString::number(qSin(m_ptxt->text().toDouble()), 'g', 12));
    else if (str == "cos")
        calculate(QString::number(cos(m_ptxt->text().toDouble()), 'g', 12));
    else if (str == "tan")
        calculate(QString::number(tan(m_ptxt->text().toDouble()), 'g', 12));
    else if (str == QChar(0x03C0)/*pi*/)
        calculate(QString::number(M_PI, 'g', 12));

    else if (str == "exp")
        calculate(QString::number(exp(m_ptxt->text().toDouble()), 'g', 12));
    else if (str == "ln")
        calculate(QString::number(log(m_ptxt->text().toDouble()), 'g', 12));
    else if (str == "log")
        calculate(QString::number(log10(m_ptxt->text().toDouble()), 'g', 12));
    else if (str == QChar(0x221B)+'x'/*x^(1/3)*/)
        calculate(QString::number(cbrt(m_ptxt->text().toDouble()), 'g', 12));

    else if (str == 'x'+QChar(0x00B2)/*x^2*/)
        calculate(QString::number(pow(m_ptxt->text().toDouble(), 2), 'g', 12));
    else if (str == 'x'+QChar(0x00B3)/*x^3*/)
        calculate(QString::number(pow(m_ptxt->text().toDouble(), 3), 'g', 12));

    //BINARY OPERATORS
    else
    {
        if (m_stk.count() >= 2 && !prevIsOperation)
        {
            m_stk.push(m_ptxt->text());
            calculate();
            m_stk.clear();
            m_stk.push(m_ptxt->text());
            if (str != "=")
                m_stk.push(str);
            m_strDisplay = "0";
            prevIsOperation = true;
            return;
        }
        else if ( str != "=")
        {
            m_stk.clear();
            m_stk.push(m_ptxt->text());
            m_stk.push(str);
            prevIsOperation = true;
            m_strDisplay = "0";
            return;
        }
        else if (str == "=") return;
    }
    prevIsOperation = false;
}

void Calculator::calculate(QString txtResult)
{
    if (txtResult == "")
    {
    qreal fOperand2      = m_stk.pop().toDouble();
    QString strOperation = m_stk.pop();
    qreal fOperand1      = m_stk.pop().toDouble();
    qreal fResult        = 0;

    if (strOperation == "+")
    {
        fResult = fOperand1 + fOperand2;
    }
    else if (strOperation == "-")
    {
        fResult = fOperand1 - fOperand2;
    }
    else if (strOperation == QChar(0x00F7)/*div*/)
    {
        fResult = fOperand1 / fOperand2;
    }
    else if (strOperation == QChar(0x00D7)/*multi*/)
    {
        fResult = fOperand1 * fOperand2;
    }
    else if (strOperation == QString(QChar(0x02B8)) + QChar(0x221A)+'x'/*ysqrt*/)
    {
        fResult = qPow(fOperand1, 1/fOperand2);
    }
    else if (strOperation == 'x'+QChar(0x02B8)/*x^y*/)
    {
        fResult = qPow(fOperand1, fOperand2);
    }
    m_ptxt->setText(QString::number(fResult, 'g', 12));
    }
    else
    {
        m_ptxt->setText(txtResult);
        m_strDisplay = "0";
        m_stk.clear();
    }
}

//factorial
QString fact(const double x)
{
    if ((x-floor(x)) != 0.0) return "nun";
    else if (x > 170) return "inf";
    else
    {
        qreal res = 1;
        for (int i = 1; i<=x; i++)
            res*= i;
        return QString::number(res, 'f', 0);
    }
}


