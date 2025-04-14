#include "oascalermain.h"
#include "resistance.h"
#include "ui_oascalermain.h"
#include <QDateTimeEdit>
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
    ui->zenerVoltageSpinBox->setValue(5.6);
    ui->outputVoltageSpinBox->setValue(9.4);

    ui->scheme->installPicture(std::unique_ptr<PictureModel>(new PictureModel));
    slotSomebodyChanged();
}

OAScalerMain::~OAScalerMain()
{
    delete ui;
}

void OAScalerMain::slotSomebodyChanged()
{
    double zI = ui->zenerCurrentSpinBox->value() / 1000;
    double zV = ui->zenerVoltageSpinBox->value();
    double outV = ui->outputVoltageSpinBox->value();

    StandartResistance sr1 = StandartResistance((outV - zV) / zI, ResistanceSeries::E48);
    ui->scheme->setTextByName("R1", sr1.toString(ResistanceStringStyle::Scematic));

    double r2pr3 = ( outV / zI );
    double r2 = r2pr3 * zV / outV;
    StandartResistance sr2 = StandartResistance(r2, ResistanceSeries::E48);
    ui->scheme->setTextByName("R2", sr2.toString(ResistanceStringStyle::Scematic));
    StandartResistance sr3 = StandartResistance(r2pr3 - r2, ResistanceSeries::E48);
    ui->scheme->setTextByName("R3", sr3.toString(ResistanceStringStyle::Scematic));

    double actV = zV * (sr2.value() + sr3.value()) / sr2.value();
    ui->actualVoltageLabel->setText(QString::number(actV, 'g', 4) + " V");
}



