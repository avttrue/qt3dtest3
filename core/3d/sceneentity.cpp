#include "sceneentity.h"
#include "helpers.h"

#include <Qt3DRender/QObjectPicker>

#include <QColor>

SceneEntity::SceneEntity(Qt3DCore::QNode *parent, Qt3DRender::QGeometryRenderer *geometry,
                         Qt3DRender::QMaterial *material) :
    Qt3DCore::QEntity(parent),
    m_Box(nullptr),
    m_Selected(false)
{
    if(!geometry || !material)
    {
        qCritical() << __func__ << ": SceneEntity init error";
        return;
    }

    m_Geometry = geometry;

    auto objectPicker = new Qt3DRender::QObjectPicker;
    objectPicker->setHoverEnabled(false);
    objectPicker->setEnabled(true);

    addComponent(geometry);
    addComponent(material);
    addComponent(objectPicker);

    QObject::connect(this, &QObject::destroyed, [=]() { qDebug() << objectName() << ": destroyed"; });
    QObject::connect(objectPicker, &Qt3DRender::QObjectPicker::clicked, this, &SceneEntity::slotClicked, Qt::DirectConnection);
}

void SceneEntity::slotClicked(Qt3DRender::QPickEvent *event)
{
    qDebug() << objectName() << event->button();
    emit signalClicked(event, objectName());
}

void SceneEntity::createBox()
{
    auto max = m_Geometry->geometry()->maxExtent() + QVector3D(1.0, 1.0 ,1.0);
    auto min = m_Geometry->geometry()->minExtent() - QVector3D(1.0, 1.0 ,1.0);
    if(!m_Box) m_Box = createEntityBox(min, max, QColor("#1047c7"), this);
    else
    {
        m_Box->setEnabled(false);
        m_Box->deleteLater();
        m_Box = createEntityBox(min, max, QColor("#1047c7"), this);
    }
}

void SceneEntity::applyGeometry(Qt3DRender::QGeometryRenderer* geometry)
{
    applyEntityGeometry(this, geometry);
    m_Geometry = geometry;
    Select(false);
}

void SceneEntity::Select(bool value)
{
    m_Selected = value;
    if(m_Selected) createBox();
    else if(m_Box)
    {
        m_Box->setEnabled(false);
        m_Box->deleteLater();
        m_Box = nullptr;
    }
}

Qt3DCore::QEntity *SceneEntity::Box() const { return m_Box; }
bool SceneEntity::isSelected() const { return m_Selected; }
