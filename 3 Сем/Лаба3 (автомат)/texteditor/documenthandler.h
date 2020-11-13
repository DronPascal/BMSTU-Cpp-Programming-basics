#ifndef DOCUMENTHANDLER_H
#define DOCUMENTHANDLER_H

#include <QFont>
#include <QObject>
#include <QTextCursor>
#include <QUrl>

#include "universalHighlighter.h"

#include <QStringListModel>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuickWidgets/QQuickWidget>

QT_BEGIN_NAMESPACE
class QTextDocument;
class QQuickTextDocument;
struct oneDocument;
QT_END_NAMESPACE

class DocumentHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQuickTextDocument *document READ document WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(int selectionStart READ selectionStart WRITE setSelectionStart NOTIFY selectionStartChanged)
    Q_PROPERTY(int selectionEnd READ selectionEnd WRITE setSelectionEnd NOTIFY selectionEndChanged)

    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment NOTIFY alignmentChanged)

    Q_PROPERTY(bool bold READ bold WRITE setBold NOTIFY boldChanged)
    Q_PROPERTY(bool italic READ italic WRITE setItalic NOTIFY italicChanged)
    Q_PROPERTY(bool underline READ underline WRITE setUnderline NOTIFY underlineChanged)

    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

    Q_PROPERTY(QString fileName READ fileName NOTIFY fileUrlChanged)
    Q_PROPERTY(QString fileType READ fileType NOTIFY fileUrlChanged)
    Q_PROPERTY(QUrl fileUrl READ fileUrl NOTIFY fileUrlChanged)

    Q_PROPERTY(int tabNumber READ tabNumber WRITE setTabNumber NOTIFY tabNumberChanged)
    Q_PROPERTY(int isUntitled READ isUntitled WRITE setUntitled NOTIFY isUntitledChanged)
    Q_PROPERTY(int isSaved READ isSaved WRITE setSaved NOTIFY isSavedChanged)

public:
    explicit DocumentHandler(QObject *parent = nullptr);

    QQuickTextDocument *document() const;
    void setDocument(QQuickTextDocument *document);

    int cursorPosition() const;
    void setCursorPosition(int position);

    int selectionStart() const;
    void setSelectionStart(int position);

    int selectionEnd() const;
    void setSelectionEnd(int position);

    QString fontFamily() const;
    void setFontFamily(const QString &family);

    QColor textColor() const;
    void setTextColor(const QColor &color);

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment alignment);

    bool bold() const;
    void setBold(bool bold);

    bool italic() const;
    void setItalic(bool italic);

    bool underline() const;
    void setUnderline(bool underline);

    int fontSize() const;
    void setFontSize(int size);

    QString fileName(QUrl *url=nullptr) const;
    QString fileType() const;
    QUrl fileUrl() const;

    int tabNumber() const;
    void setTabNumber(int num);

    bool isUntitled() const;
    void setUntitled(bool b);

    bool isSaved() const;
    void setSaved(bool b);

    Q_INVOKABLE int docCount() const;
    Q_INVOKABLE void closeDocument();
    Q_INVOKABLE void closeAllDocuments();
    Q_INVOKABLE void uploadDocument(const QString &docUrl);
//    Q_INVOKABLE void saveAll();
    Q_INVOKABLE QString urlByTab(const int num) const;
    Q_INVOKABLE QString toSaveList();
    Q_INVOKABLE bool hasUnsaved() const;

public Q_SLOTS:
    void load(const QUrl &fileUrl);
    void saveAs(const QUrl &fileUrl);
    void save();

Q_SIGNALS:
    void documentChanged();
    void cursorPositionChanged();
    void selectionStartChanged();
    void selectionEndChanged();

    void fontFamilyChanged();
    void textColorChanged();
    void alignmentChanged();

    void boldChanged();
    void italicChanged();
    void underlineChanged();

    void fontSizeChanged();

    void textChanged();
    void fileUrlChanged();

    void loaded(const QString &text);
    void error(const QString &message);

    void tabNumberChanged(int num);
    void isUntitledChanged(bool b);
    void isSavedChanged(bool b);
    void documentClosed();
private:
    void reset();
    QTextCursor textCursor() const;
    QTextDocument *textDocument() const;
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

    void updateTabBar();
    void bufferCurDocument();

    //Значения, определяющие панель  закладок
    QStringListModel m_tabBarModel;
    //m_tabBarModel.setStringList(m_docsNameList);
    QList<oneDocument*> m_documentsList;
    QStringList m_docsNameList; //список url документов
    mutable int m_untitledCount;
    int m_currentTab;

    //Значения, определяющие документ
    QQuickTextDocument *m_document;

    bool untitled;
    bool saved;

    int m_cursorPosition;
    int m_selectionStart;
    int m_selectionEnd;

    QFont m_font;
    int m_fontSize;
    QUrl m_fileUrl;
    QSyntaxHighlighter *m_syntaxHighlighter;
};

struct oneDocument {
    QString document;

    bool untitled;
    bool saved;

    int cursorPosition;
    int selectionStart;
    int selectionEnd;

    QFont font;
    int fontSize;
    QUrl fileUrl;
    QSyntaxHighlighter *syntaxHighlighter;
};

#endif // DOCUMENTHANDLER_H
