#include "RenderSystem.h"

RenderSystem::RenderSystem()
{
	screenShader = std::make_shared<Shader>("data/shaders/ScreenShader.vs", "data/shaders/ScreenShader.fs");
	projectionShader = std::make_shared<Shader>("data/shaders/ProjectionShader.vs", "data/shaders/ProjectionShader.fs");

	quadVertices = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	//Target render texture
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::Update(const std::shared_ptr<EntityManager>& world, float delta)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.18f, 0.18f, 0.18f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	projectionShader->bind();

	world->EachEntityWithComponent<Transform>([=](BaseEntity& entity, Transform& transform) {

		glm::mat4 transformation;

		glm::mat4 scaled = glm::scale(transformation, transform.Scale);
		glm::mat4 translated = glm::translate(transformation, transform.Position);
		glm::mat4 rotated = glm::rotate(transformation, transform.Axis, transform.Rotation);

		glm::mat4 model = scaled * rotated * translated;
		
		GLuint modelLoc = glGetUniformLocation(projectionShader->getProgram(), "model_matrix");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		if (entity.HasComponent<MeshRenderer>())
		{
			for (auto mesh : entity.GetComponent<MeshRenderer>()->meshes)
			{
				int counter = 0;

				if (mesh.textures.size() == 0)
				{
					glUniform4f(glGetUniformLocation(projectionShader->getProgram(), "mcolor"), 0, 0, 0, 1);
				}
				else
				{
					glUniform4f(glGetUniformLocation(projectionShader->getProgram(), "mcolor"), 0, 0, 0, 0);
				}

				for (auto texture : mesh.textures)
				{
					glActiveTexture(GL_TEXTURE0 + counter);

					if (texture->getTextureType() == TextureType::DIFFUSE_MAP)
					{
						glUniform1f(glGetUniformLocation(projectionShader->getProgram(), "material.diffuse"), counter);
					}
					else if (texture->getTextureType() == TextureType::SPECULAR_MAP)
					{
						glUniform1f(glGetUniformLocation(projectionShader->getProgram(), "material.specular"), counter);
					}

					glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
					counter++;
				}

				glActiveTexture(GL_TEXTURE0);
				glBindVertexArray(mesh.vao);
				glDrawElements(GL_TRIANGLES, (GLsizei)mesh.indices.size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
	});

	GLuint shader = projectionShader->getProgram();

	world->EachEntityWithComponent<MainCamera>([shader](BaseEntity& entity, MainCamera& camera) {

		glm::mat4 view;
		glm::mat4 model;
		glm::mat4 projection;

		model = glm::rotate(model, 0.0f, glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
		projection = glm::perspective(camera.fov, (float)camera.viewWidth / (float)camera.viewHeight, camera.near, camera.far);

		GLuint viewLoc = glGetUniformLocation(shader, "view_matrix");
		//GLuint modelLoc = glGetUniformLocation(shader, "model_matrix");
		GLuint projectionLoc = glGetUniformLocation(shader, "projection_matrix");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	});

	screenShader->bind();

	// Screen Quad VAO
	unsigned int quadVAO, quadVBO;

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), &quadVertices.front(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

GLuint RenderSystem::GetRenderTexture()
{
	return renderedTexture;
}
