#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QObject>

class EditorWidget : public QObject
{
  Q_OBJECT
public:
  explicit EditorWidget(QObject *parent = nullptr);
  
signals:
  
public slots:
};

#endif // EDITORWIDGET_H