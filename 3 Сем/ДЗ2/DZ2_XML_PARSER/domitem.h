#pragma once
#include "dommodel.h"
#include <QAbstractItemView>
#include <QDebug>

class DomItem
{
public:
    DomItem(const QDomNode &node, int row, DomItem *parent = nullptr);
    ~DomItem();
    DomItem *child(int i);
    DomItem *parent();
    QDomNode node() const;
    int row() const;

    bool setData(int column, const QVariant &value);
private:
    QDomNode domNode;
    QHash<int, DomItem *> childItems;
    DomItem *parentItem;
    int rowNumber;
};

