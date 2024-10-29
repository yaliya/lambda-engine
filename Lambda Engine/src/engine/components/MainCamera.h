#pragma once

#include "../BaseComponent.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

struct MainCamera : BaseComponent
{
	float fov;
	float far;
	float near;
	float viewWidth;
	float viewHeight;

	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 position;

	float yaw = -90.0f;
	float pitch = 0.0f;
	float speed = 0.3f;
	float sensivity = 0.25f;

	float lastX = 0;
	float lastY = 0;
	bool mouseStart = true;

	MainCamera(float viewWidth, float viewHeight, float fov, float near, float far);
	MainCamera(glm::vec3& position, glm::vec3& front, glm::vec3& right, glm::vec3& up, float viewWidth, float viewHeight, float fov, float near, float far);
	
	void MoveLeft();
	void MoveRight();
	void ResetMouse();
	void MoveForward();
	void MoveBackwards();
	void TrackMouse(float mx, float my);

	void FocusAt(glm::vec3 position);
	glm::vec3 UnprojectMouse(float mx, float my);

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive( 
				CEREAL_NVP(up),
				CEREAL_NVP(far),
				CEREAL_NVP(fov),
				CEREAL_NVP(near),
				CEREAL_NVP(front),
				CEREAL_NVP(right),
				CEREAL_NVP(position),
				CEREAL_NVP(viewWidth), 
				CEREAL_NVP(viewHeight));
	}

	template<class Archive>
	static void load_and_construct(Archive & ar, cereal::construct<MainCamera> & construct)
	{
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 right;
		glm::vec3 up;

		float viewHeight;
		float viewWidth;
		float near;
		float fov;
		float far;

		ar(up, far, fov, near, front, right, position, viewWidth, viewHeight);
		construct(position, front, right, up, viewWidth, viewHeight, fov, near, far);
	}
};

CEREAL_REGISTER_TYPE(MainCamera);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BaseComponent, MainCamera);