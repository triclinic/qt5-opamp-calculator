#include "picturemodel.h"

PictureModel::PictureModel()
{
    m_background.load(":/images/OA_vref_420x540.png");
    m_marks["R1"] = { QPoint(50, 365), -90, "???" };
    m_marks["R2"] = { QPoint(360, 320), -90, "???" };
    m_marks["R3"] = { QPoint(360, 445), -90, "???" };
    m_marks["VD1"] = { QPoint(205, 25), 0, "???" };
}

QSize PictureModel::size() const
{
    return m_background.rect().size();
}

void PictureModel::setValueByName(const QString& name, const QString& value)
{
    auto found = m_marks.find(name);
    if (found != m_marks.end()) {
        found.value().value = value;
    }
}

void PictureModel::render(QPainter & painter)
{
    painter.drawPixmap(m_background.rect(), m_background);
    QFont resFont;
    resFont.setPixelSize(25);
    painter.setFont(resFont);

    for(const auto& value : m_marks) {
        painter.save();
        painter.translate(value.offset);
        painter.rotate(value.angle);
        painter.drawText(QPoint(), value.value);
        painter.restore();
    }
}
