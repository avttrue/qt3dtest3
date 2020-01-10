#ifndef TEXTENTITY_H
#define TEXTENTITY_H

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QText2DEntity>

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

#endif // TEXTENTITY_H
