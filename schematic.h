#ifndef SCHEMATIC_H
#define SCHEMATIC_H

#include "picturemodel.h"
#include <QWidget>
#include <memory>

class Schematic : public QWidget
{
    Q_OBJECT
    std::unique_ptr<PictureModel> m_model;
public:
    explicit Schematic(QWidget *parent = nullptr);
    void installPicture(std::unique_ptr<PictureModel> && model);
    PictureModel * getPicture() const;
signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // SCHEMATIC_H
