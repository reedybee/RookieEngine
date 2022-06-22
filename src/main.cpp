#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <thread>

#include "ren/Renderer.h"
#include "ren/Camera.h"

#include "gfx/Shader.h"
#include "gfx/Texture.h"

#include "sfx/SoundEngine.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int windowW = int(1980 * 0.80f);
int windowH = int(1080 * 0.80f);

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightPos = glm::vec3(2.0f, 3.0f, 4.0f);

float lastX = windowW / 2.0f;
float lastY = windowH / 2.0f;
bool firstMouse = true;

bool mouseHidden = true;

void ProcessInput(GLFWwindow* window, float deltatime) {
	if (glfwGetKey(window, GLFW_KEY_W))
		camera.ProcessKeyboard(FORWARD, deltatime);
	if (glfwGetKey(window, GLFW_KEY_S))
		camera.ProcessKeyboard(BACKWARD, deltatime);
	if (glfwGetKey(window, GLFW_KEY_A)) 
		camera.ProcessKeyboard(LEFT, deltatime);
	if (glfwGetKey(window, GLFW_KEY_D)) 
		camera.ProcessKeyboard(RIGHT, deltatime);
	if (glfwGetKey(window, GLFW_KEY_SPACE)) 
		camera.ProcessKeyboard(UP, deltatime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) 
		camera.ProcessKeyboard(DOWN, deltatime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	windowW = width;
	windowH = height;
}

void keyboard_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_E && action == GLFW_RELEASE)
		mouseHidden = !mouseHidden;
}

void mouse_pos_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouse(xoffset, yoffset, mouseHidden);
}

int main() {
	std::cout << "Application Started\n\n";

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	Renderer renderer = Renderer();

	renderer.CreateWindow(windowW, windowH, "RookieEngine");
	glfwSetFramebufferSizeCallback(renderer.GetWindow(), framebuffer_size_callback);
	glfwSetKeyCallback(renderer.GetWindow(), keyboard_key_callback);
	glfwSetCursorPosCallback(renderer.GetWindow(), mouse_pos_callback);

	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
		return 1;

	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);

	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SoundEngine soundEngine = SoundEngine(&camera);

	Shader shader = Shader("res/shaders/vertexShader.glsl", "res/shaders/fragmentShader.glsl");

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(renderer.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

	glm::vec3 vertices[] = {
		// position				 // normals			      // tex coords / uv's					  		 				   
		{ -0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f, 0.0f },
		{ -0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 1.0f, 0.0f },
		{  0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f, 0.0f },
		{  0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f, 0.0f },
		
		{ -0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, 
		{ -0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f },
		{  0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f, 0.0f },
		{  0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f },
							  						   
		{ -0.5f,  0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f, 0.0f },
		{ -0.5f, -0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f, 0.0f },
		{ -0.5f, -0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f, 0.0f },
		{ -0.5f,  0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f, 0.0f },
							  						   
		{  0.5f,  0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f, 0.0f },
		{  0.5f, -0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f, 0.0f },
		{  0.5f, -0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f, 0.0f },
		{  0.5f,  0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f, 0.0f },
							  						   
		{ -0.5f,  0.5f,  0.5f }, {  0.0f, 1.0f,  0.0f }, { 0.0f, 0.0f, 0.0f },
		{ -0.5f,  0.5f, -0.5f }, {  0.0f, 1.0f,  0.0f }, { 0.0f, 1.0f, 0.0f },
		{  0.5f,  0.5f, -0.5f }, {  0.0f, 1.0f,  0.0f }, { 1.0f, 1.0f, 0.0f },
		{  0.5f,  0.5f,  0.5f }, {  0.0f, 1.0f,  0.0f }, { 1.0f, 0.0f, 0.0f },
							  						   
		{ -0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f },
		{ -0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
		{  0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f },
		{  0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },
	};

	unsigned int indices[] = {
		// front
		0,1,3,
		3,1,2,
		// back
		7,5,4,
		6,5,7,
		// left
		8,9,11,
		11,9,10,
		//right
		15,13,12,
		14,13,15,
		// top
		19,17,16,
		18,17,19,
		// bottom
		20,21,23,
		23,21,22
	};

	#pragma region Buffers

	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_READ);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	std::cout << "Vertices in main" << sizeof(vertices) << "\n"; 

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

#pragma endregion

	Texture albedoMap = Texture();
	albedoMap.CreateTexture("res/gfx/bricks/Bricks_color.jpg", GL_RGB);
	shader.SetInt("albedoMap", 0);

	Texture roughnessMap = Texture();
	roughnessMap.CreateTexture("res/gfx/bricks/Bricks_roughness.jpg", GL_RGB);
	shader.SetInt("roughnessMap", 1);

	Texture normalMap = Texture();
	normalMap.CreateTexture("res/gfx/bricks/Bricks_normalGL.jpg", GL_RGB);
	shader.SetInt("normalMap", 2);

	glm::vec4 backgroundColor = { 0.2f, 0.5f, 0.8f, 1.0f };

	glm::vec3 objectPosition = { 0.0f, 0.0f, 0.0f };
	glm::quat objectRotation = { 0.0f, 0.0f, 0.0f, 0.0f };
	glm::vec3 objectScale = { 1.0f, 1.0f, 1.0f };

	float ambientStrength = 0.1f;
	float rot = 0.0f;

	while (!renderer.WindowShouldClose()) {
		renderer.PollDeltaTime();

		ProcessInput(renderer.GetWindow(), renderer.GetDeltaTime());

		if (mouseHidden)
			glfwSetInputMode(renderer.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (!mouseHidden)
			glfwSetInputMode(renderer.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGUI window creation
		ImGui::Begin("World");
		ImGui::ColorEdit4("Background Colour", (float*)&backgroundColor);
		ImGui::Text("Lighting");
		ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0, 1);
		ImGui::InputFloat3("Light Pos", (float*)&lightPos);
		ImGui::SliderFloat("Rotation", &rot, 0, 360);
		ImGui::Text("Object");
		ImGui::InputFloat3("Camera Position", (float*)&camera.position);
		ImGui::InputFloat3("Object Position", (float*)&objectPosition);
		ImGui::InputFloat3("Object Scale", (float*)&objectScale);

		// Ends the window
		ImGui::End();

		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)windowW / (float)windowH, 0.1f, 100.0f);

		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, objectPosition);
		model = glm::scale(model, objectScale);
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		shader.Use();
		shader.SetVector3("viewPos", camera.position);
		shader.SetFloat("ambientStrength", ambientStrength);
		shader.SetVector3("lightColour", lightColour);
		shader.SetVector3("lightPos", lightPos);
		shader.SetMatrix4("model", model);
		shader.SetMatrix4("view", view);
		shader.SetMatrix4("projection", projection);

		albedoMap.Bind(GL_TEXTURE0);
		roughnessMap.Bind(GL_TEXTURE1);
		normalMap.Bind(GL_TEXTURE2);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, NULL);
		
		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		renderer.SwapBuffers(45);
	}
	std::cout << "\n\nApplication Terminated\n";
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	
	return 0;
}
