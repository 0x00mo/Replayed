#pragma once
#include "../core/Types.h"
#include "../core/Input.h"
#include "../renderer/Renderer.h"

class GameObject {
public:
	virtual void Render(Renderer* renderer) = 0;
	virtual void Update(f32 dt) = 0;
};