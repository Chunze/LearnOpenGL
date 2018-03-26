#include <iostream>

#include "main.h"
#include "Renderer.h"
#include "Game.h"
#include "GameContext.h"

void InitGLFW(int MajorVersion, int MinorVersion, int Profile) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MinorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, Profile);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


int main()
{
	// initialize GLFW
	InitGLFW(3, 3, GLFW_OPENGL_CORE_PROFILE);

	// create window object
	GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// make the window object to be the main context
	glfwMakeContextCurrent(window);

	// initialize GLAD, which manages function pointers for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// specify the actual window for the rendering
	glViewport(0, 0, 640, 480);

	// add callback function for resizing the window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// create game context
	GameContext* gameContext = new GameContext(window);

	// create game instance
	Game* game = new Game(gameContext);
	game->InitGame();

	game->GameLoop();

	// clean up memory
	glfwTerminate();

	return 0;
}

