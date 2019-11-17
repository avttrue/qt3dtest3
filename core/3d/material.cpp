#include "material.h"

#include <QDir>
#include <QFileInfo>
#include <QSettings>

Material::Material(QNode *parent) :
    Qt3DExtras::QDiffuseSpecularMaterial(parent)
{
    setAmbient(QColor(AMBIENT_COLOR));
    setShininess(SHININESS);
    setAlphaBlendingEnabled(false);
    setTextureScale(TEXTURE_SCALE);

    QObject::connect(this, &QObject::destroyed, [=]() { qDebug() << parent->objectName() << ": Material" << objectName() << " destroyed"; });
}

Qt3DRender::QTexture2D* Material::setTexture(const QString &path)
{
    auto fi = QFileInfo(path);
    auto texture2d = new Qt3DRender::QTexture2D(this);
    auto textureImage = new Qt3DRender::QTextureImage(texture2d);

    if(fi.exists() && fi.isFile()) textureImage->setSource(QUrl::fromLocalFile(path));
    else
    {
        qCritical() << __func__ << ": Wrong texture path" << path;
        textureImage->setSource(QUrl::fromLocalFile(":/models/default_texture.png"));
    }

    textureImage->setMirrored(false);
    texture2d->addTextureImage(textureImage);

    return texture2d;
}

void Material::load(const QString &cfg_path)
{
    auto fi = QFileInfo(cfg_path);

    if(!fi.exists() || !fi.isFile()) { qCritical() << __func__ << ": Wrong path" << cfg_path; return; }

    auto cfgdir = fi.path();
    auto cfg = new QSettings(cfg_path, QSettings::IniFormat);

    setObjectName(cfg->value("Name", "material").toString());

    auto ambientColor = QColor(cfg->value("AmbientColor", AMBIENT_COLOR).toString());
    if(!ambientColor.isValid()) ambientColor = QColor(AMBIENT_COLOR);
    setAmbient(ambientColor);

    setShininess(cfg->value("Shininess", SHININESS).toFloat());
    setShininess(cfg->value("TextureScale", TEXTURE_SCALE).toFloat());

    auto diffuse = cfgdir + QDir::separator() + cfg->value("DiffuseMap", "default_texture.png").toString();
    setDiffuse(QVariant::fromValue<Qt3DRender::QAbstractTexture*>(setTexture(diffuse)));

    auto specular = cfg->value("SpecularMap", "").toString();
    if(!specular.isEmpty())
        setSpecular(QVariant::fromValue<Qt3DRender::QAbstractTexture*>(setTexture(cfgdir + QDir::separator() + specular)));
    else
    {
        auto specularColor = QColor(cfg->value("SpecularColor", SPECULAR_COLOR).toString());
        if(!specularColor.isValid()) specularColor = QColor(SPECULAR_COLOR);
        setSpecular(QVariant::fromValue<QColor>(specularColor));
    }

    auto normal = cfg->value("NormalMap", "").toString();
    if(!normal.isEmpty())
        setNormal(QVariant::fromValue<Qt3DRender::QAbstractTexture*>(setTexture(cfgdir + QDir::separator() + normal)));
}
