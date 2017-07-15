#ifndef COMMON_H
#define COMMON_H

#include <QMainWindow>
#include <QVector>
#include <QMap>

typedef struct Rectangle {
    int x;
    int y;
    int w;
    int h;
    QString m;
} Rectangle;

typedef QVector<Rectangle> rects_t;

typedef QMap<QString, QString> map_string_t;
typedef QList<QPair<QString, QString>> table_string_t;

struct vcd_scopes_t
{
    QString name;
    QHash<int, QString> alias;
    QList<vcd_scopes_t> scopes;
};

typedef QPair<int, QPair<int, int>> vcd_change_t;

struct vcd_t
{
    QString date;
    QString version;
    int timescale;
    vcd_scopes_t scopes;
    QList<vcd_change_t> changes;
};

typedef struct ParserException {
    int lineNumber;
    QString what;
} ParserException;

#endif // COMMON_H
