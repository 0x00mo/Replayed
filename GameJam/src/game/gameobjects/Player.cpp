#include "Player.h"

Player::Player(Physics* physics, glm::vec2 startPosition) : m_CurrentAnim(&m_IdleRightAnim) {
	m_Position = startPosition;
	m_Scale = glm::vec2(0.05f, 0.0958f);
	m_Velocity = glm::vec2(0.0f);
	m_Impulse = glm::vec2(0.0f);
	m_Grounded = true;
	m_Rigidbody = Rigidbody{ &m_Position, &m_Grounded, m_Scale, 100.0f, &m_Velocity, &m_Impulse, true, false, "player" };
	m_Physics = physics;
	m_Physics->AddRigidbody(&m_Rigidbody);

	m_Sprite = ResourceManager::Sprites["player"];

	m_IdleRightAnim = Animation({ ResourceManager::Sprites["idle_right1"], ResourceManager::Sprites["idle_right2"] }, 2.0f, true);
	m_IdleLeftAnim = Animation({ ResourceManager::Sprites["idle_left1"], ResourceManager::Sprites["idle_left2"] }, 2.0f, true);
	m_RunRightAnim = Animation({ 
		ResourceManager::Sprites["walk_right1"], 
		ResourceManager::Sprites["walk_right2"], 
		ResourceManager::Sprites["walk_right3"], 
		ResourceManager::Sprites["walk_right4"],
		ResourceManager::Sprites["walk_right1"],
		ResourceManager::Sprites["walk_right2"],
		ResourceManager::Sprites["walk_right3"],
		ResourceManager::Sprites["walk_right4"]
	}, 1.0f, true);
	m_RunLeftAnim = Animation({
		ResourceManager::Sprites["walk_left1"],
		ResourceManager::Sprites["walk_left2"],
		ResourceManager::Sprites["walk_left3"],
		ResourceManager::Sprites["walk_left4"],
		ResourceManager::Sprites["walk_left1"],
		ResourceManager::Sprites["walk_left2"],
		ResourceManager::Sprites["walk_left3"],
		ResourceManager::Sprites["walk_left4"]
	}, 1.0f, true);
	m_AnimState = AnimationState::IdleRight;
	m_TurnedRight = true;
}

void Player::Render(Renderer* renderer) {
	glm::vec2 camPos = m_Position;
	renderer->UpdateCameraPosition(camPos);
	Sprite animFrame = m_CurrentAnim->GetFrame();
	renderer->RenderSprite(glm::vec2(m_Position.x - (animFrame.width - 12) / 24.0f * 0.05f, m_Position.y), m_Scale + glm::vec2((animFrame.width - 12) / 12.0f * 0.05f, 0.0f), animFrame);
}

void Player::Update(f32 dt) {
	if (Input::IsKeyPressed(GLFW_KEY_SPACE) && *m_Rigidbody.grounded) {
		m_Impulse += glm::vec2(0.0f, 75.0f);
	}
	if (Input::IsKeyDown(GLFW_KEY_A)) {
		m_Velocity.x = -0.5f;
		m_TurnedRight = false;
	}
	if (Input::IsKeyDown(GLFW_KEY_D)) {
		m_Velocity.x = 0.5f;
		m_TurnedRight = true;
	}

	if (std::fabs(m_Velocity.x) < 0.001f && m_TurnedRight && m_AnimState != AnimationState::IdleRight) {
		m_AnimState = AnimationState::IdleRight;
		m_CurrentAnim->Stop();
		m_CurrentAnim = &m_IdleRightAnim;
		m_CurrentAnim->Start();
	}
	else if (std::fabs(m_Velocity.x) < 0.001f && !m_TurnedRight && m_AnimState != AnimationState::IdleLeft) {
		m_AnimState = AnimationState::IdleLeft;
		m_CurrentAnim->Stop();
		m_CurrentAnim = &m_IdleLeftAnim;
		m_CurrentAnim->Start();
	}
	else if (m_Velocity.x > 0.0f && m_AnimState != AnimationState::RunRight) {
		m_AnimState = AnimationState::RunRight;
		m_CurrentAnim->Stop();
		m_CurrentAnim = &m_RunRightAnim;
		m_CurrentAnim->Start();
	}
	else if (m_Velocity.x < 0.0f && m_AnimState != AnimationState::RunLeft) {
		m_AnimState = AnimationState::RunLeft;
		m_CurrentAnim->Stop();
		m_CurrentAnim = &m_RunLeftAnim;
		m_CurrentAnim->Start();
	}

	std::vector<TriggerCollision> triggers = m_Physics->GetTriggerCollisions();
	for (TriggerCollision t : triggers) {
		if (t.bodyA == "player") {
			if (t.bodyB == "jumppad") {
				m_Velocity.y = 1.3f;
			}
		}
	}
}