#pragma once
#include <QAbstractItemModel>
#include <QDomDocument>
#include <QTextStream>
#include <QMessageBox>

#include "domitem.h"

class DomItem;

class DomModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit DomModel(const QDomDocument &document, QObject *parent = nullptr);
    explicit DomModel(QObject *parent = nullptr);
    ~DomModel() override;

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    bool read(QIODevice *device);
    bool write(QIODevice *device) const;
    bool add(QIODevice *device);
    int remove_s(const QModelIndex index);
private:
    DomItem *getItem(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QDomDocument domDocument;
    QList<QDomDocument *> appendedDocs;
    DomItem *rootItem;
};
