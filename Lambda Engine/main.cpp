
//https://github.com/Flix01/imgui/tree/2015-10-Addons/addons

#define WEBVIEW_WINAPI
#define WEBVIEW_IMPLEMENTATION

#include <nfd.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "src\gui\imgui_glfw.h"
#include "src\gui\imguidock.h"
#include "src\gui\imgui_styles.h"
#include "src\engine\systems\RenderSystem.h"
#include "src\engine\managers\AssetManager.h"
#include "src\engine\systems\MovementSystem.h"
#include "src\engine\managers\EntityManager.h"

int W_WIDTH = 1280;
int W_HEIGHT = 720;
GLFWwindow* window = nullptr;
nfdchar_t* saveFile = nullptr;

std::shared_ptr<EntityManager> world;
std::shared_ptr<RenderSystem> renderSystem;
std::shared_ptr<MovementSystem> movementSystem;

int main(int argc, char *argv[])
{
	ASSERT(glfwInit(), "Failed to initialize GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Lambda Engine", NULL, NULL);

	ASSERT(window, "Failed to create GLFW Window");

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	ASSERT((glewInit() == GLEW_OK), "Failed to initialize GLEW");

	glfwGetFramebufferSize(window, &W_WIDTH, &W_HEIGHT);
	glViewport(0, 0, W_WIDTH, W_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//TODO Depth clamping
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	//glEnable(GL_FRAMEBUFFER_SRGB);

	std::cout << "Engine Initialized" << std::endl;
	std::cout << "Running OpenGL Version " << glGetString(GL_VERSION) << std::endl;


	ImGui::CreateContext();
	ImGui::SetCurrentDockContext(ImGui::CreateDockContext());
	ASSERT(ImGui_ImplGlfwGL3_Init(window, true), "Failed to initialize ImGui");

	ImGui::SetGUIStyle(ImGui::ImGuiThemeStyle::MOONLIGHT);
	ImGui::SetGUIFont("data/fonts/Roboto-Medium.ttf", 18.0f);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::GetIO().ConfigDockingWithShift = true;

	world = std::make_shared<EntityManager>();
	renderSystem = std::make_shared<RenderSystem>();
	movementSystem = std::make_shared<MovementSystem>();

	world->CreateEntity()->SetName("MainCamera")->AddComponent<MainCamera>((float)W_WIDTH, (float)W_HEIGHT, 45.0f, 0.1f, 100.f);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderSystem->Update(world, glfwGetTime());

		movementSystem->Update(world, glfwGetTime());

		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGui::SetNextWindowSize(ImVec2((float)W_WIDTH, 25.0f), 1);

		if (ImGui::Begin("MenuBar", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open"))
					{
						if (NFD_OpenDialog(NULL, NULL, &saveFile) == NFD_OKAY)
						{
							std::ifstream file(saveFile);
							cereal::JSONInputArchive archive(file);
							world->serialize(archive);
						}
					}

					if (saveFile)
					{
						if (ImGui::MenuItem("Save"))
						{
							std::ifstream file(saveFile);
							cereal::JSONInputArchive archive(file);
							world->serialize(archive);
						}
					}

					if (ImGui::MenuItem("Save As..."))
					{
						if (NFD_SaveDialog(NULL, NULL, &saveFile) == NFD_OKAY)
						{
							std::ofstream file(saveFile);
							cereal::JSONOutputArchive archive(file);
							world->serialize(archive);
						}
					}

					if (ImGui::MenuItem("Exit"))
					{
						glfwSetWindowShouldClose(window, true);
					}

					ImGui::EndMenu();
				}


				if (ImGui::BeginMenu("GameObject"))
				{
					if (ImGui::MenuItem("Create Empty"))
					{
						
						world->CreateEntity()->AddComponent<Transform>();
					}

					if (ImGui::MenuItem("Import"))
					{

						nfdchar_t* importFile = nullptr;

						if (NFD_OpenDialog(NULL, NULL, &importFile) == NFD_OKAY)
						{
							auto entity = world->CreateEntity();

							entity->AddComponent<Transform>();

							entity->AddComponent<MeshRenderer>(AssetManager::GetAsset<Model>(importFile));
						}
					}

					ImGui::MenuItem("Entities");
					ImGui::MenuItem("Primitives");
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Help"))
				{
					ImGui::MenuItem("About");
					ImGui::MenuItem("Tutorials");
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
		}

		ImGui::End();


		ImGui::SetNextWindowPos(ImVec2(0, 25.0f));

		ImGui::SetNextWindowSize(ImVec2((float)W_WIDTH, (float)W_HEIGHT - 25.0f), 1);

		if (ImGui::Begin("DockSpace", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{

			ImGui::BeginDockspace();

			if (ImGui::Begin("Hierarchy")) {

				auto lastSelectedEntity = world->GetLastSelectedEntity();

				for (auto it : world->GetEntities()) {

					ImGuiTreeNodeFlags isSelectedFlag = lastSelectedEntity && lastSelectedEntity->name == it->name ? ImGuiTreeNodeFlags_Selected : 0;

					bool created = ImGui::TreeNodeEx(it->name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Bullet | isSelectedFlag);
					
					ImGui::PushID(it->name.c_str());

					if (ImGui::BeginPopupContextItem("Context Menu"))
					{
						if (ImGui::BeginMenu("Add Component")) {
							ImGui::MenuItem("Transform", false, false, !it->HasComponent<Transform>());
							ImGui::MenuItem("Rigidbody");
							if (ImGui::BeginMenu("Mesh Renderer"))
							{
								if(ImGui::MenuItem("Import Model"))
								{
									nfdchar_t* importFile = nullptr;

									if (NFD_OpenDialog(NULL, NULL, &importFile) == NFD_OKAY)
									{
										it->AddComponent<MeshRenderer>(AssetManager::GetAsset<Model>(importFile));

										world->SetLastSelectedEntity(it);
									}
								}
								ImGui::EndMenu();
							}
							ImGui::EndMenu();
						}

						ImGui::Selectable("Rename");
						ImGui::Selectable("Focus");
						ImGui::Selectable("Delete");
						ImGui::EndPopup();
					}

					if (ImGui::IsItemClicked()) {

						world->SetLastSelectedEntity(it);
					}

					if (created) {

						ImGui::TreePop();
					}

					ImGui::PopID();
				}
			}
			ImGui::End();

			if (ImGui::Begin("Render")) {

				ImVec2 pos = ImGui::GetCursorScreenPos();
				unsigned renderTexture = renderSystem->GetRenderTexture();
				ImGui::GetWindowDrawList()->AddImage((void *)renderTexture, ImVec2(0, 0), ImVec2(W_WIDTH, W_HEIGHT), ImVec2(0, 1), ImVec2(1, 0));
			}

			ImGui::End();

			if (ImGui::Begin("Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (world->GetLastSelectedEntity() != nullptr)
				{
					auto entity = world->GetLastSelectedEntity();

					entity->EachComponent([](BaseComponent& component, BaseEntity& entity) {

						if (ImGui::TreeNodeEx(component.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)) {

							if (component.name == "Transform")
							{
								auto transform = entity.GetComponent<Transform>();

								ImGui::Text("Position");
								ImGui::PushItemWidth(100);
								ImGui::PushID("transformPosition");
								ImGui::InputFloat("x", &transform->Position.x, 0, 0, 1);
								
								ImGui::SameLine();
								ImGui::PushItemWidth(100);
								ImGui::InputFloat("y", &transform->Position.y, 0, 0, 1);

								ImGui::SameLine();
								ImGui::PushItemWidth(100);
								ImGui::InputFloat("z", &transform->Position.z, 0, 0, 1);
								ImGui::PopID();
								
								ImGui::Text("Rotation");
								ImGui::PushItemWidth(100);
								ImGui::PushID("transformRotation");
								ImGui::InputFloat("x", &transform->Rotation.x, 0, 0, 1);

								ImGui::SameLine();
								ImGui::PushItemWidth(100);
								ImGui::InputFloat("y", &transform->Rotation.y, 0, 0, 1);

								ImGui::SameLine();
								ImGui::PushItemWidth(100);
								ImGui::InputFloat("z", &transform->Rotation.z, 0, 0, 1);
								ImGui::PopID();

								ImGui::Text("Scale");
								ImGui::PushItemWidth(100);
								ImGui::PushID("transformScale");
								ImGui::InputFloat("x", &transform->Scale.x, 0, 0, 1);

								ImGui::SameLine();
								ImGui::PushItemWidth(100);
								ImGui::InputFloat("y", &transform->Scale.y, 0, 0, 1);

								ImGui::SameLine();
								ImGui::PushItemWidth(100);
								ImGui::InputFloat("z", &transform->Scale.z, 0, 0, 1);
								ImGui::PopID();
							}

							ImGui::TreePop();
						}
					});
				}
			}

			ImGui::End();

			ImGui::EndDockspace();
		}

		ImGui::End();

		ImGui::Render();

		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}

