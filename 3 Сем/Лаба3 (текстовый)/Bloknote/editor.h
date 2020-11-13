#ifndef EDITOR_H
#define EDITOR_H

#include <QTextEdit>
#include <QWidget>
#include <QAction>
#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QApplication>
#include <QFileSystemModel>
#include <QDir>
#include <QStringListModel>
#include <QDockWidget>
#include <QTreeView>
#include <QListView>
#include <QFileInfo>
#include <QModelIndex>
#include <QMenu>
#include <QMenuBar>
#include <QString>
#include <QPlainTextEdit>
#include <QMouseEvent>
#include <QEvent>

class Editor : public QPlainTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget *parent = nullptr);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void newFile();
    bool save();
    bool saveAs();
    QAction *windowMenuAction() const { return action; }

    static Editor *open(QWidget *parent = 0, QString&& name = " ");
    static Editor *openFile(const QString &fileName, QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void documentWasModified();
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    bool okToContinue();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    bool readFile(const QString &fileName);
    bool writeFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QWidget *lineNumberArea;
    QString curFile;
    bool isUntitled;
    QAction *action;
};


class LineNumberArea : public QWidget
{

public:
    LineNumberArea(Editor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    Editor *codeEditor;
};

#endif // EDITOR_H
