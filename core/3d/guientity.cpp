#include "guientity.h"
#include "helpers3d.h"
#include "sceneentity.h"

#include <QFontMetrics>

EntityTransform::EntityTransform(Qt3DCore::QEntity *parent):
    Qt3DCore::QEntity(parent)
{
   m_Transform = new Qt3DCore::QTransform;
   addComponent(m_Transform);
   QObject::connect(this, &QObject::destroyed,
                    [=]() { qDebug() << parent->objectName() << ":" << objectName() << "destroyed"; });
}

Qt3DCore::QTransform *EntityTransform::Transform() const { return m_Transform; }

EntityText::EntityText(Qt3DCore::QEntity *parent,
                       int size,
                       const QString &text,
                       const QColor &color,
                       const QString &family,
                       int weight):
    EntityTransform(parent)
{
    setObjectName("EntityText");

    m_Text2DEntity = new Qt3DExtras::QText2DEntity;
    m_Font = QFont(family, size, weight);

    m_Text2DEntity->setFont(m_Font);
    m_Text2DEntity->setColor(color);

    setText(text);

    // обход бага, описано: https://forum.qt.io/topic/92944/qt3d-how-to-print-text-qtext2dentity/7
    m_Text2DEntity->setParent(this);
}

void EntityText::setText(const QString &text)
{
    m_Text2DEntity->setText(text);
    resize();
}

void EntityText::setTextWeight(int value)
{
    m_Font.setWeight(value);
    m_Text2DEntity->setFont(m_Font);
    resize();
}

void EntityText::resize()
{
    QFontMetrics fm(m_Font);
    auto rect = fm.boundingRect(m_Text2DEntity->text());

    m_Text2DEntity->setHeight(rect.height());
    m_Text2DEntity->setWidth(rect.width());
}

EntityBox::EntityBox(Qt3DCore::QEntity *parent,
                     float excess,
                     const QColor &color,
                     const QVector3D &min,
                     const QVector3D &max):
    EntityTransform(parent),
    m_Excess(excess)
{
    setObjectName("EntityBox");

    m_Box = createEntityBox(this, min, max, color);
}

void EntityBox::applyToEntity(SceneEntity *entity)
{
    m_Transform->setTranslation(entity->Position());
    m_Transform->setScale3D(m_Excess * QVector3D(1.0f, 1.0f, 1.0f) + entity->Size());
    setEnabled(true);
}
