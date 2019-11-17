#ifndef MATERIAL_H
#define MATERIAL_H

#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DRender/QTexture>

const QString AMBIENT_COLOR = "#FFFFFF";
const float SHININESS = 150.0f;
const float TEXTURE_SCALE = 1.0f;

class Scene;

class Material : public Qt3DExtras::QDiffuseSpecularMaterial
{
    Q_OBJECT

public:
    Material(Scene *parent);
    void load(const QString& cfg_path);

protected:
    Qt3DRender::QTexture2D *setTexture(const QString& path);

private:

};

#endif // MATERIAL_H
