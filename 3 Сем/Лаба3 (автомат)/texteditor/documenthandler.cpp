#include "documenthandler.h"

#include <QFile>
#include <QFileInfo>
#include <QFileSelector>
#include <QQmlFile>
#include <QQmlFileSelector>
#include <QQuickTextDocument>
#include <QTextCharFormat>
#include <QTextCodec>
#include <QTextDocument>
#include <QDebug>
#include <QSyntaxHighlighter>

#include <algorithm>

DocumentHandler::DocumentHandler(QObject *parent)
    : QObject(parent)
    , m_untitledCount(0)    //untitled documents count
    , m_currentTab(0)   //number of current tab
    , m_document(nullptr)   //current document
    , m_cursorPosition(-1)
    , m_selectionStart(0)
    , m_selectionEnd(0)
    , m_syntaxHighlighter(nullptr)  //current highlighter
{
    m_tabBarModel.setStringList(m_docsNameList);
}

QQuickTextDocument *DocumentHandler::document() const
{
    return m_document;
}

void DocumentHandler::setDocument(QQuickTextDocument *document)
{
    if (document == m_document)
        return;
    m_document = document;

    emit documentChanged();
}

int DocumentHandler::cursorPosition() const
{
    return m_cursorPosition;
}

void DocumentHandler::setCursorPosition(int position)
{
    if (position == m_cursorPosition)
        return;

    m_cursorPosition = position;
    emit cursorPositionChanged();
}

int DocumentHandler::selectionStart() const
{
    return m_selectionStart;
}

void DocumentHandler::setSelectionStart(int position)
{
    if (position == m_selectionStart)
        return;

    m_selectionStart = position;
    emit selectionStartChanged();
}

int DocumentHandler::selectionEnd() const
{
    return m_selectionEnd;
}

void DocumentHandler::setSelectionEnd(int position)
{
    if (position == m_selectionEnd)
        return;

    m_selectionEnd = position;
    emit selectionEndChanged();
}

QString DocumentHandler::fontFamily() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return QString();
    QTextCharFormat format = cursor.charFormat();
    return format.font().family();
}

void DocumentHandler::setFontFamily(const QString &family)
{
    QTextCharFormat format;
    format.setFontFamily(family);
    mergeFormatOnWordOrSelection(format);
    emit fontFamilyChanged();
}

QColor DocumentHandler::textColor() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return QColor(Qt::black);
    QTextCharFormat format = cursor.charFormat();
    return format.foreground().color();
}

void DocumentHandler::setTextColor(const QColor &color)
{
    QTextCharFormat format;
    format.setForeground(QBrush(color));
    mergeFormatOnWordOrSelection(format);
    emit textColorChanged();
}

Qt::Alignment DocumentHandler::alignment() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return Qt::AlignLeft;
    return textCursor().blockFormat().alignment();
}

void DocumentHandler::setAlignment(Qt::Alignment alignment)
{
    QTextBlockFormat format;
    format.setAlignment(alignment);
    QTextCursor cursor = textCursor();
    cursor.mergeBlockFormat(format);
    emit alignmentChanged();
}

bool DocumentHandler::bold() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontWeight() == QFont::Bold;
}

void DocumentHandler::setBold(bool bold)
{
    QTextCharFormat format;
    format.setFontWeight(bold ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(format);
    emit boldChanged();
}

bool DocumentHandler::italic() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontItalic();
}

void DocumentHandler::setItalic(bool italic)
{
    QTextCharFormat format;
    format.setFontItalic(italic);
    mergeFormatOnWordOrSelection(format);
    emit italicChanged();
}

bool DocumentHandler::underline() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontUnderline();
}

void DocumentHandler::setUnderline(bool underline)
{
    QTextCharFormat format;
    format.setFontUnderline(underline);
    mergeFormatOnWordOrSelection(format);
    emit underlineChanged();
}

int DocumentHandler::fontSize() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return 0;
    QTextCharFormat format = cursor.charFormat();
    return format.font().pointSize();
}

void DocumentHandler::setFontSize(int size)
{
    if (size <= 0)
        return;

    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;

    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    if (cursor.charFormat().property(QTextFormat::FontPointSize).toInt() == size)
        return;

    QTextCharFormat format;
    format.setFontPointSize(size);
    mergeFormatOnWordOrSelection(format);
    emit fontSizeChanged();
}

QString DocumentHandler::fileName(QUrl *url) const
{
    QString filePath;
    if (url)
        filePath = QQmlFile::urlToLocalFileOrQrc(*url);
    else
        filePath = QQmlFile::urlToLocalFileOrQrc(m_fileUrl);

    const QString fileName = QFileInfo(filePath).fileName();
    if (fileName.isEmpty())
        return QStringLiteral("noname");
    return fileName;
}

QString DocumentHandler::fileType() const
{
    return QFileInfo(fileName()).suffix();
}

QUrl DocumentHandler::fileUrl() const
{
    return m_fileUrl;
}

