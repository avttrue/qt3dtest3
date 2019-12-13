#include "material.h"
#include "properties.h"
#include "scene.h"

#include <QDir>
#include <QFileInfo>
#include <QSettings>

Material::Material(Scene *parent) :
    Qt3DExtras::QDiffuseSpecularMaterial(parent)
{
    QObject::connect(this, &QObject::destroyed, [=]() { qDebug() << parent->objectName() << ": Material" << objectName() << " destroyed"; });
    QObject::connect(this, &Material::signalTextureDone, this, &Material::slotTextureDone, Qt::DirectConnection);
}

Qt3DRender::QTexture2D* Material::createTexture(const QString &path, bool mirrored)
{
    auto fi = QFileInfo(path);
    auto texture2d = new Qt3DRender::QTexture2D(this);
    auto textureImage = new Qt3DRender::QTextureImage(texture2d);

    if(fi.exists() && fi.isFile()) textureImage->setSource(QUrl::fromLocalFile(path));
    else
    {
        qCritical() << __func__ << ": Wrong texture path" << path;
        textureImage->setSource(QUrl::fromLocalFile(DEFAULT_TEXTURE));
    }

    textureImage->setMirrored(mirrored);
    texture2d->addTextureImage(textureImage);

    return texture2d;
}

void Material::loadTexture(MapTypes type, const QString &path, bool mirrored)
{
    auto fi = QFileInfo(path);
    auto tl = new Qt3DRender::QTextureLoader(this);

    if(fi.exists() && fi.isFile()) tl->setSource(QUrl::fromLocalFile(path));
    else
    {
        qCritical() << __func__ << ": Wrong texture path" << path;
        tl->setSource(QUrl::fromLocalFile(DEFAULT_TEXTURE));
    }

    tl->setMirrored(mirrored);
    tl->setMinificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
    tl->setMagnificationFilter(Qt3DRender::QAbstractTexture::Linear);
    tl->setFormat(Qt3DRender::QAbstractTexture::Automatic);

    Qt3DRender::QTextureWrapMode wm;
    wm.setX(Qt3DRender::QTextureWrapMode::ClampToEdge);
    wm.setY(Qt3DRender::QTextureWrapMode::ClampToEdge);
    wm.setZ(Qt3DRender::QTextureWrapMode::ClampToEdge);
    tl->setWrapMode(wm);

    if(type == MapTypes::DiffuseMap)
        setDiffuse(QVariant::fromValue<Qt3DRender::QTextureLoader*>(tl));

    else if(type == MapTypes::SpecularMap)
        setSpecular(QVariant::fromValue<Qt3DRender::QTextureLoader*>(tl));

    else if(type == MapTypes::NormalMap)
        setNormal(QVariant::fromValue<Qt3DRender::QTextureLoader*>(tl));

    qDebug() << objectName() << ": Texture loaded" << path;
    Q_EMIT signalTextureDone();
}

void Material::slotTextureDone()
{
    m_MapsCount++;
    if(m_MapsCount >= MAPS_COUNT)
    {
        QObject::disconnect(this, &Material::signalTextureDone, nullptr, nullptr);
        Q_EMIT signalReady();
    }
}

void Material::loadCFG(const QString &cfg_path)
{
    auto fi = QFileInfo(cfg_path);

    if(!fi.exists() || !fi.isFile()) { qCritical() << __func__ << ": Wrong path" << cfg_path; return; }

    auto assetsdir = fi.path() + QDir::separator();
    auto cfg = new QSettings(cfg_path, QSettings::IniFormat);

    setObjectName(cfg->value("Name", "material").toString());
    setAlphaBlendingEnabled(cfg->value("AlphaBlending", ALPHA_BLENDING).toBool());
    setShininess(cfg->value("Shininess", SHININESS).toFloat());
    setTextureScale(cfg->value("Scale", SCALE).toFloat());

    auto map_mirrored = cfg->value("MapMirrored", MAP_MIRRORED).toBool();
    auto ambientColor = QColor(cfg->value("AmbientColor", QColor::Invalid).toString());
    auto specularColor = QColor(cfg->value("SpecularColor", QColor::Invalid).toString());
    auto diffuse = cfg->value("DiffuseMap", "").toString();
    auto specular = cfg->value("SpecularMap", "").toString();
    auto normal = cfg->value("NormalMap", "").toString();

    // AmbientColor
    if(!ambientColor.isValid()) ambientColor = QColor(AMBIENT_COLOR);
    setAmbient(ambientColor);

    // DiffuseMap
    loadTexture(MapTypes::DiffuseMap, assetsdir + diffuse, map_mirrored);

    // SpecularMap
    if(!specular.isEmpty()) loadTexture(MapTypes::SpecularMap, assetsdir + specular, map_mirrored);
    else
    {
        if(!specularColor.isValid()) specularColor = ambientColor.darker();
        setSpecular(QVariant::fromValue<QColor>(specularColor));
        Q_EMIT signalTextureDone();
    }

    // NormalMap
    if(!normal.isEmpty()) loadTexture(MapTypes::NormalMap, assetsdir + normal, map_mirrored);
    else Q_EMIT signalTextureDone();
}
