#include "entity.h"
#include "properties.h"
#include "helpers.h"

#include <Qt3DRender/QObjectPicker>
#include <QColor>

Entity::Entity(Qt3DCore::QNode *parent, Qt3DRender::QGeometryRenderer *geometry,
                                   Qt3DRender::QMaterial *material) :
    Qt3DCore::QEntity(parent),
    m_Box(nullptr),
    m_Selected(false)
{
    if(!geometry || !material) { qCritical() << __func__ << ": SceneEntity init error"; return; }

    m_Geometry = geometry;

    auto objectPicker = new Qt3DRender::QObjectPicker;
    objectPicker->setHoverEnabled(false);
    objectPicker->setEnabled(true);

    addComponent(geometry);
    addComponent(material);
    addComponent(objectPicker);

    QObject::connect(this, &QObject::destroyed, [=]() { qDebug() << objectName() << ": destroyed"; });
    QObject::connect(objectPicker, &Qt3DRender::QObjectPicker::clicked, this, &Entity::slotClicked, Qt::DirectConnection);
}

void Entity::slotClicked(Qt3DRender::QPickEvent *event)
{
    emit signalClicked(event, objectName());
}

void Entity::createBox()
{
    auto excess = QVector3D(SELECTION_BOX_EXCESS, SELECTION_BOX_EXCESS ,SELECTION_BOX_EXCESS);
    auto max = m_Geometry->geometry()->maxExtent() + excess;
    auto min = m_Geometry->geometry()->minExtent() - excess;
    if(m_Box)
    {
        m_Box->setEnabled(false);
        m_Box->deleteLater();
    }
    m_Box = createEntityBox(min, max, QColor(COLOR_SELECT), this);
}

void Entity::applyGeometry(Qt3DRender::QGeometryRenderer* geometry)
{
    applyEntityGeometry(this, geometry);
    m_Geometry = geometry;
    Select(false);
}

void Entity::Select(bool value)
{
    m_Selected = value;
    if(m_Selected) createBox();
    else if(m_Box)
    {
        m_Box->setEnabled(false);
        m_Box->deleteLater();
        m_Box = nullptr;
    }
    qDebug() << objectName() << ": selection" << m_Selected;

    emit signalSelected(this, m_Selected);
}

Qt3DCore::QEntity *Entity::Box() const { return m_Box; }
bool Entity::isSelected() const { return m_Selected; }
