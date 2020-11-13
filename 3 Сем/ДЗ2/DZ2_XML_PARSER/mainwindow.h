#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include <QGridLayout>
#include <QMenuBar>
#include <QMenu>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeView>
#include <QtXmlPatterns/QXmlQuery>

#include "dommodel.h"
#include "mydelegate.h"
#include <mypushbutton.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QGridLayout *pMainLayout;
    QWidget *pMainWidget;
    QMenu *pMenu;

    QTreeView *pTreeView;
    DomModel *domModel = nullptr;
    QString singleXMLPath = "null";
    //xq
    QWidget *xQueryWidget;
    QTextEdit *xqTextEdit;
    QPushButton *loadXqPb;
    QPushButton *startXqPb;
    QTextEdit *pResultText;

    QFileDialog *pFileDialog;
    QMenu *contextMenu = nullptr;
    QAction *removeAction;

    void createMenus();
    QWidget * createXQueryWidget();
    QString getXMLFile();
    void execXQ(const QString &);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void open();
    void closeAll();
    void saveAs();

    void onCustomContextMenu(const QPoint &point);

    void openXQ();
    void startXQ();
    void setText(const QString &);

};
#endif // MAINWINDOW_H
