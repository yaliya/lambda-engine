#pragma once

#include "../BaseSystem.h"
#include "../components/MainCamera.h"

#include "../../gui/imgui.h"
#include <GLFW\glfw3.h>
#include <iostream>

class MovementSystem : BaseSystem
{
private:
	float mouseX;
	float mouseY;
public:
	MovementSystem();
	virtual ~MovementSystem();
	void Update(const std::shared_ptr<EntityManager>& world, float delta) override;
};