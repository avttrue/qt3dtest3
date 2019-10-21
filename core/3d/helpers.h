#ifndef HELPERS_H
#define HELPERS_H

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QGeometryRenderer>
#include <QVector3D>

// https://forum.qt.io/topic/66808/qt3d-draw-grid-axis-lines

/*!
 * \brief addEntityLine - нарисовать линию от точки start до end
 */
Qt3DCore::QEntity* addEntityLine(const QVector3D& start,
                                 const QVector3D& end,
                                 const QColor& color,
                                 Qt3DCore::QEntity* parent);
/*!
 * \brief addEntityBox - нарисовать каркас из линий в рамках самых удалённых точек min и max
 */
Qt3DCore::QEntity* createEntityBox(const QVector3D& min,
                                const QVector3D& max,
                                const QColor& color,
                                Qt3DCore::QEntity* parent);

Qt3DCore::QEntity* addText(Qt3DCore::QEntity* parent, const QString& text = "");

void applyEntityName(Qt3DCore::QEntity* entity, const QString &prefix, const QString& name = "");

void applyEntityGeometry(Qt3DCore::QEntity* e, Qt3DRender::QGeometryRenderer *gr);

#endif // HELPERS_H
