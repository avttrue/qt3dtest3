#include "guientity.h"

#include <QFontMetrics>

GuiEntity::GuiEntity(Qt3DCore::QEntity *parent):
    Qt3DCore::QEntity(parent)
{
   m_Transform = new Qt3DCore::QTransform;
   addComponent(m_Transform);
}

void GuiEntity::slotMatrix(const QMatrix4x4 &matrix)
{
    //qDebug() << matrix;

    m_Transform->setMatrix(matrix);
}

TextEntity::TextEntity(Qt3DCore::QEntity *parent,
                       int size,
                       const QString &text,
                       const QColor &color,
                       const QString &family, int weight):
    Qt3DCore::QEntity(parent)
{
    setObjectName("TextEntity");
    QObject::connect(this, &QObject::destroyed, [=]() { qDebug() << parent->objectName() << objectName() << "destroyed"; });

    m_Transform = new Qt3DCore::QTransform;
    addComponent(m_Transform);

    m_Text2D = new Qt3DExtras::QText2DEntity;
    m_Font = QFont(family, size, weight);

    m_Text2D->setFont(m_Font);
    m_Text2D->setColor(color);

    setText(text);

    // обход бага, описано: https://forum.qt.io/topic/92944/qt3d-how-to-print-text-qtext2dentity/7
    m_Text2D->setParent(this);
}

void TextEntity::setText(const QString &text)
{
    m_Text2D->setText(text);
    resize();
}

void TextEntity::setTextWeight(int value)
{
    m_Font.setWeight(value);
    resize();
    m_Text2D->setFont(m_Font);
}

void TextEntity::resize()
{
    QFontMetrics fm(m_Font);
    auto rect = fm.boundingRect(m_Text2D->text());

    m_Text2D->setHeight(rect.height());
    m_Text2D->setWidth(rect.width());
}
Qt3DCore::QTransform *TextEntity::Transform() const { return m_Transform; }
void TextEntity::setTransform(Qt3DCore::QTransform* transform) { m_Transform = transform; }
