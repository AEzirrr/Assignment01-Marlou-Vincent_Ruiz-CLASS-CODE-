#pragma once
#include <list>

#ifndef P6PARTICLE_DEF
#define P6PARTICLE_DEF

#include "P6Particle.h"

#endif 

#include "ForceRegistry.h"
#include "GravityForceGenerator.h"

namespace Physics {
	class PhysicsWorld
	{
	public:
		ForceRegistry forceRegistry;

		std::list<P6Particle*> particles;

		void AddParticle(P6Particle* toAdd);

		void Update(float time);

	private:
		void UpdateParticleList();

		GravityForceGenerator gravity = GravityForceGenerator(MyVector(0, 0.0f, 0));
	};
}
