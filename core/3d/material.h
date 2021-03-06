#ifndef MATERIAL_H
#define MATERIAL_H

#include <Qt3DExtras/QDiffuseSpecularMaterial>

const QString AMBIENT_COLOR = "#FFFFFF";
const float SHININESS = 150.0f;
const float SCALE = 1.0f;
const bool ALPHA_BLENDING = false;
const bool MAP_MIRRORED = false;
/*!
 * \brief MAPS_COUNT - количество текстур в материале, используется в slotTextureDone
 */
const int MAPS_COUNT = 3;

/*!
 * \brief The MapTypes - тип загружаемой текстуры в loadTexture
 */
enum MapTypes
{
    DiffuseMap = 0,
    SpecularMap,
    NormalMap
};

class Scene;

class Material : public Qt3DExtras::QDiffuseSpecularMaterial
{
    Q_OBJECT
    // Transparent - используется как флаг при загрузке в сцену и размещении в TransparentLayer
    Q_PROPERTY(bool Transparent READ Transparent FINAL)

public:
    Material(Scene *parent);
    /*!
     * \brief load - загрузка и сборка материала:
     * Сначала loadTexture загружает по одной все указанные текстуры, при завершении
     * загрузки каждой текстуры испускается сигнал signalTextureDone;
     * slotTextureDone проверяет все ли текстуры загружены и испускает сигнал signalReady.
     */
    void load(const QString& path);

    //Q_PROPERTY
    bool Transparent() const { return m_Transparent; }

protected:
    /*!
     * \brief loadTexture - загрузка единичной текстуры вматериал, только в составе load
     */
    void loadTexture(MapTypes type, const QString& path, bool mirrored = false);

private:
    int m_MapsCount;
    bool m_Transparent;

private Q_SLOTS:
    /*!
     * \brief slotTextureDone - проверка материала на готовность
     */
    void slotTextureDone();

Q_SIGNALS:
    /*!
     * \brief signalTextureDone - испускается когда текстура из состава материала загружена
     */
    void signalTextureDone();
    /*!
     * \brief signalReady - испускается когда материал полность загружен
     */
    void signalReady(Material* material);

};

#endif // MATERIAL_H
