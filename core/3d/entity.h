#ifndef ENTITY_H
#define ENTITY_H

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QPickEvent>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QGeometryRenderer>


class Entity : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    Entity(Qt3DCore::QNode* parent,
                     Qt3DRender::QGeometryRenderer* geometry,
                     Qt3DRender::QMaterial* material);
    Qt3DCore::QEntity *Box() const;
    void applyGeometry(Qt3DRender::QGeometryRenderer* geometry);
    void Select(bool value);
    bool isSelected() const;

Q_SIGNALS:
    void signalClicked(Qt3DRender::QPickEvent *event, const QString &name);
    void signalSelected(Entity* entity, bool selected);

public Q_SLOTS:
    void slotClicked(Qt3DRender::QPickEvent *event);

protected:
    void createBox();

private:
    Qt3DRender::QGeometryRenderer* m_Geometry;
    Qt3DCore::QEntity* m_Box;
    bool m_Selected;
};

#endif // ENTITY_H
