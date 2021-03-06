#include <iostream>
#include <string>
#include "Game.h"
#include "Jello.h"
#include "pic.h"

// components and managers for the game
#include "Renderer.h"
#include "PhysicsManager.h"
#include "JelloWorld.h"
#include "ParticleWorld.h"
#include "SimpleWorld.h"
#include "InputHandler.h"
#include "HW3/IKWorld.h"

#define SCREEN_SHOTS 0
GameContext* gameContext;
double mouselastX = 400, mouselastY = 300;
bool firstMouse = true;
int* WindowWidth_callback;
int* WindowHeight_callback;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) // this bool variable is initially set to true
	{
		mouselastX = xpos;
		mouselastY = ypos;
		firstMouse = false;
	}

	float offsetX = (float)(xpos - mouselastX);
	float offsetY = (float)(mouselastY - ypos);
	mouselastX = xpos;
	mouselastY = ypos;

	float sensitivity = 0.05f;
	offsetX *= sensitivity;
	offsetY *= sensitivity;

	gameContext->m_renderer->mainCamera->Rotate(offsetY, offsetX);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glfwGetWindowSize(gameContext->GetWindow(), WindowWidth_callback, WindowHeight_callback);
}

Game::Game(GameContext* _gameContext)
	: BaseClass(_gameContext)
{
	
}

void Game::InitGame()
{
	std::cout << "Initializing game...\n";

	// add callback function for resizing the window
	WindowWidth_callback = &WindowWidth;
	WindowHeight_callback = &WindowHeight;
	glfwSetFramebufferSizeCallback(m_gameContext->GetWindow(), framebuffer_size_callback);
	gameContext = m_gameContext;

	{
		m_gameContext->m_Game = this;
		glfwGetWindowSize(m_gameContext->GetWindow(), &WindowWidth, &WindowHeight);
	}

	// creating components and managers for the game
	{
		DrawList* drawList = new DrawList(m_gameContext);
		m_gameContext->m_drawList = drawList;
	}
	{
		std::cout << "Initializing renderer...\n";
		Renderer* renderer = new Renderer(m_gameContext);
		m_gameContext->m_renderer = renderer;
	}
	{
		std::cout << "Initializing physics manager...\n";
		PhysicsManager* physicsManager = new PhysicsManager(m_gameContext);
		m_gameContext->m_physicsManager = physicsManager;
	}
	{
		
			m_gameContext->m_renderer->SetBackgroundColor(0.0f, 0.0f, 0.0f);
			World* world = new SimpleWorld(m_gameContext);
			bPaused = true;
			m_gameContext->m_world = world;
	}

	// Construct physics scene
	m_gameContext->GetPhysicsManager()->ConstructPhysicsScene(m_gameContext->GetWorld()->GetPhysicsProxies());
}

void Game::GameLoop()
{
	while (!glfwWindowShouldClose(m_gameContext->GetWindow()))
	{
		bWasPausedLastFrame = bPaused;
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastFrameTime;

		if (true)
		{
			std::string fps;
			char buf[10];

			TimeSinceFPSRender += deltaTime;
			if (TimeSinceFPSRender >= FPS_RENDER_TIME_STEP)
			{
				FPS = 1 / deltaTime;
				TimeSinceFPSRender -= FPS_RENDER_TIME_STEP;
			}

			sprintf(buf, "%.2f", FPS);
			fps = std::string(buf);

			m_gameContext->GetDrawList()->AddOnScreenText("FPS: " + fps, 25.0f, 25.0f, 1.0f, glm::vec3(0.9, 0.9, 0.9));
		}

		// First frame
		if (lastFrameTime == 0.0f)
		{
			deltaTime = 0.016f;
		}

		lastFrameTime = currentTime;

		// input
		processInput(m_gameContext->GetWindow());

		float TimeSinceLastScreenSave = currentTime - lastScreenCapture;

		float ScreenCaptureRate = 1.0f / 15.0f;

		if (SCREEN_SHOTS && deltaTime != 0.0f && ScreenCaptureIndex < 250 && TimeSinceLastScreenSave > ScreenCaptureRate)
		{
			TimeSinceLastScreenSave = currentTime;
			char fileName[] = "../../cs520_ScreenShots/xxx.ppm";
			
			fileName[24] = '0' + (ScreenCaptureIndex / 100);
			fileName[25] = '0' + ((ScreenCaptureIndex % 100) / 10);
			fileName[26] = '0' + (ScreenCaptureIndex % 10);
			
			saveScreenshot(WindowWidth, WindowHeight, fileName);

			ScreenCaptureIndex++;
		}

		if (m_gameContext->GetWorld() && m_gameContext->GetWorld()->m_customDelta > 0.0f)
		{
			deltaTime *= m_gameContext->GetWorld()->m_customDelta;
		}

		//deltaTime = 0.016f;

		if (bPaused)
		{
			deltaTime = 0.0f;
			bWasPausedLastFrame = false;
		}

		FrameDelay = 0.0f;

		if (FrameDeleyAccu > FrameDelay && !bPaused)
		{
			FrameDeleyAccu -= FrameDelay;
			float Delta = FrameDelay > deltaTime ? FixedDelta : deltaTime;

			m_gameContext->GetWorld()->Update(Delta);

			// 		PhysicsTimeAccumulator += deltaTime;
			// 		while (PhysicsTimeAccumulator >= PHYSICS_TIME_STEP)
			// 		{
			// 			m_gameContext->GetPhysicsManager()->Update(PHYSICS_TIME_STEP);
			// 			PhysicsTimeAccumulator -= PHYSICS_TIME_STEP;
			// 		}
			m_gameContext->GetPhysicsManager()->Update(Delta);
			m_gameContext->GetWorld()->PostPhysicsUpdate();
		}
		FrameDeleyAccu += deltaTime;
		
		/****     Update(deltaTime)    ****/
		
		//m_gameContext->GetWorld()->RenderWorld();
		m_gameContext->m_renderer->GetherDrawCalls(m_gameContext->GetWorld()->GetRenderableObjects());
		m_gameContext->m_renderer->Update(deltaTime);
		
		glfwSwapBuffers(m_gameContext->GetWindow());
		glfwPollEvents();
	}
}

void Game::processInput(GLFWwindow* contextWindow)
{
	if (m_gameContext->GetInputHandler() == nullptr)
	{
		return;
	}

	m_gameContext->GetInputHandler()->ProcessInput(contextWindow);
}

void Game::saveScreenshot(int windowWidth, int windowHeight, char *filename)
{
	if (filename == NULL)
		return;

	// Allocate a picture buffer 
	Pic * in = pic_alloc(windowWidth, windowHeight, 3, NULL);

	//printf("File to save to: %s\n", filename);

	for (int i = windowHeight - 1; i >= 0; i--)
	{
		glReadPixels(0, windowHeight - i - 1, windowWidth, 1, GL_RGB, GL_UNSIGNED_BYTE,
			&in->pix[i*in->nx*in->bpp]);
	}

	if (ppm_write(filename, in))
		printf("File saved Successfully\n");
	else
		printf("Error in Saving\n");

	pic_free(in);
}

void Game::Update(float Delta)
{
	
}

