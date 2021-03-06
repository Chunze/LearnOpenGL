
#include "ParticleWorld.h"
#include "ParticleAttractor.h"
#include "HW2InputHandler.h"
#include "ParticleSystem.h"

ParticleWorld::ParticleWorld(GameContext* gameContext, int _particleNum)
	: World(gameContext),
	ParticleNum(_particleNum)
{
	InitCamera();
	InitInputHandler();
	InitParticleSystem();

	m_DynamicAttractor = new ParticleAttractor(gameContext, this, glm::vec2(100, 100));
	Attractors.push_back(m_DynamicAttractor);
	
}

void ParticleWorld::Update(float Delta)
{
	m_particleSystem->Update(Delta);
}

void ParticleWorld::CreateAttractor(int x, int y)
{
	if (Attractors.size() >= MaxAttractorNum)
	{
		Attractors[AttractorToBeModified]->SetPosition(x, y);
		AttractorToBeModified = (AttractorToBeModified + 1) % MaxAttractorNum;
	}
	ParticleAttractor* NewAttractor = new ParticleAttractor(m_gameContext, this, glm::vec2(x, y));
	Attractors.push_back(NewAttractor);

	// reset number of left clicks
	NumLeftClicks = 0;
}

void ParticleWorld::UpdateDynamicAttractor(int x, int y)
{
	m_DynamicAttractor->SetPosition(x, y);
}

void ParticleWorld::LeftClicked()
{
	NumLeftClicks++;
	if (NumLeftClicks >= 3)
	{
		NumLeftClicks = 0;
		ClearAttractors();
	}
}

void ParticleWorld::InitCamera()
{
	m_Camera = new Camera(CameraType::Camera_2D, 0.0f, static_cast<float>(m_gameContext->GetGame()->WindowWidth), 0.0f, static_cast<float>(m_gameContext->GetGame()->WindowHeight));
	m_Camera->SetupCamera(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	m_gameContext->GetRenderer()->SetActiveCamera(m_Camera);
}

void ParticleWorld::InitInputHandler()
{
	m_gameContext->m_InputHandler = new HW2InputHandler(m_gameContext);
}

void ParticleWorld::InitParticleSystem()
{
	m_particleSystem = new ParticleSystem(m_gameContext, this);
	m_particleSystem->Initialize(ParticleNum);
}

void ParticleWorld::ClearAttractors()
{
	Attractors.clear();
	AttractorToBeModified = 0;
}
