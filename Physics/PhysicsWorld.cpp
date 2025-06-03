#include "PhysicsWorld.h"

namespace Physics {

	void PhysicsWorld::AddParticle(P6Particle* toAdd) {
		particles.push_back(toAdd);
		forceRegistry.Add(toAdd, &gravity); // Add the particle to the force registry with gravity
	}

	void PhysicsWorld::Update(float time) {

		UpdateParticleList();

		forceRegistry.UpdateForces(time);

		for (std::list<P6Particle*>::iterator p = particles.begin();

			p != particles.end();

			++p

			)
		{
			(*p)->Update(time);
		}
	}

	void PhysicsWorld::UpdateParticleList() {
		
		particles.remove_if(
			[](P6Particle* p) {
				return p->IsDestroyed();
			}
		);
	}
}