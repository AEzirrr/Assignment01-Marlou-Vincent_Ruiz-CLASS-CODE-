#include "PhysicsWorld.h"

namespace Physics {

	void PhysicsWorld::AddParticle(P6Particle* toAdd) {
		particles.push_back(toAdd);
	}

	void PhysicsWorld::Update(float time) {

		UpdateParticleList();

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