#pragma once
#include "MyVector.h"

namespace Physics {
    class P6Particle
    {
    protected:
		bool isDestroyed = false;

        void UpdatePosition(float time);
        void UpdateVelocity(float time);

    public:
        void Update(float time);

        void Destroy();

        bool IsDestroyed();

        float mass = 0;
        Physics::MyVector position;
        Physics::MyVector velocity;
        Physics::MyVector acceleration;
    };
}
