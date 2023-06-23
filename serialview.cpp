#include "serialview.h"

SerialView::SerialView(QWidget *parent)
    : QAbstractItemView{parent}
{
    a = (quint64)parent;
}