void DocumentHandler::load(const QUrl &fileUrl)
{
    if (m_docsNameList.contains(fileUrl.toString()) && fileUrl != QUrl("untitled"))
    {
        emit error(tr("File allready opened"));
        return;
    }
    QQmlEngine *engine = qmlEngine(this);
    if (!engine) {
        qWarning() << "load() called before DocumentHandler has QQmlEngine";
        return;
    }
    //buffering current doc before open new one
    if (m_docsNameList.count() > 0)
        bufferCurDocument();

    //set highlighter
    QString extention =  fileUrl.toString().right( fileUrl.toString().size()- fileUrl.toString().lastIndexOf("."));
    if (extention != "untitled")
    {
        const QUrl xmlHighlpath = QQmlFileSelector::get(engine)->selector()->select(QUrl("qrc:/syntaxHighlighting.xml"));
        m_syntaxHighlighter = new universalHighlighter(xmlHighlpath, extention, textDocument());
        if (static_cast<universalHighlighter*>(m_syntaxHighlighter)->keywordsNotFounded)
        {
            delete m_syntaxHighlighter;
            m_syntaxHighlighter = nullptr;
            qDebug() << "highlighter removed";
        }
    }

    const QUrl path = QQmlFileSelector::get(engine)->selector()->select(fileUrl);
    const QString fileName = QQmlFile::urlToLocalFileOrQrc(path);
    //open existing document
    if (QFile::exists(fileName)) {
        if (QFileInfo(fileName).fileName() == "untitled" + QString::number(m_untitledCount+1))
            m_untitledCount++;
        untitled = false;
        saved = true;
        m_fileUrl = fileUrl;
        QFile file(fileName);
        if (file.open(QFile::ReadOnly))
            emit loaded(file.readAll());
            reset();
    }
    //create new document
    else if (fileName.isEmpty())
    {
        m_fileUrl = "qrc:///untitled" + QString::number(m_untitledCount+1);
        m_untitledCount++;
        untitled = true;
        saved = true;
        emit loaded("");
    }
    emit fileUrlChanged();
    //add new document tab
    m_docsNameList.append(m_fileUrl.toString());
    m_currentTab = m_docsNameList.count()-1;
    updateTabBar();
}

void DocumentHandler::saveAs(const QUrl &fileUrl)
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return;

    const QString filePath = fileUrl.toLocalFile();
    const bool isHtml = QFileInfo(filePath).suffix().contains(QLatin1String("htm"));
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Truncate | (isHtml ? QFile::NotOpen : QFile::Text))) {
        emit error(tr("Cannot save: ") + file.errorString());
        return;
    }
    file.write((isHtml ? doc->toHtml() : doc->toPlainText()).toUtf8());
    file.close();

    m_docsNameList.replaceInStrings(m_fileUrl.toString(), fileUrl.toString());
    m_fileUrl = fileUrl;
    emit fileUrlChanged();
    updateTabBar();
}

void DocumentHandler::save()
{
    //save previously opened document
    QTextDocument *doc = textDocument();
    if (!doc)
        return;

    const QString filePath = m_fileUrl.toLocalFile();
    const bool isHtml = QFileInfo(filePath).suffix().contains(QLatin1String("htm"));
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Truncate | (isHtml ? QFile::NotOpen : QFile::Text))) {
        emit error(tr("Cannot save: ") + file.errorString());
        return;
    }
    file.write((isHtml ? doc->toHtml() : doc->toPlainText()).toUtf8());
    file.close();
}

void DocumentHandler::reset()
{
    emit fontFamilyChanged();
    emit alignmentChanged();
    emit boldChanged();
    emit italicChanged();
    emit underlineChanged();
    emit fontSizeChanged();
    emit textColorChanged();
    emit fileUrlChanged();
}

QTextCursor DocumentHandler::textCursor() const
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return QTextCursor();

    QTextCursor cursor = QTextCursor(doc);
    if (m_selectionStart != m_selectionEnd) {
        cursor.setPosition(m_selectionStart);
        cursor.setPosition(m_selectionEnd, QTextCursor::KeepAnchor);
    } else {
        cursor.setPosition(m_cursorPosition);
    }
    return cursor;
}

QTextDocument *DocumentHandler::textDocument() const
{
    if (!m_document)
        return nullptr;

    return m_document->textDocument();
}

void DocumentHandler::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
}

void DocumentHandler::updateTabBar()
{
    QQmlEngine *engine = qmlEngine(this);
    if (!engine) {
        qWarning() << "load() called before DocumentHandler has QQmlEngine";
        return;
    }
    m_tabBarModel.setStringList(m_docsNameList);
    engine->rootContext()->setContextProperty("tabBarModel", &m_tabBarModel);
    emit tabNumberChanged(m_currentTab);
}

