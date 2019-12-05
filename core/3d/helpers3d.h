#ifndef HELPERS_H
#define HELPERS_H

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QMaterial>

#include <QAbstractLight>
#include <QColor>
#include <QVector3D>

/*!
 * \brief addEntityLine - нарисовать линию от точки start до end.
 * https://forum.qt.io/topic/66808/qt3d-draw-grid-axis-lines
 */
Qt3DCore::QEntity* createEntityLine(const QVector3D& start,
                                    const QVector3D& end,
                                    const QColor& color,
                                    Qt3DCore::QEntity* parent);

/*!
 * \brief createEntityBottomGrid - нарисовать горизонтальную сетку из линий в рамках
 * точек start и end с квадратным шагом cell на уровне start.y
 */
Qt3DCore::QEntity* createEntityBottomGrid(const QVector3D& start,
                                          const QVector3D& end,
                                          float cell,
                                          const QColor& color,
                                          Qt3DCore::QEntity* parent);

/*!
 * \brief addEntityBox - нарисовать каркас из линий в рамках самых удалённых точек min и max
 */
Qt3DCore::QEntity* createEntityBox(const QVector3D& min,
                                   const QVector3D& max,
                                   const QColor& color,
                                   Qt3DCore::QEntity* parent);

Qt3DCore::QEntity* createEntityText(Qt3DCore::QEntity* parent,
                                    int size,
                                    const QString& text,
                                    const QColor &color =Qt::white,
                                    const QString &family = "monospace");

void applyEntityName(Qt3DCore::QEntity* entity, const QString &prefix, const QString& name = "");

void applyEntityGeometry(Qt3DCore::QEntity* entity, Qt3DRender::QGeometryRenderer *geometryrenderer);

void applyEntityMaterial(Qt3DCore::QEntity* entity, Qt3DRender::QMaterial* material);

void applyEntityLight(Qt3DCore::QEntity* entity, Qt3DRender::QAbstractLight* light);

float getGeometryDiagonal(Qt3DRender::QGeometry* geometry);

#endif // HELPERS_H
