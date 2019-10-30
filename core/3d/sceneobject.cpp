#include "sceneobject.h"
#include "scene.h"
#include "properties.h"
#include "helpers3d.h"

#include <Qt3DRender/QObjectPicker>
#include <QColor>

SceneObject::SceneObject(Scene* parent,
                         Qt3DRender::QGeometryRenderer *geometry,
                         Qt3DRender::QMaterial *material) :
    Qt3DCore::QEntity(parent),        
    m_Scene(parent),
    m_Geometry(geometry),
    m_Material(material),
    m_SelectionBox(nullptr)
{
    if(!geometry || !material) { qCritical() << __func__ << ": SceneEntity init error"; return; }

    auto objectPicker = new Qt3DRender::QObjectPicker;
    objectPicker->setHoverEnabled(false);
    objectPicker->setEnabled(true);

    addComponent(m_Geometry);
    addComponent(m_Material);
    addComponent(objectPicker);

    QObject::connect(m_Geometry->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, [=](){
        if (m_SelectionBox) createSelectionBox();
        QObject::disconnect(m_Geometry->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, nullptr, nullptr); });

    QObject::connect(this, &QObject::destroyed, [=]() { qDebug() << objectName() << ": destroyed"; });
    QObject::connect(objectPicker, &Qt3DRender::QObjectPicker::clicked, this, &SceneObject::slotClicked, Qt::DirectConnection);
}

void SceneObject::slotClicked(Qt3DRender::QPickEvent *event)
{
    emit signalClicked(event, objectName());
}

void SceneObject::createSelectionBox()
{
    auto excess = QVector3D(SELECTION_BOX_EXCESS, SELECTION_BOX_EXCESS ,SELECTION_BOX_EXCESS);
    auto max = m_Geometry->geometry()->maxExtent() + excess;
    auto min = m_Geometry->geometry()->minExtent() - excess;
    if(m_SelectionBox)
    {
        m_SelectionBox->setEnabled(false);
        m_SelectionBox->deleteLater();
    }
    m_SelectionBox = createEntityBox(min, max, QColor(COLOR_SELECT), this);
}

void SceneObject::applyGeometry(Qt3DRender::QGeometryRenderer* geometry)
{
    if(!geometry) { qCritical()<< objectName() << "(" << __func__ << "): Geometry is empty"; }
    applyEntityGeometry(this, geometry);
    m_Geometry = geometry;
    QObject::connect(m_Geometry->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, [=](){
        if (m_SelectionBox) createSelectionBox();
        QObject::disconnect(m_Geometry->geometry(), &Qt3DRender::QGeometry::maxExtentChanged, nullptr, nullptr); });
}

void SceneObject::slotSelect(bool value)
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

Qt3DCore::QEntity *SceneObject::Box() const { return m_SelectionBox; }

