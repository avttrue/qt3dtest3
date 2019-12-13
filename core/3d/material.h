#ifndef MATERIAL_H
#define MATERIAL_H

#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DRender/QTexture>

const QString AMBIENT_COLOR = "#FFFFFF";
const float SHININESS = 150.0f;
const float SCALE = 1.0f;
const int MAPS_COUNT = 3;
const bool ALPHA_BLENDING = false;
const bool MAP_MIRRORED = false;

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

public:
    Material(Scene *parent);
    void load(const QString& path);

protected:
    void loadTexture(MapTypes type, const QString& path, bool mirrored = false);

private:
    int m_MapsCount = 0;

private Q_SLOTS:
    void slotTextureDone();

Q_SIGNALS:
    void signalTextureDone();
    void signalReady();

};

#endif // MATERIAL_H
