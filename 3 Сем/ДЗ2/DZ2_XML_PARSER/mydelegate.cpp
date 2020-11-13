#include "mydelegate.h"


myDelegate::myDelegate (QObject *parent)
    :QItemDelegate(parent)
{
}

QWidget *myDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    LineEdit *dlg = new LineEdit (index, parent); //создаем наше поле ввода
    return dlg;
}

void myDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //в этой функции на входе данные из модели и указатель на виджет редактора
    QString value = index.model()->data(index).toString(); //получаем что уже есть в модели
    LineEdit *fileDialog = static_cast<LineEdit*> (editor); //преобразуем указатель
    fileDialog->setText(value); //устанавливаем текст
}

void myDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //сюда попадаем когда редактор делегата закрывается
    LineEdit *fileDialog = static_cast<LineEdit*> (editor);
    model->setData(index, fileDialog->text()); //устанавливаем новое значение в модель
}
