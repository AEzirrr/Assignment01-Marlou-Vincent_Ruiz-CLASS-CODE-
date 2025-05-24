#include "P6Particle.h"

namespace Physics {
	void P6Particle::UpdatePosition(float time) {

		this->position = this->position + (this->velocity * time) + ((this->acceleration * time * time) * (1.0f / 2.0f));
	}

	void P6Particle::UpdateVelocity(float time) {
		this->velocity = this->velocity + (this->acceleration * time);
	}

	void P6Particle::Update(float time) {
		UpdatePosition(time);
		UpdateVelocity(time);
	}

	bool P6Particle::IsDestroyed() {
		return isDestroyed;
	}

	void P6Particle::Destroy() {
		isDestroyed = true;
	}
}