#include "sceneentity.h"
#include "scene.h"
#include "properties.h"
#include "helpers3d.h"
#include "material.h"

#include <Qt3DRender/QObjectPicker>

SceneEntity::SceneEntity(Scene *parent,
                         Qt3DRender::QGeometryRenderer *geometry,
                         Qt3DRender::QMaterial *material,
                         Qt3DCore::QTransform *transform) :
    Qt3DCore::QEntity(parent),
    m_Scene(parent),
    m_Geometry(geometry),
    m_Material(material),
    m_Transform(transform),
    m_SelectionBox(nullptr)
{
    if(!geometry || !material || !transform) { qCritical() << __func__ << ": SceneEntity init error"; return; }

    m_Picker = new Qt3DRender::QObjectPicker;
    m_Picker->setHoverEnabled(false);
    m_Picker->setEnabled(true);

    addComponent(m_Geometry);
    addComponent(m_Material);
    addComponent(transform);
    addComponent(m_Picker);

    QObject::connect(m_Geometry->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, [=](){
        if (m_SelectionBox) createSelectionBox();
        QObject::disconnect(m_Geometry->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, nullptr, nullptr); });

    QObject::connect(this, &QObject::destroyed, [=]() { qDebug() << objectName() << ": destroyed"; });
    QObject::connect(m_Picker, &Qt3DRender::QObjectPicker::clicked, this, &SceneEntity::slotClicked, Qt::DirectConnection);
}

void SceneEntity::slotClicked(Qt3DRender::QPickEvent *event)
{
    qDebug() << objectName() << ": clicked";
    emit signalClicked(event, this);
}

void SceneEntity::createSelectionBox()
{
    auto max = m_Geometry->geometry()->maxExtent() + BOX_EXCESS;
    auto min = m_Geometry->geometry()->minExtent() - BOX_EXCESS;
    if(m_SelectionBox)
    {
        m_SelectionBox->setEnabled(false);
        m_SelectionBox->deleteLater();
    }
    m_SelectionBox = createEntityBox(min, max, QColor(COLOR_SELECT), this);
}

void SceneEntity::applyGeometry(Qt3DRender::QGeometryRenderer* geometry, float diagonal)
{
    if(!geometry) { qCritical() << objectName() << "(" << __func__ << "): Wrong geometry";  return; }

    applyEntityGeometry(this, geometry);
    m_Geometry = geometry;

    auto func = [=]()
    {
        if(diagonal > 0.0f)
        {
            auto gd = getGeometryDiagonal(m_Geometry->geometry());
            if(gd > 0.0f) m_Transform->setScale(diagonal * m_Transform->scale() / gd);
            else qCritical() << objectName() << "(" << __func__ << "): Diagonal of new geometry is 0.0";
        }
        if (m_SelectionBox) createSelectionBox();
        QObject::disconnect(m_Geometry->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, nullptr, nullptr);
    };

    QObject::connect(m_Geometry->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, func);
}

void SceneEntity::applyGeometry(const QString &name)
{
    auto gr = m_Scene->Geometries().value(name);

    if(!gr){ qCritical() << objectName() << "(" << __func__ << "): Wrong geometry name:" << name;  return; }

    auto diagonal = getGeometryDiagonal(m_Geometry->geometry());
    applyGeometry(gr, diagonal);
}

void SceneEntity::applyMaterial(Qt3DRender::QMaterial *material)
{
    if(!material) { qCritical() << objectName() << "(" << __func__ << "): Wrong material";  return; }

    applyEntityMaterial(this, material);
    m_Material = material;
}

void SceneEntity::applyMaterial(const QString &name)
{
    auto m = m_Scene->Materials().value(name);
    if(!m) { qCritical() << objectName() << "(" << __func__ << "): Wrong material name";  return; }

    applyMaterial(m);
}

void SceneEntity::slotSelect(bool value)
{
    if(m_SelectionBox)
    {
        m_SelectionBox->setEnabled(false);
        m_SelectionBox->deleteLater();
        m_SelectionBox = nullptr;
    }

    if(value) createSelectionBox();

    qDebug() << objectName() << ": selection" << value;
}

Qt3DCore::QEntity *SceneEntity::SelectionBox() const { return m_SelectionBox; }
void SceneEntity::Interactive(bool value) { m_Picker->setEnabled(value); }
Qt3DRender::QGeometryRenderer *SceneEntity::Geometry() const { return m_Geometry; }
