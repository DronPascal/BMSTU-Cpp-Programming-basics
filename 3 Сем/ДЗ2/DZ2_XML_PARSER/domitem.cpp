#include "domitem.h"
#include <QDebug>

DomItem::DomItem(const QDomNode &node, int row, DomItem *parent)
    : domNode(node),
      parentItem(parent),
      rowNumber(row)
{

}

DomItem *DomItem::parent()
{
    return parentItem;
}

int DomItem::row() const
{
    return rowNumber;
}

QDomNode DomItem::node() const
{
    return domNode;
}

DomItem *DomItem::child(int i)
{
    DomItem *childItem = childItems.value(i);
    if (childItem)
        return childItem;

    // if child does not yet exist, create it
    if (i >= 0 && i < domNode.childNodes().count()) {
        QDomNode childNode = domNode.childNodes().item(i);
        childItem = new DomItem(childNode, i, this);
        childItems[i] = childItem;
    }
        //without #text
//        if (childNode.childNodes().at(0).isNull())
//        {
//            this->setData(2, childNode.nodeValue());
//            childItem = nullptr;
//        }
//        else
//        {
//            childItem = new DomItem(childNode, i, this);
//            childItems[i] = childItem;
//            childItem->child(0);
//        }
//    }
    return childItem;
}

DomItem::~DomItem()
{
    qDeleteAll(childItems);
}

bool DomItem::setData(int column, const QVariant &value)
{
    switch (column)
    {
    case 0:
    {
        QDomElement *cur_element = static_cast<QDomElement*>(&domNode);
        cur_element->setTagName(value.toString());
        break;
    }
    case 1:
    {
        QDomElement *cur_element = static_cast<QDomElement*>(&domNode);
        if (value == "")
        {
            const QDomNamedNodeMap attributeMap = cur_element->attributes();
            QStringList attributes;
            for (int i = 0; i < attributeMap.count(); ++i)
            {
                qDebug() << attributeMap.item(i).nodeName();
                cur_element->removeAttribute(attributeMap.item(i).nodeName());
            }
        }
        else
        {
            cur_element->setAttribute(value.toString().left(value.toString().indexOf('=')),
                                      value.toString().right(value.toString().indexOf('=')));
        }
        break;
    }
    case 2:
        domNode.setNodeValue(value.toString());
    }
    return true;
}
