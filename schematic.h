#ifndef SCHEMATIC_H
#define SCHEMATIC_H

#include <QWidget>
#include <memory>

class PictureModel;

class Schematic : public QWidget
{
    Q_OBJECT

    std::unique_ptr<PictureModel> m_model;
public:
    explicit Schematic(QWidget *parent = nullptr);
    void installPicture(std::unique_ptr<PictureModel> && model);
    PictureModel * getPicture() const;
    bool setTextByName(const QString& name, const QString& text);

    ~Schematic();
signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // SCHEMATIC_H
