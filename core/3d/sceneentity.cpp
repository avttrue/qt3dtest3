#include "sceneentity.h"
#include "scene.h"
#include "properties.h"
#include "helpers3d.h"
#include "material.h"

#include <Qt3DRender/QObjectPicker>

SceneEntity::SceneEntity(Scene *parent) :
    Qt3DCore::QEntity(parent),    
    m_Scene(parent),
    m_Transform(new Qt3DCore::QTransform(this)),
    m_Geometry(nullptr),
    m_Material(nullptr),
    m_SelectionBox(nullptr),
    m_Size(QVector3D(1.0, 1.0, 1.0))
{
    m_Transform->setScale3D(m_Size);

    m_Picker = new Qt3DRender::QObjectPicker;
    m_Picker->setHoverEnabled(true); // NOTE: с 5.14 необходимо для перехвата клика

    if(m_Geometry)
    {
        addComponent(m_Geometry);
        QObject::connect(m_Geometry->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, [=](){
            if (m_SelectionBox) createSelectionBox();
            QObject::disconnect(m_Geometry->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, nullptr, nullptr); });
    }
    if(m_Material) addComponent(m_Material);
    addComponent(m_Transform);
    addComponent(m_Picker);

    QObject::connect(this, &QObject::destroyed, [=]() { qDebug() << objectName() << ": destroyed"; });
    QObject::connect(m_Picker, &Qt3DRender::QObjectPicker::clicked, this, &SceneEntity::slotClicked, Qt::DirectConnection);
}

void SceneEntity::slotClicked(Qt3DRender::QPickEvent *event)
{
    qDebug() << objectName() << ": clicked";

    Q_EMIT m_Scene->signalEntityClicked(event, this);
}

void SceneEntity::createSelectionBox()
{
    auto max = m_Geometry->geometry()->maxExtent() + QVector3D(config->SceneExcess(), config->SceneExcess(), config->SceneExcess());
    auto min = m_Geometry->geometry()->minExtent() - QVector3D(config->SceneExcess(), config->SceneExcess(), config->SceneExcess());

   if(m_SelectionBox) deleteEntity(m_SelectionBox);

    m_SelectionBox = createEntityBox(min, max, QColor(config->SceneColorSelect()), this);
    m_SelectionBox->setObjectName("SelectionBox");
    m_SelectionBox->addComponent(m_Scene->View()->InterfaceLayer());
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

    auto func = [=]()
    {
        if (m_SelectionBox) createSelectionBox();
        QObject::disconnect(m_Geometry->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, nullptr, nullptr);
    };

    QObject::connect(m_Geometry->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, func);
}

void SceneEntity::applyMaterial(Qt3DRender::QMaterial *material)
{
    if(!material) { qCritical() << objectName() << "(" << __func__ << "): Wrong material";  return; }

    applyEntityMaterial(this, material);
    m_Material = material;
}

void SceneEntity::slotSelect(bool value)
{
    if(m_SelectionBox) m_SelectionBox->setEnabled(false);

    if(value) createSelectionBox();

    qDebug() << objectName() << ": selection" << value;
}

QVector3D SceneEntity::Size() const { return m_Size; }
void SceneEntity::applyPosition(const QVector3D &position) { m_Transform->setTranslation(position); }
Qt3DCore::QEntity *SceneEntity::SelectionBox() const { return m_SelectionBox; }
void SceneEntity::Interactive(bool value) { m_Picker->setEnabled(value); }
Qt3DRender::QGeometryRenderer *SceneEntity::Geometry() const { return m_Geometry; }
Qt3DRender::QMaterial *SceneEntity::Material() const { return m_Material; }
QVector3D SceneEntity::Position() { return m_Transform->translation(); }
