#pragma once
#include <list>
#include "P6Particle.h"

namespace Physics {
	class PhysicsWorld
	{
	public:
		std::list<P6Particle*> particles;

		void AddParticle(P6Particle* toAdd);

		void Update(float time);

	private:
		void UpdateParticleList();
	};
}
