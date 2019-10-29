#include "helpers3d.h"

#include <QUuid>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QMesh>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QText2DEntity>
#include <Qt3DCore/QTransform>


Qt3DCore::QEntity *addEntityLine(const QVector3D& start,
                                 const QVector3D& end,
                                 const QColor& color,
                                 Qt3DCore::QEntity* parent)
{
    auto geometry = new Qt3DRender::QGeometry( parent);

    // position vertices (start and end)
    QByteArray bufferBytes;
    bufferBytes.resize(3 * 2 * sizeof(float));
    float *positions = reinterpret_cast<float*>(bufferBytes.data());
    *positions++ = start.x();
    *positions++ = start.y();
    *positions++ = start.z();
    *positions++ = end.x();
    *positions++ = end.y();
    *positions++ = end.z();

    auto vertexBuffer = new Qt3DRender::QBuffer(geometry);
    vertexBuffer->setData(bufferBytes);

    auto positionAttribute = new Qt3DRender::QAttribute(geometry);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexBuffer);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(2);
    geometry->addAttribute(positionAttribute);

    // connectivity between vertices
    QByteArray indexBytes;
    indexBytes.resize(2 * sizeof(unsigned int));
    unsigned int *indices = reinterpret_cast<unsigned int*>(indexBytes.data());
    *indices++ = 0; *indices++ = 1;

    auto indexBuffer = new Qt3DRender::QBuffer(geometry);
    indexBuffer->setData(indexBytes);

    auto indexAttribute = new Qt3DRender::QAttribute(geometry);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(2);
    geometry->addAttribute(indexAttribute);

    // entity
    auto lineEntity = new Qt3DCore::QEntity(parent);

    // mesh
    auto line = new Qt3DRender::QGeometryRenderer(lineEntity);
    line->setGeometry(geometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    // material
    auto material = new Qt3DExtras:: QDiffuseSpecularMaterial(lineEntity);
    material->setAmbient(color);

    lineEntity->addComponent(line);
    lineEntity->addComponent(material);

    QObject::connect(lineEntity, &QObject::destroyed, [=]() { qDebug() << parent->objectName() << ": EntityLine destroyed"; });
    qDebug() << parent->objectName() << ": EntityLine created";

    return lineEntity;
}

Qt3DCore::QEntity *createEntityBox(const QVector3D &min,
                                   const QVector3D &max,
                                   const QColor &color,
                                   Qt3DCore::QEntity *parent)
{
    auto geometry = new Qt3DRender::QGeometry;

    // position vertices
    QByteArray bufferBytes;
    bufferBytes.resize(3 * 8 * sizeof(float));
    float *positions = reinterpret_cast<float*>(bufferBytes.data());
    *positions++ = min.x(); *positions++ = min.y(); *positions++ = min.z();
    *positions++ = max.x(); *positions++ = min.y(); *positions++ = min.z();
    *positions++ = max.x(); *positions++ = min.y(); *positions++ = max.z();
    *positions++ = min.x(); *positions++ = min.y(); *positions++ = max.z();
    *positions++ = min.x(); *positions++ = max.y(); *positions++ = min.z();
    *positions++ = max.x(); *positions++ = max.y(); *positions++ = min.z();
    *positions++ = max.x(); *positions++ = max.y(); *positions++ = max.z();
    *positions++ = min.x(); *positions++ = max.y(); *positions++ = max.z();

    auto *vertexBuffer = new Qt3DRender::QBuffer(geometry);
    vertexBuffer->setData(bufferBytes);

    auto *positionAttribute = new Qt3DRender::QAttribute(geometry);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexBuffer);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(8);
    geometry->addAttribute(positionAttribute);

    // connectivity between vertices
    QByteArray indexBytes;
    indexBytes.resize(24 * sizeof(unsigned int)); // start to end
    unsigned int *indices = reinterpret_cast<unsigned int*>(indexBytes.data());

    *indices++ = 0; *indices++ = 1;
    *indices++ = 1; *indices++ = 2;
    *indices++ = 2; *indices++ = 3;
    *indices++ = 3; *indices++ = 0;
    *indices++ = 0; *indices++ = 4;
    *indices++ = 1; *indices++ = 5;
    *indices++ = 2; *indices++ = 6;
    *indices++ = 3; *indices++ = 7;
    *indices++ = 4; *indices++ = 5;
    *indices++ = 5; *indices++ = 6;
    *indices++ = 6; *indices++ = 7;
    *indices++ = 7; *indices++ = 4;

    auto indexBuffer = new Qt3DRender::QBuffer(geometry);
    indexBuffer->setData(indexBytes);

    auto indexAttribute = new Qt3DRender::QAttribute(geometry);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(24);
    geometry->addAttribute(indexAttribute);

    geometry->setParent(parent);

    // entity
    auto lineEntity = new Qt3DCore::QEntity(parent);

    // mesh
    auto line = new Qt3DRender::QGeometryRenderer(lineEntity);
    line->setGeometry(geometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    // material
    auto material = new Qt3DExtras::QDiffuseSpecularMaterial(lineEntity);
    material->setAmbient(color);

    lineEntity->addComponent(line);
    lineEntity->addComponent(material);

    QObject::connect(lineEntity, &QObject::destroyed, [=]() { qDebug() << parent->objectName() << ": EntityBox destroyed"; });
    qDebug() << parent->objectName() << ": EntityBox created";
    return lineEntity;
}

Qt3DCore::QEntity* addText(Qt3DCore::QEntity* parent, const QString& text)
{
    auto textEntity = new Qt3DCore::QEntity(parent);
    auto text2D = new Qt3DExtras::QText2DEntity(textEntity);
    text2D->setFont(QFont("monospace"));
    text2D->setHeight(20.0);
    text2D->setWidth(50.0);
    text2D->setText(text);
    text2D->setColor(Qt::yellow);

    auto text2dTransform = new Qt3DCore::QTransform;
    text2dTransform->setTranslation(QVector3D(0, 0, 0));

    textEntity->addComponent(text2dTransform);
    return textEntity;
}

void applyEntityName(Qt3DCore::QEntity *entity, const QString& prefix, const QString &name)
{
    auto newname = name;
    if (newname.isEmpty()) newname = QString("%1-%2").arg(prefix, QUuid::createUuid().toString());
    entity->setObjectName(newname);
}

void applyEntityGeometry(Qt3DCore::QEntity* e, Qt3DRender::QGeometryRenderer *gr)
{
    if(!e || !gr) { qCritical() << __func__ << ": Wrong parameters"; return; }

    // searching old geometry and meshes
    QVector<Qt3DCore::QComponent*> vc;
    for(Qt3DCore::QComponent* c: e->components())
    {
        if(qobject_cast<Qt3DRender::QGeometryRenderer*>(c) || qobject_cast<Qt3DRender::QMesh*>(c))
            vc.append(c);
    }

    // apply new geometry
    e->addComponent(gr);
    QObject::connect(gr, &QObject::destroyed, [=](){ qDebug() << e->objectName() << ": Geometry destroyed"; });

    // delete old geometry and meshes
    for(Qt3DCore::QComponent* c: vc)
    {
        c->setEnabled(false);
        e->removeComponent(c);
        c->deleteLater();
    }
    qDebug() << e->objectName() << ": Geometry applied";
}
