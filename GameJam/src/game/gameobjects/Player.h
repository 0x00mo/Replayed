#pragma once
#include "../GameObject.h"
#include "../Physics.h"
#include "../Animation.h"

enum class AnimationState {
	IdleRight,
	IdleLeft,
	RunRight,
	RunLeft,
	JumpRight,
	JumpLeft,
};

class Player : public GameObject {
public:
	Player() : m_CurrentAnim(&m_IdleRightAnim) {}
	Player(Physics* physics, glm::vec2 startPosition);
	void Render(Renderer* renderer) override;
	void Update(f32 dt) override;

	glm::vec2 GetPosition() { return m_Position; }
	Sprite GetAnimationFrame() { return m_CurrentAnim->GetFrame(); }
private:
	glm::vec2 m_Position;
	bool m_Grounded;
	glm::vec2 m_Scale;
	glm::vec2 m_Velocity;
	glm::vec2 m_Impulse;
	Rigidbody m_Rigidbody;
	Sprite m_Sprite;

	Animation m_IdleRightAnim;
	Animation m_IdleLeftAnim;
	Animation m_RunRightAnim;
	Animation m_RunLeftAnim;
	Animation* m_CurrentAnim;
	AnimationState m_AnimState;
	bool m_TurnedRight;

	Physics* m_Physics;
};