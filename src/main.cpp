#include "includes.hpp"
#include "logger.hpp"
#include "entity.hpp"
#include "framebuffer.hpp"
#include "engineManager.hpp"

#pragma warning(disable : 4244)

const int resolutionX = 1600;
const int resolutionY = 1000;
const char* windowTitle = "OpenGL";
const bool fullscreen = false;

//engine::glCamera camera = engine::glCamera(90, (float)resolutionX / (float)resolutionY, 0.1f, 100.f);
base::bEntity camera = base::bEntity();
bool cursorMode = false;
bool cursorModeLastFrame = false;

//   POS VEC3               NORMALS VEC3          UV VEC2
core::glVertexArray vertices = {
	 1.0f,  1.0f, -1.0f,	0.0f, 0.0f, -1.0f,    1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,    1.0f,  0.0f,
	-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,    0.0f,  0.0f,
	-1.0f,  1.0f, -1.0f,	0.0f, 0.0f, -1.0f,    0.0f,  1.0f,

	 1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,     1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,	0.0f, 0.0f, 1.0f,     1.0f,  0.0f,
	-1.0f, -1.0f,  1.0f,    0.0f, 0.0f, 1.0f,     0.0f,  0.0f,
	-1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,     0.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,     1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,	1.0f, 0.0f, 0.0f,     1.0f,  0.0f,
	 1.0f,  1.0f,  1.0f,    1.0f, 0.0f, 0.0f,     0.0f,  0.0f,
	 1.0f,  1.0f, -1.0f,    1.0f, 0.0f, 0.0f,     0.0f,  1.0f,

	-1.0f, -1.0f,  1.0f,    -1.0f, 0.0f, 0.0f,    1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,    -1.0f, 0.0f, 0.0f,    1.0f,  0.0f,
	-1.0f,  1.0f, -1.0f,    -1.0f, 0.0f, 0.0f,    0.0f,  0.0f,
	-1.0f,  1.0f,  1.0f,    -1.0f, 0.0f, 0.0f,    0.0f,  1.0f,

	-1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,     1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,     1.0f,  0.0f,
	 1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,     0.0f,  0.0f,
	 1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,     0.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,	0.0f, -1.0f, 0.0f,    1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,	0.0f, -1.0f, 0.0f,    1.0f,  0.0f,
	-1.0f, -1.0f,  1.0f,    0.0f, -1.0f, 0.0f,    0.0f,  0.0f,
	-1.0f, -1.0f, -1.0f,    0.0f, -1.0f, 0.0f,    0.0f,  1.0f,
};

core::glIndexArray indicies = {
	0, 1, 3,
	1, 2, 3,

	4, 5, 7,
	5, 6, 7,

	8, 9, 11,
	9, 10, 11,

	12, 13, 15,
	13, 14, 15,

	16, 17, 19,
	17, 18, 19,

	20, 21, 23,
	21, 22, 23
};

core::glVertexArray quadVerts = {
	 100.0f, 0.0f,  100.0f,   0.0f, 1.0f, 0.0f,     10.0f,  10.0f,
	 100.0f, 0.0f, -100.0f,   0.0f, 1.0f, 0.0f,     10.0f, -10.0f,
	-100.0f, 0.0f, -100.0f,   0.0f, 1.0f, 0.0f,    -10.0f, -10.0f,
	-100.0f, 0.0f,  100.0f,   0.0f, 1.0f, 0.0f,    -10.0f,  10.0f,
};

core::glIndexArray quadIndicies = {
	0, 1, 3,
	1, 2, 3
};

// Updates resolution on resize.
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	auto aspect = (float)width / (float)height;
	camera.getComponent<base::bCameraComponent>()->setAspectRatio((width == 0 || height == 0) ? 1 : aspect);
	DebugConsole::log((std::string("Updating viewport resolution to [") + std::to_string(width) + "*" + std::to_string(height) + "]").c_str());
}

