#ifndef DOOMFIRE_H
#define DOOMFIRE_H

#include "effect.h"

class DoomFire : public Effect
{
public:
    DoomFire(int w, int h);
    ~DoomFire();

    void create();
    void destroy();
    void update();
    int defaultRefreshRate();
    const QVector<QRgb>& palette() const;

private:
    class Priv;
    Priv *d;
};

#endif // DOOMFIRE_H
