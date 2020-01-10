#ifndef GUIENTITY_H
#define GUIENTITY_H

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QText2DEntity>

class SceneEntity;

class EntityTransform : public Qt3DCore::QEntity
{
public:
    EntityTransform(Qt3DCore::QEntity *parent);
    Qt3DCore::QTransform *Transform() const;

protected:
    Qt3DCore::QTransform* m_Transform;
};

//TODO: EntityText есть проблема с динамическим удалением объекта со сцены
class EntityText : public EntityTransform
{
public:
    EntityText(Qt3DCore::QEntity *parent,
               int size,
               const QString& text,
               const QColor &color = Qt::white,
               const QString &family = "monospace",
               int weight = QFont::Bold);
    void setText(const QString& text);
    void setTextWeight(int value);

protected:
    void resize();

private:
    QFont m_Font;
    Qt3DExtras::QText2DEntity* m_Text2DEntity;
};

class EntityBox : public EntityTransform
{
public:
    EntityBox(Qt3DCore::QEntity *parent, float excess,
              const QColor &color,
              const QVector3D &min = QVector3D(0.0f, 0.0f, 0.0f),
              const QVector3D &max = QVector3D(1.0f, 1.0f, 1.0f));
    void applyToEntity(SceneEntity* entity);

private:
    Qt3DCore::QEntity* m_Box;
    float m_Excess;
};

#endif // GUIENTITY_H
