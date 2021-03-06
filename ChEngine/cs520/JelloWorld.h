#pragma once
#include "World.h"
#include "glm.h"

class JelloWorld : public World
{
public:
	JelloWorld(GameContext* gameContext);

	~JelloWorld() override;

	class Jello* m_jello;

	char m_intergrator[10];		// "RK4" or "Euler"
	int m_n;					// display only every nth timestep
	int m_incPlanePresent;		// Is the inclined plane present? 1 = YES, 0 = NO
	float a, b, c, d;			// inclined plane has equation a * x + b * y + c * z + d = 0;
	float m_kCollision;			// Hook's elasticity coefficient for collision springs
	float m_dCollision;			// Damping coefficient collision springs
	int m_resolution;			// resolution for the 3d grid specifying the external force field; value of 0 means that there is no force field
	glm::vec3 * m_forceField;	// pointer to the array of values of the force field

	glm::vec3 m_lightPosition = glm::vec3(0.0f, 6.0f, 0.0f);

	int RenderMode = 0;

	int ShowShearSpring = 0;
	int ShowBendSpring = 0;
	int ShowStructuralSpring = 0;
	DrawListElement DebugDrawElement;
	Shader* debugShader;
	Light* m_light;

	void ToggleDrawingMode();
	void ToggleStructuralSpring();
	void ToggleShearSpring();
	void ToggleBendSpring();

	virtual bool LoadWorld(const char* fileName);
	virtual void Update(float Delta);

	// checking the jello against the boundary, generate collision spring if out of bound
	void CheckBoundary();

	void CheckBoundary(class Particle* CurrentParticle);

	glm::vec3 GetForceInForceField(glm::vec3 position) const;

	Jello* GetJello() { return m_jello; }

protected:

	virtual void InitCamera() override;
	void InitInputHandler();
	void InitDebugElement();
	void InitLight();

private:
	float BoundaryLineColor = 0.7f;
};