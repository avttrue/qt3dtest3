#include "scene.h"
#include "sceneentity.h"
#include "helpers.h"

#include <Qt3DExtras/QCuboidMesh>

Scene::Scene(Qt3DExtras::Qt3DWindow *view, const QString &name)
{
    applyEntityName(this, "scene", name);
    view->setRootEntity(this);

    QObject::connect(this, &QObject::destroyed, [=](QObject* o){ qDebug() << o->objectName() << ": destroyed"; });
}

SceneEntity* Scene::createEntity(Qt3DRender::QGeometryRenderer *geometry,
                                 Qt3DRender::QMaterial *material,
                                 const QString &name)
{
    auto entity = new SceneEntity(this, geometry, material);
    applyEntityName(entity, "entity", name);
    m_Entities.insert(entity->objectName(), entity);

    QObject::connect(entity, &SceneEntity::signalClicked, this, &Scene::slotEntityClicked, Qt::DirectConnection);

    qDebug() << objectName() << ": Entity created, count =" << m_Entities.count();
    return entity;
}

void Scene::deleteEntity(const QString &name)
{
    auto e = m_Entities.take(name);
    if(!e) { qCritical() << __func__ << ": Entity <" << name << "> not found"; return; }

    e->setEnabled(false);
    e->deleteLater();
}

void Scene::slotEntityClicked(Qt3DRender::QPickEvent *event, const QString &name)
{
    auto e = Entities().value(name);
    if(!e) { qCritical() << __func__ << ": Entity <" << name << "> not found"; return; }

    if(event->button() == Qt3DRender::QPickEvent::Buttons::LeftButton)
    {
        auto cm = new Qt3DExtras::QCuboidMesh;
        cm->setXExtent(20);
        cm->setYExtent(20);
        cm->setZExtent(20);
        e->applyGeometry(cm);
    }
    else if(event->button() == Qt3DRender::QPickEvent::Buttons::MiddleButton)
    {
        e->Select(!e->isSelected());
    }
    else if(event->button() == Qt3DRender::QPickEvent::Buttons::RightButton)
    {
        deleteEntity(name);
    }
}

QHash<QString, SceneEntity *> Scene::Entities() const { return m_Entities; }
