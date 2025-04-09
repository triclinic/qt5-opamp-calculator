#include "schematic.h"
#include <QPainter>

Schematic::Schematic(QWidget *parent) : QWidget(parent)
{

}

void Schematic::installPicture(std::unique_ptr<PictureModel> && model)
{
    m_model.swap(model);
    setMinimumSize(m_model->size());
    setMaximumSize(m_model->size());
}

PictureModel * Schematic::getPicture() const
{
    return m_model.get();
}

void Schematic::paintEvent(QPaintEvent *)
{
    if (!m_model) return;
    QPainter painter(this);
    m_model->render(painter);
}
