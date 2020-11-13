#ifndef UNIVERSALHIGHLIGHTER_H
#define UNIVERSALHIGHLIGHTER_H
#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>
#include <QtGui>
#include <QFile>
#include <QtXml/QXmlReader>
#include <QQmlFile>

class universalHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    bool keywordsNotFounded = false;
    universalHighlighter(const QUrl &xmlHighlighterPath, const QString &fileExtention, QTextDocument *parent = nullptr) : QSyntaxHighlighter(parent)
    {
        qDebug() << fileExtention;
        const QString fileName = QQmlFile::urlToLocalFileOrQrc(xmlHighlighterPath);
        QFile file(fileName);
        bool readingExtentions = false;
        bool readingSettings = false;
        bool readingKeywords = false;
        QString keywords;
        if (file.open(QIODevice::ReadOnly))
        {
            QXmlStreamReader sr(&file);
            do {
                sr.readNext();
                if (sr.name() == "extentions")
                {
                    if (sr.tokenString() == "StartElement")
                    {
                        readingExtentions = true;
                        continue;
                    }
                    else readingExtentions =false;
                }
                if (readingExtentions)
                {
                    if (sr.tokenString() == "Characters" && sr.text().toString().contains(fileExtention))
                        readingSettings = true;
                    else readingExtentions = false;
                }
                if (readingSettings)
                {
                    if (sr.name() == "keywords")
                    {
                        if (sr.tokenString() == "StartElement")
                        {
                            readingKeywords = true;
                            continue;
                        }
                    }
                    if (readingKeywords)
                    {
                        if (sr.tokenString() == "Characters")
                            keywords = sr.text().toString();
                        if (sr.tokenString() == "EndElement")
                        {
                            readingSettings = false;
                            readingKeywords = false;
                            continue;
                        }
                    }
                }
            } while (!sr.atEnd());

            if (sr.hasError()) {
                qDebug() << "Error: " << sr.errorString();
            }
        }
        if (keywords == "")
        {
            keywordsNotFounded = true;
            qDebug() << "Extention is: " << fileExtention << " Keywords not founded";
            return;
        }
        else qDebug() << "Extention is: " << fileExtention << "Keywords is: " << keywords;

        HighlightingRule rule;
        //ключевые слова
        keywordFormat.setForeground(QColor(153,7,134));
        keywordFormat.setFontWeight(QFont::Bold);
        QStringList keywordPatterns = keywords.split("/n");
        foreach (const QString &pattern, keywordPatterns) {
            rule.pattern = QRegExp(QString("\\b%1\\b").arg(pattern));
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }
        //однострочный комментарий
        singleLineCommentFormat.setForeground(Qt::darkGreen);
        rule.pattern = QRegExp("//[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);
        //многострочные комментарии
        multiLineCommentFormat.setForeground(Qt::darkGreen);
        commentStartExpression = QRegExp("/\\*");
        commentEndExpression = QRegExp("\\*/");
        //в одинарных кавычках
        quotationFormat.setForeground(QColor(192,70,67));
        rule.pattern = QRegExp("'[^']*'");
        rule.format = quotationFormat;
        highlightingRules.append(rule);
        //в <>
        quotationFormat.setForeground(QColor(192,70,67));
        rule.pattern = QRegExp("<[^<>]*>");
        rule.format = quotationFormat;
        highlightingRules.append(rule);
        //в двойных кавычках
        quotationFormat.setForeground(QColor(192,70,67));
        rule.pattern = QRegExp("\"[^\"]*\"");
        rule.format = quotationFormat;
        highlightingRules.append(rule);
        //функции
        functionFormat.setForeground(QColor(153,7,134));
        rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
        rule.format = functionFormat;
        highlightingRules.append(rule);
    }

protected:
    void highlightBlock(const QString &text)
    {
        foreach (const HighlightingRule &rule, highlightingRules) {
            QRegExp expression(rule.pattern);
            int index = expression.indexIn(text);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, rule.format);
                index = expression.indexIn(text, index + length);
            }
        }
        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = commentStartExpression.indexIn(text);

        while (startIndex >= 0) {
            int endIndex = commentEndExpression.indexIn(text, startIndex);
            int commentLength;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex
                                + commentEndExpression.matchedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
        }
    }

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

#endif // UNIVERSALHIGHLIGHTER_H
