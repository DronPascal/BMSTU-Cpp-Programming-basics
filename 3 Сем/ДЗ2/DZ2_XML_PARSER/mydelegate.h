#pragma once

#include <QObject>
#include <QLineEdit>
#include <QModelIndex>
#include <QItemDelegate>

class LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    LineEdit(const QModelIndex &index, QWidget *parent = nullptr)
    {
        setParent(parent);
        indexSource = index;
    }
    ~LineEdit(){}

    QString getChoice () {return value;}

protected:
    //void resizeEvent(QResizeEvent *); //в этой функции делаем так чтобы текст не залезал под кнопку

private:
    QModelIndex indexSource;
    QString value;
};

class myDelegate : public QItemDelegate
{
public:
    myDelegate (QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const; //создаем редактор делегата - это наш виджет
    void setEditorData(QWidget *editor, const QModelIndex &index) const; //устанавливаем данные в редакторе
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const; //а здесь данные из редактора передаем уже в модель
    //void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};



