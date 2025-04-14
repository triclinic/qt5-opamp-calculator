#ifndef PICTUREMODEL_H
#define PICTUREMODEL_H
#include <QPainter>
#include <QMap>
#include <QPixmap>
#include <QSize>

class PictureModel
{

public:
    PictureModel();
    QSize size() const;
    bool setTextByName(const QString& name, const QString& text);
    bool isResizable() const;
    void paint(QPainter &);
protected:
    struct Position
    {
        QPoint offset;
        int angle;
        QString value;
    };
private:
    QPixmap m_background;
    QMap<QString, Position> m_marks;
};

#endif // PICTUREMODEL_H
