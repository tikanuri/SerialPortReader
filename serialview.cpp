#include "serialview.h"

SerialView::SerialView(QWidget *parent)
    : QAbstractItemView{parent}
{
    a = (quint64)parent;
}

QModelIndex SerialView::indexAt(const QPoint &point) const
{
    return QModelIndex();
}

QRect SerialView::visualRect(const QModelIndex &index) const
{
    int column = index.model()->columnCount();
    return rectList[index.column() + column*index.row()];
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

    selectionModel()->select()
}

int SerialView::verticalOffset() const
{
    return 0;
}

QRegion SerialView::visualRegionForSelection(const QItemSelection &selection) const
{

}
