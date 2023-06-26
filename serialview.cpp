#include "serialview.h"

SerialView::SerialView(QWidget *parent)
    : QAbstractItemView{parent}
{
}

QModelIndex SerialView::indexAt(const QPoint &point) const
{

    return QModelIndex();
}

void SerialView::scrollTo(const QModelIndex &index, ScrollHint hint)
{

}

QRect SerialView::visualRect(const QModelIndex &index) const
{

    return QRect(50,50,20,20);
}

void SerialView::setModel(QAbstractItemModel *model)
{
    m_columnShow = QList<bool>(model->columnCount(),true);
    viewport()->update();
    QAbstractItemView::setModel(model);
}

void SerialView::setColumnShow(int n, bool b)
{
    if(n<0 || n>= m_columnShow.size())
        return;
    m_columnShow[n] = b;
}

int SerialView::horizontalOffset() const
{
    return 0;
}

bool SerialView::isIndexHidden(const QModelIndex &index) const
{
    return false;
}

QModelIndex SerialView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    return QModelIndex();
}

void SerialView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags)
{
    //selectionModel()->select()
}

int SerialView::verticalOffset() const
{
    return 0;
}

QRegion SerialView::visualRegionForSelection(const QItemSelection &selection) const
{
    return QRegion();
}
