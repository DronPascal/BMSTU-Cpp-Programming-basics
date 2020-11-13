#include "dommodel.h"
#include <QTreeView>

DomModel::DomModel(const QDomDocument &document, QObject *parent)
    : QAbstractItemModel(parent),
      domDocument(document),
      rootItem(new DomItem(domDocument, 0))
{}
DomModel::DomModel(QObject *parent)
    : QAbstractItemModel(parent)
{}

DomModel::~DomModel()
{
    delete rootItem;
}

int DomModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 3;
}

Qt::ItemFlags DomModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    if (index.row()==0 && index.column()==1)
        return QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant DomModel::headerData(int section, Qt::Orientation orientation,
                              int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Name");
            case 1:
                return tr("Attributes");
            case 2:
                return tr("Value");
            default:
                break;
        }
    }
    return QVariant();
}

QModelIndex DomModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());
    DomItem *childItem = parentItem->child(row);
        if (childItem)
            return createIndex(row, column, childItem);
        return QModelIndex();
}

int DomModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());

    return parentItem->node().childNodes().count();
}

QModelIndex DomModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    DomItem *childItem = static_cast<DomItem*>(child.internalPointer());
    DomItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

QVariant DomModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    const DomItem *item = static_cast<DomItem*>(index.internalPointer());

    const QDomNode node = item->node();
    switch (index.column()) {
            case 0:
                return node.nodeName();
            case 1:
            {
                const QDomNamedNodeMap attributeMap = node.attributes();
                QStringList attributes;
                for (int i = 0; i < attributeMap.count(); ++i) {
                    QDomNode attribute = attributeMap.item(i);
                    attributes << attribute.nodeName() + "=\""
                                  + attribute.nodeValue() + '"';
                }
                return attributes.join(' ');
            }
            case 2:
                return node.nodeValue().split('\n').join(' ');
            default:
                break;
        }
        return QVariant();
}

DomItem *DomModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        DomItem *item = static_cast<DomItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

bool DomModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        DomItem *item = getItem(index);
        bool result = item->setData(index.column(), value);

        if (result)
            emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

        return result;
    }
    return false;
}

bool DomModel::write(QIODevice *device) const
{
    const int IndentSize = 4;
    QTextStream out(device);
    domDocument.save(out, IndentSize);
    return true;
}

bool DomModel::read(QIODevice *device)
{
    domDocument.setContent(device, true);
    rootItem = new DomItem(domDocument, 0);
    return true;
}

bool DomModel::add(QIODevice *device)
{
    QDomDocument *newDoc = new QDomDocument;
    newDoc->setContent(device, 1);
    appendedDocs.append(newDoc);

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    domDocument.appendChild(newDoc->documentElement());
    rootItem= new DomItem(domDocument, 0);
    endInsertRows();

    if (domDocument.childNodes().at(domDocument.childNodes().size()) == newDoc->documentElement())
        return true;
    return false;
}

int DomModel::remove_s(const QModelIndex index)
{
    int size = domDocument.childNodes().size();
    int row = index.row();
     qDebug() << row << size;
    if (index.isValid())
    {
        if (size > 1)
        {
            beginRemoveRows(QModelIndex(), size-1, size-1);
            domDocument.removeChild(domDocument.childNodes().at(size-1));
            removeRow(row);
            endRemoveRows();
//            beginRemoveRows(QModelIndex(), row, row);
//            //this->remove(index);
//            domDocument.removeChild(domDocument.childNodes().at(row));
//            endRemoveRows();
//            beginResetModel();
//            //beginRemoveRows(QModelIndex(), row, row);
//            QDomNodeList nodeList = domDocument.childNodes();
//            int childsCount = domDocument.childNodes().size();
//            for (int i = 0; i < childsCount-2; i++)
//                domDocument.removeChild(domDocument);

//            for (int i = 0; i < appendedDocs.size(); i++)
//                if (i != row-2)
//                    domDocument.appendChild(appendedDocs.at(i)->documentElement());
//            //endRemoveRows();
//            endResetModel();
//            if (row == size-1)
//            {
//                beginRemoveRows(QModelIndex(), row, row);
//                //this->remove(index);
//                domDocument.removeChild(domDocument);
//                endRemoveRows();
//            }
//            else
//            {
//                QList<QDomNode> nodesList;
//                int size = domDocument.childNodes().size();

//                for (int i = 0; i < size-row; i++)
//                {
//                    _sleep(1000);
//                    int childsSize = domDocument.childNodes().size();
//                    nodesList.push_front(domDocument.childNodes().at(childsSize-1));

//                    beginRemoveRows(QModelIndex(), childsSize-1, childsSize-1);
//                    qDebug() << "cur ch count:" << childsSize;
//                    domDocument.removeChild(domDocument.childNodes().at(childsSize-1));
//                    removeRow(row);
//                    endRemoveRows();
//                }
//                for (int i = 0; i < nodesList.size(); i++)
//                    qDebug() << "saved: "<<nodesList.at(i).nodeName();

//                for (int i = 1; i < nodesList.size(); i++)
//                {
//                    _sleep(1000);
//                    int childsSize = domDocument.childNodes().size();
//                    beginInsertRows(QModelIndex(), childsSize, childsSize);
//                    domDocument.appendChild(nodesList.at(i));
//                    qDebug() << "inserted: "<<nodesList.at(i).nodeName();
//                    endInsertRows();
//                }
//            }

            if (domDocument.childNodes().size() != size)
                return true;
            else return false;
        }
        else
        {
            delete this;
            return -1;
        }
    }

    return false;
}

//beginResetModel();
////beginRemoveRows(QModelIndex(), row, row);
//QDomNodeList nodeList = domDocument.childNodes();
//int childsCount = domDocument.childNodes().size();
//for (int i = 0; i < childsCount-2; i++)
//    domDocument.removeChild(domDocument);

//for (int i = 0; i < appendedDocs.size(); i++)
//    if (i != row-2)
//        domDocument.appendChild(appendedDocs.at(i)->documentElement());
////endRemoveRows();
//endResetModel();
