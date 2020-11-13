#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QMainWindow>
#include <QApplication>
#include <QAction>
#include <QActionGroup>
#include <QRadioButton>
#include <QMenu>
#include <QMenuBar>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTimer>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QTextEdit>
#include <QWidget>
#include <QCloseEvent>
#include <QObject>
#include <QTabWidget>
#include <QModelIndex>
#include <QTextCharFormat>
#include <QDockWidget>
#include <QFileSystemModel>
#include <QDomNode>
#include <QString>
#include "highlighter.h"

class QAction;
class QActionGroup;
class QLabel;
class QMdiArea;
class QMenu;
class QToolBar;
class Editor;
class LineNumberArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void newFile();
    void openFile(const QString &fileName);
    void on_treeView_doubleClicked(const QModelIndex &index);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void open();
    void save();
    void saveAs();
    void cut();
    void copy();
    void paste();
    void about();
    void updateActions();
    void loadFiles();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void addEditor(Editor *editor, QString & str);
    Editor *activeEditor();

    Editor *currentTextEdit;
    QVector<Editor*> textEdits;

    void setupEditor(Editor *editor);
    Highlighter *highlighter;

    QMdiArea *mdiArea;

    QLabel *readyLabel;
    QWidgetList windows;
    QFileSystemModel *model;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QActionGroup *windowActionGroup;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *closeAction;
    QAction *closeAllAction;
    QAction *tileAction;
    QAction *cascadeAction;
    QAction *nextAction;
    QAction *previousAction;
    QAction *separatorAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
};

#endif // MAINWINDOW_H
