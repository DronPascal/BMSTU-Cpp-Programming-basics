#ifndef SAXMODEL_H
#define SAXMODEL_H

#include <QIcon>
#include <QXmlDefaultHandler>

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

class XbelHandler : public QXmlDefaultHandler
{
public:
    XbelHandler(QTreeWidget *treeWidget);

    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes) override;
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName) override;
    bool characters(const QString &str) override;
    bool fatalError(const QXmlParseException &exception) override;
    QString errorString() const override;

private:
    QTreeWidgetItem *createChildItem(const QString &tagName);

    QTreeWidget *treeWidget;
    QTreeWidgetItem *item;
    QString currentText;
    QString errorStr;
    bool metXbelTag;

    QIcon folderIcon;
    QIcon bookmarkIcon;
};

#endif // SAXMODEL_H
