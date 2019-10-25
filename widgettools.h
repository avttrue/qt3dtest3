/*
 * дополнительный функционал управления виджетами
 */

#ifndef WIDGETTOOLS_H
#define WIDGETTOOLS_H

#include <QWidget>

/*!
 * поместить виджет по центру экрана
 */
void setWidgetToScreenCenter(QWidget* w);

/*!
 * поместить виджет по центру предка
 */
void setWidgetToParentCenter(QWidget* w);

#endif // WIDGETTOOLS_H
