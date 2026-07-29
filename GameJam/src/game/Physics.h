#pragma once
#include <string>

#include "GameObject.h"

typedef struct {
	glm::vec2* position;
	bool* grounded;
	glm::vec2 scale;
	f32 mass;
	glm::vec2* velocity;
	glm::vec2* impulse;
	bool kinematic;
	bool trigger;
	std::string name;
} Rigidbody;

typedef struct {
	glm::vec2 position;
	glm::vec2 scale;
	bool trigger;
	std::string name;
} StaticCollider;

typedef struct {
	std::string bodyA;
	std::string bodyB;
} TriggerCollision;

class Physics {
public:
	Physics() {}

	void AddRigidbody(Rigidbody* object) { m_Rigidbodys.push_back(object); }
	void RemoveRigidbodysByName(const std::string& name);
	void AddStaticCollider(StaticCollider collider) { m_StaticColliders.push_back(collider); }

	void Update(f32 dt);

	std::vector<TriggerCollision> GetTriggerCollisions() { return m_TriggerCollisions; }
	std::vector<StaticCollider> GetStaticColliders() { return m_StaticColliders; }
private:
	void CheckCollision(u32 i, u32 j);
	void CheckCollisionStaticCollider(u32 i, u32 j);
private:
	std::vector<Rigidbody*> m_Rigidbodys;
	std::vector<StaticCollider> m_StaticColliders;
	std::vector<TriggerCollision> m_TriggerCollisions;
};