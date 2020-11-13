#ifndef LINEEDITOR_H
#define LINEEDITOR_H

#include <QTextEdit>
#include <QWidget>
#include <QSize>


class Editor;

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

#endif // LINEEDITOR_H
