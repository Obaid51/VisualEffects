#include "recursiontree.h"

#include <math.h>
#include <QColor>

#define MAX_STEM 4

typedef enum {
    K_DEFAULT,
    K_SIERPINSKI_TRIANGLE,
    K_SQUARE,
    K_90
} TreeKind;

struct treeParams {
    bool enable[MAX_STEM];
    int maxRecursions;
    double a[MAX_STEM];
    double c[MAX_STEM];
    double size, shrink;
    double angle;
};

class RecursionTree::Priv
{
public:
    explicit Priv (int _w, int _h) : w(_w), h(_h), curMaxRecursions(0) {
        palette.push_back(QColor(232, 232, 232).rgb());
        palette.push_back(QColor(128, 96, 64).rgb());

        init(K_DEFAULT);
    }

    void init(TreeKind tk) {
        switch (tk) {
        case K_DEFAULT:
            params.enable[0] = params.enable[1] = params.enable[2] = true;
            params.enable[3] = false;
            params.a[0] = 1; params.a[1] = -1; params.a[2] = 0;
            params.c[0] = params.c[1] = 0; params.c[2] = 0.2;
            params.size = h/3;
            params.shrink = 1.5;
            params.maxRecursions = 6;
            break;

        case K_SIERPINSKI_TRIANGLE:
            params.enable[0] = params.enable[1] = params.enable[2] = true;
            params.enable[3] = false;
            params.a[0] = 0; params.a[1] = 0; params.a[2] = 0;
            params.c[0] = 0; params.c[1] = 2*M_PI/3; params.c[2] = -2*M_PI/3;
            params.size = h/2;
            params.shrink = 2;
            params.maxRecursions = 8;
            break;

        case K_SQUARE:
            params.enable[0] = params.enable[1] = params.enable[2] = params.enable[3] = true;
            params.a[0] = 0; params.a[1] = 0; params.a[2] = 0; params.a[3] = 0;
            params.c[0] = 0; params.c[1] = M_PI/2; params.c[2] = M_PI; params.c[3] = -M_PI/2;
            params.size = h/2;
            params.shrink = 2;
            params.maxRecursions = 8;
            break;

        case K_90:
            params.enable[0] = params.enable[1] = params.enable[2] = true;
            params.enable[3] = false;
            params.a[0] = 0; params.a[1] = 0; params.a[2] = 0;
            params.c[0] = 0; params.c[1] = M_PI/2; params.c[2] = -M_PI/2;
            params.size = h/2;
            params.shrink = 2;
            params.maxRecursions = 8;
            break;
        }

        params.angle = 0.25 * M_PI;
    }

    void recursion(QPainter *p, double posX, double posY, double dirX, double dirY, double size, int n)
    {
        double posX2, posY2;
        posX2 = posX + size * dirX;
        posY2 = posY + size * dirY;

        p->drawLine(int(posX), int(posY), int(posX2), int(posY2));

        if (n >= curMaxRecursions)
            return;

        double dirX2, dirY2, size2;
        size2 = size / params.shrink;

        for (int i=0; i<MAX_STEM; i++) {
            if (params.enable[i]) {
                dirX2 = cos(params.a[i] * params.angle + params.c[i]) * dirX + sin(params.a[i] * params.angle + params.c[i]) * dirY;
                dirY2 = -sin(params.a[i] * params.angle + params.c[i]) * dirX + cos(params.a[i] * params.angle + params.c[i]) * dirY;
                recursion(p, posX2, posY2, dirX2, dirY2, size2, n+1);
            }
        }
    }

    QVector<QRgb> palette;
    struct treeParams params;
    int w, h;
    int curMaxRecursions;
};

RecursionTree::RecursionTree(int w, int h) : Effect (w, h)
{
    d = new Priv(w, h);
}

RecursionTree::~RecursionTree()
{
    delete d;
}

void RecursionTree::create()
{
}

void RecursionTree::destroy()
{
}

void RecursionTree::update()
{
    d->curMaxRecursions = (d->curMaxRecursions + 1) % d->params.maxRecursions;
}

const QVector<QRgb>& RecursionTree::palette() const
{
    return d->palette;
}

bool RecursionTree::paint(QPainter *painter) const
{
    if (!painter)
        return false;

    painter->setClipping(false);
    painter->fillRect(0, 0, w, h, d->palette[0]);

    painter->setPen(d->palette[1]);
    if (d->curMaxRecursions > 0)
        d->recursion(painter, w/2, h-1, 0, -1, d->params.size, 1);

    return true;
}