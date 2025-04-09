#ifndef OASCALERMAIN_H
#define OASCALERMAIN_H

#include <QMainWindow>
#include "resistance.h"

QT_BEGIN_NAMESPACE
namespace Ui { class OAScalerMain; }
QT_END_NAMESPACE

class OAScalerMain : public QMainWindow
{
    Q_OBJECT

public:
    OAScalerMain(QWidget *parent = nullptr);
    ~OAScalerMain();
signals:

public slots:
    void slotSomebodyChanged();
private:
    Ui::OAScalerMain *ui;
};
#endif // OASCALERMAIN_H
