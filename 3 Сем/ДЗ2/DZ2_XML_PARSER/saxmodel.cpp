#include "saxmodel.h"

#include <QtWidgets>

static inline QString versionAttribute() { return QStringLiteral("version"); }
static inline QString hrefAttribute() { return QStringLiteral("href"); }
static inline QString foldedAttribute() { return QStringLiteral("folded"); }

XbelHandler::XbelHandler(QTreeWidget *treeWidget)
    : treeWidget(treeWidget)
{
    item = nullptr;
    metXbelTag = false;

    QStyle *style = treeWidget->style();

    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    bookmarkIcon.addPixmap(style->standardPixmap(QStyle::SP_FileIcon));
}

bool XbelHandler::startElement(const QString & /* namespaceURI */,
                               const QString & /* localName */,
                               const QString &qName,
                               const QXmlAttributes &attributes)
{
    if (!metXbelTag && qName != QLatin1String("xbel")) {
        errorStr = QObject::tr("The file is not an XBEL file.");
        return false;
    }

    if (qName == QLatin1String("xbel")) {
        QString version = attributes.value(versionAttribute());
        if (!version.isEmpty() && version != QLatin1String("1.0")) {
            errorStr = QObject::tr("The file is not an XBEL version 1.0 file.");
            return false;
        }
        metXbelTag = true;
    } else if (qName == QLatin1String("folder")) {
        item = createChildItem(qName);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setIcon(0, folderIcon);
        item->setText(0, QObject::tr("Folder"));
        bool folded = (attributes.value(foldedAttribute()) != QLatin1String("no"));
        item->setExpanded(!folded);
    } else if (qName == QLatin1String("bookmark")) {
        item = createChildItem(qName);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setIcon(0, bookmarkIcon);
        item->setText(0, QObject::tr("Unknown title"));
        item->setText(1, attributes.value(hrefAttribute()));
    } else if (qName == QLatin1String("separator")) {
        item = createChildItem(qName);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setText(0, QString(30, 0xB7));
    }

    currentText.clear();
    return true;
}

bool XbelHandler::endElement(const QString & /* namespaceURI */,
                             const QString & /* localName */,
                             const QString &qName)
{
    if (qName == QLatin1String("title")) {
        if (item)
            item->setText(0, currentText);
    } else if (qName == QLatin1String("folder") || qName == QLatin1String("bookmark")
               || qName == QLatin1String("separator")) {
        item = item->parent();
    }
    return true;
}

bool XbelHandler::characters(const QString &str)
{
    currentText += str;
    return true;
}

bool XbelHandler::fatalError(const QXmlParseException &exception)
{
    QMessageBox::information(treeWidget->window(), QObject::tr("SAX Bookmarks"),
                             QObject::tr("Parse error at line %1, column %2:\n"
                                         "%3")
                             .arg(exception.lineNumber())
                             .arg(exception.columnNumber())
                             .arg(exception.message()));
    return false;
}

QString XbelHandler::errorString() const
{
    return errorStr;
}

QTreeWidgetItem *XbelHandler::createChildItem(const QString &tagName)
{
    QTreeWidgetItem *childItem;
    if (item) {
        childItem = new QTreeWidgetItem(item);
    } else {
        childItem = new QTreeWidgetItem(treeWidget);
    }
    childItem->setData(0, Qt::UserRole, tagName);
    return childItem;
}
