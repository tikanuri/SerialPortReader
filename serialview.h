#ifndef SERIALVIEW_H
#define SERIALVIEW_H

#include <QAbstractItemView>
#include <QObject>
#include <QWidget>

class SerialView : public QAbstractItemView
{

private:
    QList<bool> m_columnShow;
    quintptr a;


public:
    SerialView(QWidget *parent = nullptr);
    QModelIndex indexAt(const QPoint &point) const override;
    void scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint = EnsureVisible) override;
    QRect visualRect(const QModelIndex &index) const override;
    void setModel(QAbstractItemModel *model)  override;

    void setColumnShow(int n, bool b = true);

protected:
    int horizontalOffset() const override;
    bool isIndexHidden(const QModelIndex &index) const override;
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags) override;
    int verticalOffset() const override;
    QRegion visualRegionForSelection(const QItemSelection &selection) const override;
};

#endif // SERIALVIEW_H
