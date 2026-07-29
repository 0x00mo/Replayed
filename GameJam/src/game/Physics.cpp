#include "Physics.h"

void Physics::RemoveRigidbodysByName(const std::string& name) {
	for (u32 i = 0; i < m_Rigidbodys.size(); i++) {
		Rigidbody* body = m_Rigidbodys.at(i);
		if (body->name == name) {
			m_Rigidbodys.erase(m_Rigidbodys.begin() + i);
		}
	}
}

void Physics::Update(f32 dt) {
	m_TriggerCollisions.clear();

	dt /= 1.0f;
	for (u32 step = 0; step < 1; step++) {
		for (u32 i = 0; i < m_Rigidbodys.size(); i++) {
			Rigidbody* body = m_Rigidbodys.at(i);
			if (body->kinematic) {
				*body->grounded = false;
				*body->velocity += *body->impulse / body->mass;
				body->velocity->y += -1.0f * dt;
				*body->position += *body->velocity * dt;
				*body->impulse = glm::vec2(0.0f);
				body->velocity->x = 0.0f;
			}
		}

		for (u32 i = 0; i < m_Rigidbodys.size(); i++) {
			for (u32 j = i + 1; j < m_Rigidbodys.size(); j++) {
				CheckCollision(i, j);
			}
			for (u32 j = 0; j < m_StaticColliders.size(); j++) {
				CheckCollisionStaticCollider(i, j);
			}
		}
	}
}

void Physics::CheckCollision(u32 i, u32 j) {
	Rigidbody* bodyA = m_Rigidbodys.at(i);
	Rigidbody* bodyB = m_Rigidbodys.at(j);
	if ((bodyA->position->x < bodyB->position->x + bodyB->scale.x) && (bodyA->position->x + bodyA->scale.x > bodyB->position->x) && (bodyA->position->y < bodyB->position->y + bodyB->scale.y) && (bodyA->position->y + bodyA->scale.y > bodyB->position->y)) {
		f32 overlapX1 = (bodyA->position->x + bodyA->scale.x) - bodyB->position->x;
		f32 overlapX2 = (bodyB->position->x + bodyB->scale.x) - bodyA->position->x;
		f32 overlapY1 = (bodyA->position->y + bodyA->scale.y) - bodyB->position->y;
		f32 overlapY2 = (bodyB->position->y + bodyB->scale.y) - bodyA->position->y;
		
		f32 overlapX = (overlapX1 < overlapX2) ? overlapX1 : -overlapX2;
		f32 overlapY = (overlapY1 < overlapY2) ? overlapY1 : -overlapY2;
		
		glm::vec2 correction = glm::vec2(0.0f);

		if (fabs(overlapX) < fabs(overlapY)) {
			correction.x = overlapX;
		}
		else {
			correction.y = overlapY;
		}

		if (!bodyB->trigger) {
			if (!(bodyA->kinematic)) {
				*bodyB->position += correction;
			}
			else if (!(bodyB->kinematic)) {
				*bodyA->position -= correction;

				if (correction.y < 0) {
					bodyA->velocity->y = 0;
					*bodyA->grounded = true;
				}
				if (correction.y > 0 && bodyA->velocity->y > 0.0f) {
					bodyA->velocity->y = 0;
					*bodyA->grounded = false;
				}
			}
			else {
				*bodyA->position -= correction;
				*bodyB->position += correction;

				if (correction.y < 0) {
					bodyA->velocity->y = 0;
					*bodyA->grounded = true;
				}
				if (correction.y > 0 && bodyA->velocity->y > 0.0f) {
					bodyA->velocity->y = 0;
					*bodyA->grounded = false;
				}
			}
		}
		else {
			m_TriggerCollisions.push_back(TriggerCollision{ bodyA->name, bodyB->name });
		}
	}
}

void Physics::CheckCollisionStaticCollider(u32 i, u32 j) {
	Rigidbody* bodyA = m_Rigidbodys.at(i);
	StaticCollider bodyB = m_StaticColliders.at(j);
	if ((bodyA->position->x < bodyB.position.x + bodyB.scale.x) && (bodyA->position->x + bodyA->scale.x > bodyB.position.x) && (bodyA->position->y < bodyB.position.y + bodyB.scale.y) && (bodyA->position->y + bodyA->scale.y > bodyB.position.y)) {
		f32 overlapX1 = (bodyA->position->x + bodyA->scale.x) - bodyB.position.x;
		f32 overlapX2 = (bodyB.position.x + bodyB.scale.x) - bodyA->position->x;
		f32 overlapY1 = (bodyA->position->y + bodyA->scale.y) - bodyB.position.y;
		f32 overlapY2 = (bodyB.position.y + bodyB.scale.y) - bodyA->position->y;

		f32 overlapX = (overlapX1 < overlapX2) ? overlapX1 : -overlapX2;
		f32 overlapY = (overlapY1 < overlapY2) ? overlapY1 : -overlapY2;

		glm::vec2 correction = glm::vec2(0.0f);

		if (fabs(overlapX) < fabs(overlapY)) {
			correction.x = overlapX;
		}
		else {
			correction.y = overlapY;
		}

		if (!bodyB.trigger) {
			*bodyA->position -= correction;

			if (correction.y < 0) {
				bodyA->velocity->y = 0;
				*bodyA->grounded = true;
			}
			if (correction.y > 0 && bodyA->velocity->y > 0.0f) {
				bodyA->velocity->y = 0;
				*bodyA->grounded = false;
			}
		}
		else {
			m_TriggerCollisions.push_back(TriggerCollision{ bodyA->name, bodyB.name });
		}
	}
}
