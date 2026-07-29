#pragma once
#include "../../core/Types.h"
#include "../../renderer/Renderer.h"

class Scene {
public:
	virtual void Render(Renderer* renderer) = 0;
	virtual void Update(f32 dt) = 0;
};