void DocumentHandler::closeDocument()
{
    const QString fileName = m_fileUrl.toString();
    int tabNum = m_docsNameList.indexOf(fileName);
    delete m_syntaxHighlighter;
    //upload next document
    if (m_docsNameList.count() > 1)
    {
        //if current tab is last
        if (tabNum == m_docsNameList.count()-1)
            m_currentTab--;
        //delete closed document info
        m_docsNameList.removeOne(fileName);
        for (auto iter = m_documentsList.begin(); iter !=m_documentsList.end(); iter++)
            if ((*iter)->fileUrl == fileName)
            {
                uploadDocument(m_docsNameList.at(m_currentTab));
                updateTabBar();
                delete *iter;
                m_documentsList.removeAt(m_documentsList.indexOf(*iter));
                break;
            }
        uploadDocument(m_docsNameList.at(m_currentTab));
        updateTabBar();
    }
    else
    {
        for (auto iter = m_documentsList.begin(); iter != m_documentsList.end(); iter++)
           delete *iter;
        m_docsNameList.clear();
        m_documentsList.clear();
        m_untitledCount = 0;
        m_currentTab = 0;
        load(QUrl("untitled"));
    }
    emit documentClosed();
}

void DocumentHandler::closeAllDocuments()
{
    m_docsNameList.clear();
    for (int i = 0; i< m_documentsList.count(); i++)
        m_documentsList.removeAt(i);
    m_documentsList.clear();
    m_untitledCount = 0;
    m_currentTab = 0;
    load(QUrl("untitled"));
}

void DocumentHandler::bufferCurDocument()
{
    oneDocument *curDocument = nullptr;
    bool key = false;
    for (auto iter = m_documentsList.begin(); iter !=m_documentsList.end(); iter++)
        if ((*iter)->fileUrl == m_fileUrl.toString())
            curDocument = *iter;
    if (!curDocument)
    {
        curDocument = new oneDocument;
        key = true;
    }
    curDocument->document = textDocument()->toHtml();
    curDocument->untitled = untitled;
    curDocument->saved = saved;
    curDocument->cursorPosition = m_cursorPosition;
    curDocument->selectionStart = m_selectionStart;
    curDocument->selectionEnd = m_selectionEnd;
    curDocument->font = m_font;
    curDocument->fontSize = m_fontSize;
    curDocument->fileUrl = m_fileUrl;
    curDocument->syntaxHighlighter = m_syntaxHighlighter;
    if (key)
        m_documentsList.append(curDocument);

}
//сохраняем данные текущего документа и выгружаем данные документ вкладки, по которой кликнули
void DocumentHandler::uploadDocument(const QString &docUrl)
{
    bufferCurDocument();
    //uploading
    oneDocument* uploadingDocument = *std::find_if(m_documentsList.begin(), m_documentsList.end(), [docUrl](oneDocument *doc){return doc->fileUrl == docUrl;});
    untitled = uploadingDocument->untitled;
    m_cursorPosition = uploadingDocument->cursorPosition;
    m_selectionStart = uploadingDocument->selectionStart;
    m_selectionEnd = uploadingDocument->selectionEnd;
    m_font = uploadingDocument->font;
    m_fontSize = uploadingDocument->fontSize;
    m_fileUrl = uploadingDocument->fileUrl;
    //set highlighter
    QQmlEngine *engine = qmlEngine(this);
    QString extention =  m_fileUrl.toString().right(m_fileUrl.toString().size()- m_fileUrl.toString().lastIndexOf("."));
    if (!extention.contains("untitled"))
    {
        const QUrl xmlHighlpath = QQmlFileSelector::get(engine)->selector()->select(QUrl("qrc:/syntaxHighlighting.xml"));
        m_syntaxHighlighter = new universalHighlighter(xmlHighlpath, extention, textDocument());
        if (static_cast<universalHighlighter*>(m_syntaxHighlighter)->keywordsNotFounded)
        {
            delete m_syntaxHighlighter;
            m_syntaxHighlighter = nullptr;
            qDebug() << "highlighter removed";
        }
    }
    saved = uploadingDocument->saved;

    emit loaded(uploadingDocument->document);
    emit reset();
}

int DocumentHandler::tabNumber() const
{
    return m_currentTab;
}

void DocumentHandler::setTabNumber(int num)
{
    m_currentTab = num;
}

bool DocumentHandler::isUntitled() const
{
    return untitled;
}

void DocumentHandler::setUntitled(bool b)
{
    untitled = b;
}

bool DocumentHandler::isSaved() const
{
    return saved;
}

void DocumentHandler::setSaved(bool b)
{
    saved = b;
}

int DocumentHandler::docCount() const
{
    return m_docsNameList.count();
}

QString DocumentHandler::urlByTab(const int num) const
{
    return m_docsNameList.at(num);
}

QString DocumentHandler::toSaveList()
{
    QString toSave = "";
    for (auto iter = m_documentsList.begin(); iter !=m_documentsList.end(); iter++)
        if ((*iter)->saved == 0)
            toSave.append(fileName(&((*iter)->fileUrl))+ " - "+ (*iter)->fileUrl.toString() + "\n");
    if (saved == 0)
        toSave.append(fileName()+ " - "+ m_fileUrl.toString());
    return toSave;
}

bool DocumentHandler::hasUnsaved() const
{
    for (auto iter = m_documentsList.begin(); iter !=m_documentsList.end(); iter++)
        if ((*iter)->saved == 0)
        {
            qDebug() << ((*iter)->fileUrl.toString()) << " is unsaved!";
            return true;
        }
    return false;
}

