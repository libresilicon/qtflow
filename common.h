#ifndef COMMON_H
#define COMMON_H

#include <QVector>

typedef struct Rectangle {
    int x;
    int y;
    int w;
    int h;
    QString m;
} Rectangle;

typedef QVector<Rectangle> rects_t;

typedef struct ParserException {
    int lineNumber;
    QString what;
} ParserException;

#endif // COMMON_H
