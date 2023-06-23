#ifndef SERIALVIEW_H
#define SERIALVIEW_H

#include <QAbstractItemView>
#include <QObject>
#include <QWidget>

class SerialView : public QAbstractItemView
{

private:
    quintptr a;
public:
    SerialView(QWidget *parent = nullptr);

};

#endif // SERIALVIEW_H
