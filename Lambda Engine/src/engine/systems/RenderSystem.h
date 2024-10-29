#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "../BaseSystem.h"
#include "../utils/Shader.h"

#include "../components/Transform.h"
#include "../components/MainCamera.h"
#include "../components/MeshRenderer.h"

class RenderSystem : BaseSystem
{
private:
	GLuint framebuffer;
	GLuint renderBuffer;
	unsigned int renderedTexture;
	std::vector<float> quadVertices;
	std::shared_ptr<Shader> screenShader;
	std::shared_ptr<Shader> projectionShader;

public:
	RenderSystem();
	virtual ~RenderSystem();
	void Update(const std::shared_ptr<EntityManager>& world, float delta) override;
	GLuint GetRenderTexture();
};