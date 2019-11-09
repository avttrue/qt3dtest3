#include "helpers3d.h"

#include <QUuid>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QText2DEntity>

Qt3DCore::QEntity *createEntityLine(const QVector3D& start,
                                    const QVector3D& end,
                                    const QColor& color,
                                    Qt3DCore::QEntity* parent)
{
    auto lineEntity = new Qt3DCore::QEntity(parent);
    QObject::connect(lineEntity, &QObject::destroyed, [=]() { qDebug() << parent->objectName() << ": EntityLine destroyed"; });

    auto geometry = new Qt3DRender::QGeometry(lineEntity);

    QByteArray bufferBytes;
    bufferBytes.resize(3 * 2 * sizeof(float));
    float *positions = reinterpret_cast<float*>(bufferBytes.data());
    *positions++ = start.x(); *positions++ = start.y(); *positions++ = start.z();
    *positions++ = end.x(); *positions++ = end.y(); *positions++ = end.z();

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

    // mesh
    auto line = new Qt3DRender::QGeometryRenderer;
    line->setGeometry(geometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    // material
    auto material = new Qt3DExtras:: QDiffuseSpecularMaterial;
    material->setAmbient(color);

    lineEntity->addComponent(line);
    lineEntity->addComponent(material);

    qDebug() << parent->objectName() << ": EntityLine created";
    return lineEntity;
}

Qt3DCore::QEntity *createEntityBottomGrid(const QVector3D& start,
                                          const QVector3D& end,
                                          float cell,
                                          const QColor& color,
                                          Qt3DCore::QEntity* parent)
{
    auto lineEntity = new Qt3DCore::QEntity(parent);
    QObject::connect(lineEntity, &QObject::destroyed, [=]() { qDebug() << parent->objectName() << ": EntityHGrid destroyed"; });

    auto width = static_cast<unsigned int>(abs(end.x() - start.x()) / cell);
    auto depth = static_cast<unsigned int>(abs(end.z() - start.z()) / cell);
    if(width <= 0 || depth <= 0)
    {
        qDebug() << parent->objectName() << ": EntityHGrid created EMPTY";
        return lineEntity;
    }
    auto geometry = new Qt3DRender::QGeometry(lineEntity);

    QByteArray bufferBytes;
    bufferBytes.resize(static_cast<int>(3 * 2 * (width + depth) * sizeof(float)));
    float *positions = reinterpret_cast<float*>(bufferBytes.data());

    for(unsigned int w = 0; w < width; w++)
    {
        *positions++ = start.x() + w * cell; *positions++ = start.y(); *positions++ = start.z();
        *positions++ = start.x() + w * cell; *positions++ = start.y(); *positions++ = end.z();
    }
    for(unsigned int d = 0; d < depth; d++)
    {
        *positions++ = start.x(); *positions++ = start.y(); *positions++ = start.z() + d * cell;
        *positions++ = end.x(); *positions++ = start.y(); *positions++ = start.z() + d * cell;
    }

    auto vertexBuffer = new Qt3DRender::QBuffer(geometry);
    vertexBuffer->setData(bufferBytes);

    auto positionAttribute = new Qt3DRender::QAttribute(geometry);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexBuffer);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(2 * (width + depth));
    geometry->addAttribute(positionAttribute);

    QByteArray indexBytes;
    indexBytes.resize(static_cast<int>(2 * (width + depth) * sizeof(unsigned int)));
    unsigned int *indices = reinterpret_cast<unsigned int*>(indexBytes.data());

    for(unsigned int i = 0; i < width + depth; i++)
    { *indices++ = i * 2; *indices++ = i * 2 + 1; }

    auto indexBuffer = new Qt3DRender::QBuffer(geometry);
    indexBuffer->setData(indexBytes);

    auto indexAttribute = new Qt3DRender::QAttribute(geometry);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(2 * (width + depth));
    geometry->addAttribute(indexAttribute);

    // mesh
    auto line = new Qt3DRender::QGeometryRenderer;
    line->setGeometry(geometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    // material
    auto material = new Qt3DExtras:: QDiffuseSpecularMaterial;
    material->setAmbient(color);

    lineEntity->addComponent(line);
    lineEntity->addComponent(material);

    qDebug() << parent->objectName() << ": EntityHGrid created";
    return lineEntity;
}

Qt3DCore::QEntity *createEntityBox(const QVector3D &min,
                                   const QVector3D &max,
                                   const QColor &color,
                                   Qt3DCore::QEntity *parent)
{
    auto lineEntity = new Qt3DCore::QEntity(parent);
    QObject::connect(lineEntity, &QObject::destroyed, [=]() { qDebug() << parent->objectName() << ": EntityBox destroyed"; });

    auto geometry = new Qt3DRender::QGeometry(lineEntity);

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

    // mesh
    auto line = new Qt3DRender::QGeometryRenderer;
    line->setGeometry(geometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    // material
    auto material = new Qt3DExtras::QDiffuseSpecularMaterial;
    material->setAmbient(color);

    lineEntity->addComponent(line);
    lineEntity->addComponent(material);

    qDebug() << parent->objectName() << ": EntityBox created";
    return lineEntity;
}

Qt3DCore::QEntity* createEntityText(Qt3DCore::QEntity* parent,
                                    int size,
                                    const QString& text,
                                    const QColor &color,
                                    const QString& family)
{
    auto text2D = new Qt3DExtras::QText2DEntity(parent);
    QObject::connect(text2D, &QObject::destroyed, [=]() { qDebug() << parent->objectName() << ": EntityText destroyed"; });

    QFont font;
    font.setFamily(family);
    font.setPointSize(size);
    text2D->setFont(font);
    text2D->setHeight(font.pointSize() * 4);
    text2D->setWidth(text.length() * font.pointSize() * 2);
    text2D->setText(text);
    text2D->setColor(color);

    qDebug() << parent->objectName() << ": EntityText created";
    return text2D;
}

void applyEntityName(Qt3DCore::QEntity *entity, const QString& prefix, const QString &name)
{
    auto newname = name;
    if (newname.isEmpty()) newname = QString("%1-%2").arg(prefix, QUuid::createUuid().toString());
    entity->setObjectName(newname);
}

void applyEntityGeometry(Qt3DCore::QEntity* entity, Qt3DRender::QGeometryRenderer *gr)
{
    if(!entity || !gr) { qCritical() << __func__ << ": Wrong parameters"; return; }

    // searching old geometry and meshes
    QVector<Qt3DCore::QComponent*> vc;
    for(Qt3DCore::QComponent* c: entity->components())
    {
        if(qobject_cast<Qt3DRender::QGeometryRenderer*>(c) || qobject_cast<Qt3DRender::QMesh*>(c))
            vc.append(c);
    }

    // apply new geometry
    entity->addComponent(gr);

    // delete old geometry and meshes
    for(Qt3DCore::QComponent* c: vc)
    {
        c->setEnabled(false);
        entity->removeComponent(c);
        c->deleteLater();
    }
    qDebug() << entity->objectName() << ": Geometry applied";
}

float getGeometryDiagonal(Qt3DRender::QGeometry* geometry)
{
    if(!geometry) { qCritical() << __func__ << ": Wrong parameter"; return 0.0f; }

    return geometry->maxExtent().distanceToPoint(geometry->minExtent());
}
