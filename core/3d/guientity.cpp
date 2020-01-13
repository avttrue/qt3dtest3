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

void EntityTransform::addComponentToDeep(Qt3DCore::QComponent *comp)
{
    for(auto n: childNodes())
    {
        auto e = qobject_cast<Qt3DCore::QEntity*>(n);
        if(e) e->addComponent(comp);
    }
}

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
                     const QColor &color):
    EntityTransform(parent),
    m_Excess(excess)
{
    setObjectName("EntityBox");

    // изначальный размер 1x1x1 с началом координат в 0-0-0
    auto min = QVector3D(-0.5f, -0.5f, -0.5f);
    auto max = QVector3D(0.5f, 0.5f, 0.5f);
    m_Box = createEntityBox(this, min, max, color);
}

void EntityBox::applyToEntity(SceneEntity *entity)
{
    m_Transform->setTranslation(entity->Position());

    auto delta = entity->Geometry()->geometry()->maxExtent() -
                 entity->Geometry()->geometry()->minExtent();

    m_Transform->setScale3D(m_Excess * QVector3D(2.0f, 2.0f, 2.0f) +
                            entity->Transform()->scale3D() * delta);

    setEnabled(true);
}

void EntityBox::setExcess(float excess) { m_Excess = excess; }
