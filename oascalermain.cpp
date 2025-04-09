#include "oascalermain.h"
#include "ui_oascalermain.h"
#include <QStringList>
#include <QDebug>

#include "picturemodel.h"
#include <memory>

OAScalerMain::OAScalerMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OAScalerMain)
{
    ui->setupUi(this);

    ui->resistorSeriesComboBox->addItem("E24");
    ui->resistorSeriesComboBox->addItem("E48");

    ui->zenerCurrentSpinBox->setValue(2.0);
    ui->vccVoltageSpinBox->setValue(15.0);
    ui->outputVoltageSpinBox->setValue(9.4);

    ui->scheme->installPicture(std::unique_ptr<PictureModel>(new PictureModel));
}

OAScalerMain::~OAScalerMain()
{
    delete ui;
}

void OAScalerMain::slotSomebodyChanged()
{
    auto pic = ui->scheme->getPicture();
    if(pic) {
        pic->setValueByName("R1", "2k2");
        pic->setValueByName("R2", "10k");
        pic->setValueByName("R3", "6k8");
        pic->setValueByName("VD1", "5V6");
        update();
    }
}



