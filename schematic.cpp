#include "picturemodel.h"
#include "schematic.h"
#include <QPainter>
#include <QWheelEvent>
#include <QDebug>

Schematic::Schematic(QWidget *parent) : QWidget(parent), m_model(nullptr)
{

}

void Schematic::installPicture(std::unique_ptr<PictureModel> && model)
{
    m_model.swap(model);
    if (not m_model->isResizable())
    {
        setMinimumSize(m_model->size());
        setMaximumSize(m_model->size());
    }
    update();
}

PictureModel * Schematic::getPicture() const
{
    return m_model.get();
}

bool Schematic::setTextByName(const QString &name, const QString &value)
{
    if (!m_model) return false;
    bool success = false;
    success = m_model->setTextByName(name, value);
    if (success) update();
    return success;
}

Schematic::~Schematic()
{

}

void Schematic::paintEvent(QPaintEvent *)
{
    if (!m_model) return;
    QPainter painter(this);
    m_model->paint(painter);
}

void Schematic::wheelEvent(QWheelEvent *event)
{
    qDebug() << "wheel:" << event->pos() << event->angleDelta();
}
