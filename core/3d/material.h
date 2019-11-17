#ifndef MATERIAL_H
#define MATERIAL_H

#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DRender/QTexture>

const QString AMBIENT_COLOR = "#FFFFFF";
const QString SPECULAR_COLOR = "#DCDCDC";
const float SHININESS = 0.9f;
const float TEXTURE_SCALE = 1.0f;

class Material : public Qt3DExtras::QDiffuseSpecularMaterial
{
    Q_OBJECT

public:
    Material(QNode *parent = nullptr);
    void load(const QString& cfg_path);

protected:
    Qt3DRender::QTexture2D *setTexture(const QString& path);

private:

};

#endif // MATERIAL_H
