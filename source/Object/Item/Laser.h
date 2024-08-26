#pragma once
#include "ItemBase.h"

class Laser :
    public ItemBase
{
public:

    Laser();
    ~Laser();

    virtual void Update() override;
    virtual void Draw(Vector2DFloat offset)override;
    virtual void Activate(Vector2DFloat playerpos) override;


private:

    Vector2DFloat vel_;
};



