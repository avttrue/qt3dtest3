#ifndef SCENE_H
#define SCENE_H

#include "sceneview.h"

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DLogic/QFrameAction>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QPickEvent>
#include <Qt3DRender/QAbstractLight>

/*!
 * \brief LOADING_STEPS - количество этапов загрузки сцены, используется в slotLoaded
 */
const int LOADING_STEPS = 2;

class SceneEntity;
class SceneObject;
class FrameRateCalculator;
class Light;
class TextEntity;
class GuiEntity;

class Scene : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    Scene(SceneView* view,
          float cell,
          float width, float height, float depth,
          const QString &name = "");
    /*!
     * \brief load - загрузить ресурсы сцены, запускает loadGeometries, loadMaterials
     */
    void load();
    Light *addLight(Qt3DRender::QAbstractLight* light,
                    const QString &name = "");
    bool delLight(const QString& name);
    bool delLight(SceneEntity *entity);
    SceneObject* addObject(const QString& geometry, const QString &material, const QString &name = "");
    bool delObject(const QString &name);
    bool delObject(SceneEntity *entity);
    QHash<QString, SceneObject* > Objects() const;
    QHash<QString, Light* > Lights() const;
    QHash<QString, Qt3DRender::QGeometryRenderer *> Geometries() const;
    QHash<QString, Qt3DRender::QMaterial* > Materials() const;
    SceneEntity *SelectedEntity() const;
    FrameRateCalculator *FRC() const;
    float CellSize() const;
    float Height() const;
    float Width() const;
    float Depth() const;
    QVector3D Size() const;
    QVector3D RealSize() const;
    void SelectEntity(SceneEntity* entity);
    QString EntityGeometry(SceneEntity* entity) const;
    QString EntityMaterial(SceneEntity* entity) const;
    void setEntityPosition(SceneEntity* entity, const QVector3D& position);
    QVector3D EntityPosition(SceneEntity* entity) const;
    void setEntitySize(SceneEntity* entity, const QVector3D& size);
    QVector3D EntitySize(SceneEntity* entity) const;
    void setEntityGeometry(SceneEntity* entity, const QString& name);
    void setEntityMaterial(SceneEntity* entity, const QString& name);
    void applyEntityRenderLayer(SceneEntity* entity);
    SceneView *View() const;
    GuiEntity *Gui() const;

protected:
    /*!
     * \brief loadGeometry - загрузить единичную геометрию, только в составе loadGeometries
     */
    void loadGeometry(const QString& path);
    /*!
     * \brief loadMaterial - загрузить единичный материал, только в составе loadMaterials
     */
    void loadMaterial(const QString& path);
    /*!
     * \brief loadGeometries - загрузить все геометрии, только в составе load
     */
    void loadGeometries(const QStringList& filters);
    /*!
     * \brief loadMaterials - загрузить все материалы, только в составе load
     */
    void loadMaterials(const QStringList& filters);
    SceneObject* addObject(Qt3DRender::QGeometryRenderer *geometry,
                           Qt3DRender::QMaterial *material,
                           const QString &name = "");

private:
    SceneView* m_View;
    Qt3DLogic::QFrameAction* m_FrameAction;
    SceneEntity* m_SelectedEntity;
    FrameRateCalculator* m_FRC;
    Qt3DCore::QEntity* m_Box;
    GuiEntity* m_Gui; //test
    TextEntity* m_InterfaceText1; //test
    QHash <QString, Light*> m_Lights;
    QHash <QString, SceneObject*> m_Objects;
    QHash <QString, Qt3DRender::QGeometryRenderer*> m_Geometries;
    QHash <QString, Qt3DRender::QMaterial*> m_Materials;
    float m_CellSize;
    float m_Height;
    float m_Width;
    float m_Depth;
    int m_LoadingSteps;

Q_SIGNALS:
    void signalSelectedEntityChanged(SceneEntity* entity);
    void signalObjectChanged(const QString& name);
    void signalLightChanged(const QString& name);
    void signalEntityClicked(Qt3DRender::QPickEvent *event, SceneEntity *entity);
    /*!
     * \brief signalGeometryLoaded - испускается при загрузке единичной геометрии
     */
    void signalGeometryLoaded(const QString& name);
    /*!
     * \brief signalMaterialLoaded - испускается при загрузке единичного материала,
     * как реакция на Material::signalReady
     */
    void signalMaterialLoaded(const QString& name);
    /*!
     * \brief signalGeometriesLoaded - испускается при загрузке всех геометрий
     */
    void signalGeometriesLoaded(int count);
    /*!
     * \brief signalMaterialsLoaded - испускается при загрузке всех материалов
     */
    void signalMaterialsLoaded(int count);
    /*!
     * \brief signalLoaded - испускается при загрузке всех ресурсов сцены
     */
    void signalLoaded();


public Q_SLOTS:
    void slotFrameActionTriggered(float dt);
    void slotShowBoxes(bool value);

private Q_SLOTS:
    /*!
     * \brief slotLoaded - обрабатывает готовность загрузки сцены;
     * реагирует на signalGeometriesLoaded, signalGeometriesLoaded;
     * испускает signalLoaded.
     */
    void slotLoaded();

};

#endif // SCENE_H