// Processes the window key and mouse input.
void processInput(GLFWwindow* window, float deltaTime) {
	// Exit on esc key
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	// Camera
	auto cameraTransform = camera.getComponent<base::bTransformComponent>();
	auto cameraComponent = camera.getComponent<base::bCameraComponent>();

	float flyingCameraSpeed = 5.0f * deltaTime * (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 3.0f : 1.0f);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraTransform->translate(cameraComponent->getCameraFront() * flyingCameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraTransform->translate(cameraComponent->getCameraFront() * -flyingCameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraTransform->translate(cameraComponent->getCameraRight() * flyingCameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraTransform->translate(cameraComponent->getCameraRight() * -flyingCameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraTransform->translate(cameraComponent->getCameraUp() * flyingCameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		cameraTransform->translate(cameraComponent->getCameraUp() * -flyingCameraSpeed);

	if (glfwGetKey(window, GLFW_KEY_INSERT) == GLFW_PRESS) {
		if (cursorModeLastFrame == cursorMode) {
			cursorMode = !cursorMode;
			glfwSetInputMode(window, GLFW_CURSOR, cursorMode ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		}
	} else
		cursorModeLastFrame = cursorMode;
}

const float sensitivity = 0.1f;
float lastX = resolutionX / 2.f, lastY = resolutionY / 2.f;
float pitch = 0.f, yaw = 0.f;
void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
	if (cursorMode) {
		float xOffset = xPos - lastX;
		float yOffset = lastY - yPos;
		lastX = xPos;
		lastY = yPos;

		xOffset *= sensitivity;
		yOffset *= sensitivity;

		yaw   += xOffset;
		pitch += yOffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction = glm::vec3();
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		camera.getComponent<base::bCameraComponent>()->setCameraDirection(glm::normalize(direction));
	}
}

int main(int argc, char** argv) {
	DebugConsole::log("Initializing GLFW...");

	// GLFW Init
	glfwInit();
	// OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Init window
	GLFWwindow* window = glfwCreateWindow(resolutionX, resolutionY, windowTitle, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (window == NULL) {
		DebugConsole::error("Failed to create GLFW Window.");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	DebugConsole::log("  Done!");

	// GLAD Init
	DebugConsole::log("Initializing GLAD...");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		DebugConsole::error("Failed to initialize GLAD.");
		glfwTerminate();
		return -1;
	}
	DebugConsole::log("  Done!");

	// Setting default viewport size to 1:1 ratio
	glViewport(0, 0, resolutionX, resolutionY);
	DebugConsole::log((std::string("Setting viewport resolution to [") + std::to_string(resolutionX) + "*" + std::to_string(resolutionY) + "]").c_str());
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSwapInterval(1);

	// Initializing engine
	base::bEngineManager::Init();

	// Adding scene objects
	camera.addComponent(new base::bTransformComponent());
	camera.addComponent(new base::bCameraComponent(90.f, (float)resolutionX / (float)resolutionY, 0.1f, 100.f));

	camera.getComponent<base::bTransformComponent>()->translate(glm::vec3(0.f, 0.f, 5.f));

	base::bEntity cube = base::bEntity();
	cube.addComponent(new base::bTransformComponent());
	cube.addComponent(new base::bMaterialComponent(
		{ 
			new engine::glTexture(
				".\\res\\textures\\test.png",
				core::glTextureWrappingTypes::gl_repeat,
				core::glTextureFilteringTypes::gl_bilinear
			)
		},
		".\\res\\shaders\\vertex.glsl",
		".\\res\\shaders\\fragment.glsl",
		".\\res\\shaders\\geometry.glsl"
	));
	cube.addComponent(new base::bRendererComponent(
		vertices, indicies,
		{ 
			{engine::glLayoutType::gl_float, 3}, // Pos
			{engine::glLayoutType::gl_float, 3}, // Normals
			{engine::glLayoutType::gl_float, 2}  // UV
		},
		&camera
	));

	base::bEntity groundPlane = base::bEntity();
	groundPlane.addComponent(new base::bTransformComponent());
	groundPlane.addComponent(new base::bMaterialComponent(
		{
			new engine::glTexture(
				".\\res\\textures\\gravel.jpg",
				core::glTextureWrappingTypes::gl_repeat,
				core::glTextureFilteringTypes::gl_bilinear
			)
		},
		".\\res\\shaders\\vertex.glsl",
		".\\res\\shaders\\fragment.glsl"
	));
	groundPlane.addComponent(new base::bRendererComponent(
		quadVerts, quadIndicies,
		{
			{engine::glLayoutType::gl_float, 3}, // Pos
			{engine::glLayoutType::gl_float, 3}, // Normals
			{engine::glLayoutType::gl_float, 2}  // UV
		},
		&camera
	));
	groundPlane.getComponent<base::bTransformComponent>()->setPosition(glm::vec3(0.0, -5.0, 0.0));

	double thisFrame = 0.0;
	double lastFrame = 0.0;
	double deltaTime = 0.0;

	engine::glFramebuffer buffer = engine::glFramebuffer(resolutionX, resolutionY);

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		thisFrame = glfwGetTime();
		deltaTime = thisFrame - lastFrame;

		processInput(window, deltaTime);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.65f, 0.79f, 0.94f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		cube.getComponent<base::bTransformComponent>()->rotate(deltaTime * 30, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
		
		base::bEngineManager::Instance->Render(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();

		lastFrame = thisFrame;

		glfwSetWindowTitle(window, std::string("FPS: " + std::to_string(round(1.0 / deltaTime))).c_str());
	}

	// Terminating the engine

	base::bEngineManager::Destroy();

	// Terminating glfw, etc.

	DebugConsole::log("Terminating GLFW and exiting...");
	glfwTerminate();
	return 0;
}