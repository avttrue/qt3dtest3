#include "material.h"
#include "properties.h"
#include "scene.h"

#include <QDir>
#include <QFileInfo>
#include <QSettings>

Material::Material(Scene *parent) :
    Qt3DExtras::QDiffuseSpecularMaterial(parent),
    m_MapsCount(0),
    m_Transparent(false)
{   
    QObject::connect(this, &QObject::destroyed, [=]() { qDebug() << parent->objectName() << ": Material" << objectName() << " destroyed"; });
    QObject::connect(this, &Material::signalTextureDone, this, &Material::slotTextureDone, Qt::DirectConnection);
}

void Material::loadTexture(MapTypes type, const QString &path, bool mirrored)
{
    auto fi = QFileInfo(path);
    auto texture2d = new Qt3DRender::QTexture2D(this);
    auto textureImage = new Qt3DRender::QTextureImage(texture2d);

    textureImage->setMirrored(mirrored);
    texture2d->setMinificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
    texture2d->setMagnificationFilter(Qt3DRender::QAbstractTexture::Linear);
    texture2d->setFormat(Qt3DRender::QAbstractTexture::Automatic);

    Qt3DRender::QTextureWrapMode wm;
    wm.setX(Qt3DRender::QTextureWrapMode::ClampToEdge);
    wm.setY(Qt3DRender::QTextureWrapMode::ClampToEdge);
    wm.setZ(Qt3DRender::QTextureWrapMode::ClampToEdge);
    texture2d->setWrapMode(wm);

    auto func = [=](Qt3DRender::QAbstractTexture::Status s)
    {
        if(s == Qt3DRender::QAbstractTexture::Ready)
        {
            QObject::disconnect(texture2d, &Qt3DRender::QAbstractTexture::statusChanged, nullptr, nullptr);
            qDebug() << objectName() << ": Texture loaded" << fi.fileName();
            Q_EMIT signalTextureDone();
        }
        else if(s == Qt3DRender::QAbstractTexture::Error)
        {
            QObject::disconnect(texture2d, &Qt3DRender::QAbstractTexture::statusChanged, nullptr, nullptr);
            qCritical() << objectName() << ": Error at texture loading" << fi.fileName();
            Q_EMIT signalTextureDone();
        }
        else
        { qDebug() << objectName() << ": Texture" << fi.fileName() << "loading status:" << s; }
    };

    if(fi.exists() && fi.isFile()) textureImage->setSource(QUrl::fromLocalFile(path));
    else
    {
        qCritical() << __func__ << ": Wrong texture path" << path;
        textureImage->setSource(QUrl::fromLocalFile(DEFAULT_TEXTURE));
    }
    texture2d->addTextureImage(textureImage);

    QObject::connect(texture2d, &Qt3DRender::QAbstractTexture::statusChanged, func);

    if(type == MapTypes::DiffuseMap)
        setDiffuse(QVariant::fromValue<Qt3DRender::QTexture2D*>(texture2d));

    else if(type == MapTypes::SpecularMap)
        setSpecular(QVariant::fromValue<Qt3DRender::QTexture2D*>(texture2d));

    else if(type == MapTypes::NormalMap)
        setNormal(QVariant::fromValue<Qt3DRender::QTexture2D*>(texture2d));
    else
    {
        QObject::disconnect(texture2d, &Qt3DRender::QAbstractTexture::statusChanged, nullptr, nullptr);
        qCritical() << objectName() << ": Unknown texture type" << type;
        Q_EMIT signalTextureDone();
    }
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

void Material::load(const QString &path)
{
    auto fi = QFileInfo(path);
    if(!fi.exists() || !fi.isFile()) { qCritical() << __func__ << ": Wrong path" << path; return; }

    auto assetsdir = fi.path() + QDir::separator();
    auto cfg = new QSettings(path, QSettings::IniFormat);

    setObjectName(cfg->value("Name", "material").toString());

    // AlphaBlending
    m_Transparent = cfg->value("AlphaBlending", ALPHA_BLENDING).toBool();
    setAlphaBlendingEnabled(m_Transparent);
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
