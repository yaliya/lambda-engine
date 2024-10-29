#include "MainCamera.h"

MainCamera::MainCamera(float viewWidth, float viewHeight, float fov, float near, float far)
{
	name = "MainCamera";
	this->viewHeight = viewHeight;
	this->viewWidth = viewWidth;
	this->near = near;
	this->fov = fov;
	this->far = far;

	position = glm::vec3(0, 0, 3.0f);
	front = glm::vec3(0, 0, -1);
	right = glm::vec3(0, 0, 0);
	up = glm::vec3(0, 1, 0);

	glm::vec3 camFront;

	camFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	camFront.y = sin(glm::radians(pitch));
	camFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(camFront);
}

MainCamera::MainCamera(glm::vec3 & position, glm::vec3 & front, glm::vec3 & right, glm::vec3 & up, float viewWidth, float viewHeight, float fov, float near, float far)
{
	name = "MainCamera";
	this->viewHeight = viewHeight;
	this->viewWidth = viewWidth;
	this->near = near;
	this->fov = fov;
	this->far = far;

	this->position = position;
	this->front = front;
	this->right = right;
	this->up = up;

	glm::vec3 camFront;

	camFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	camFront.y = sin(glm::radians(pitch));
	camFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(camFront);
}

void MainCamera::TrackMouse(float mx, float my)
{
	if (mouseStart)
	{
		lastX = mx;
		lastY = my;
		mouseStart = false;
	}

	float xoffset = mx - lastX;
	float yoffset = lastY - my;

	lastX = mx;
	lastY = my;

	xoffset *= sensivity;
	yoffset *= sensivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 camFront;

	camFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	camFront.y = sin(glm::radians(pitch));
	camFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(camFront);
}

void MainCamera::MoveForward()
{
	position += speed * front;
}

void MainCamera::MoveBackwards()
{
	position -= speed * front;
}

void MainCamera::MoveLeft()
{
	position -= glm::cross(front, up) * speed;
}

void MainCamera::MoveRight()
{
	position += glm::cross(front, up) * speed;
}

void MainCamera::ResetMouse()
{
	mouseStart = true;
}

void MainCamera::FocusAt(glm::vec3 position)
{
	position = position - front * glm::vec3(5, 5, 5);
}

glm::vec3 MainCamera::UnprojectMouse(float mx, float my)
{
	glm::vec4 vec;

	vec.x = (2.0f * mx) / viewWidth - 1.0f;
	vec.y = -(2.0f * my / viewHeight - 1);
	vec.z = 1.0f;
	vec.w = 1.0f;

	glm::mat4 proj;
	glm::perspectiveFov(glm::radians(fov), viewWidth, viewHeight, near, far);
	glm::mat4 view = glm::lookAt(glm::vec3(), front, up);

	glm::mat4 viewInv = glm::inverse(view);
	glm::mat4 projInv = glm::inverse(proj);

	/*
	Vector4.Transform(ref vec, ref projInv, out vec);
	Vector4.Transform(ref vec, ref viewInv, out vec);

	if (vec.W > 0.000001f || vec.W < -0.000001f)
	{
		vec.X /= vec.W;
		vec.Y /= vec.W;
		vec.Z /= vec.W;
	}
	*/
	return glm::vec3();
}
