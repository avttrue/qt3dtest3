#ifndef MATERIAL_H
#define MATERIAL_H

#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DRender/QTexture>

const QString AMBIENT_COLOR = "#FFFFFF";
const float SHININESS = 150.0f;
const float TEXTURE_SCALE = 1.0f;
const int MAPS_COUNT = 3;

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
    void loadCFG(const QString& cfg_path);

protected:
    Qt3DRender::QTexture2D* createTexture(const QString& path, bool mirrored = false); // delete
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
