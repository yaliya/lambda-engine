#include "MovementSystem.h"

MovementSystem::MovementSystem()
{
}

MovementSystem::~MovementSystem()
{
}

void MovementSystem::Update(const std::shared_ptr<EntityManager>& world, float delta)
{
	world->EachEntityWithComponent<MainCamera>([](BaseEntity& entity, MainCamera& camera) {

		if (ImGui::IsMouseDragging(1))
		{
			ImVec2 mousePosition = ImGui::GetMousePos();

			camera.TrackMouse(mousePosition.x, mousePosition.y);
		}

		if (ImGui::IsMouseReleased(1))
		{
			camera.ResetMouse();
		}

		if (ImGui::IsKeyDown('W'))
		{
			camera.MoveForward();
		}

		if (ImGui::IsKeyDown('A'))
		{
			camera.MoveLeft();
		}

		if (ImGui::IsKeyDown('S'))
		{
			camera.MoveBackwards();
		}

		if (ImGui::IsKeyDown('D'))
		{
			camera.MoveRight();
		}
	});
}