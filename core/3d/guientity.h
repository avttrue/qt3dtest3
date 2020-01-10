#ifndef GUIENTITY_H
#define GUIENTITY_H

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QText2DEntity>

class GuiEntity : public Qt3DCore::QEntity
{
public:
    GuiEntity(Qt3DCore::QEntity *parent);

public Q_SLOT:
        void slotMatrix(const QMatrix4x4 &matrix);

private:
    Qt3DCore::QTransform* m_Transform;
};

class TextEntity : public Qt3DCore::QEntity
{
public:
    TextEntity(Qt3DCore::QEntity *parent,
               int size,
               const QString& text,
               const QColor &color = Qt::white,
               const QString &family = "monospace",
               int weight = QFont::Bold);
    Qt3DCore::QTransform* Transform() const;
    void setTransform(Qt3DCore::QTransform *transform);
    void setText(const QString& text);
    void setTextWeight(int value);

protected:
    void resize();

private:
    Qt3DCore::QTransform* m_Transform;
    Qt3DExtras::QText2DEntity* m_Text2DEntity;
    QFont m_Font;
    Qt3DExtras::QText2DEntity* m_Text2D;
};

#endif // GUIENTITY_H
