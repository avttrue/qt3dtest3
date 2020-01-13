#include "sceneentity.h"
#include "scene.h"
#include "guientity.h"
#include "properties.h"
#include "helpers3d.h"
#include "material.h"

SceneEntity::SceneEntity(Scene *parent) :
    Qt3DCore::QEntity(parent),    
    m_Scene(parent),
    m_Transform(new Qt3DCore::QTransform(this)),
    m_Geometry(nullptr),
    m_Material(nullptr),
    m_Size(QVector3D(1.0, 1.0, 1.0))
{
    m_Transform->setScale3D(m_Size);

    m_Picker = new Qt3DRender::QObjectPicker;
    m_Picker->setHoverEnabled(true); // NOTE: с 5.14 необходимо для перехвата клика

    if(m_Geometry) addComponent(m_Geometry);
    if(m_Material) addComponent(m_Material);
    addComponent(m_Transform);
    addComponent(m_Picker);

    QObject::connect(this, &QObject::destroyed, [=]() { qDebug() << objectName() << ": destroyed"; });
    QObject::connect(m_Picker, &Qt3DRender::QObjectPicker::clicked, this, &SceneEntity::slotClicked, Qt::DirectConnection);
}

void SceneEntity::slotClicked(Qt3DRender::QPickEvent *event)
{
    qInfo() << objectName() << ": clicked";

    Q_EMIT m_Scene->signalEntityClicked(event, this);
}

void SceneEntity::applySize(const QVector3D &size)
{
    m_Transform->setScale3D(QVector3D(size.x(), size.y(), size.z()));
    m_Size = size;
}

void SceneEntity::applyGeometry(Qt3DRender::QGeometryRenderer* geometry)
{
    if(!geometry) { qCritical() << objectName() << "(" << __func__ << "): Wrong geometry";  return; }

    applyEntityGeometry(this, geometry);
    m_Geometry = geometry;
}

void SceneEntity::applyMaterial(Qt3DRender::QMaterial *material)
{
    if(!material) { qCritical() << objectName() << "(" << __func__ << "): Wrong material";  return; }

    applyEntityMaterial(this, material);
    m_Material = material;
}

QVector3D SceneEntity::Size() const { return m_Size; }
void SceneEntity::applyPosition(const QVector3D &position) { m_Transform->setTranslation(position); }
void SceneEntity::Interactive(bool value) { m_Picker->setEnabled(value); }
Qt3DRender::QGeometryRenderer *SceneEntity::Geometry() const { return m_Geometry; }
Qt3DRender::QMaterial *SceneEntity::Material() const { return m_Material; }
QVector3D SceneEntity::Position() { return m_Transform->translation(); }
Qt3DCore::QTransform *SceneEntity::Transform() const { return m_Transform; }
