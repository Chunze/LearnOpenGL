#include "RigidBox.h"

RigidBox::RigidBox(vec3 HalfSize)
	: m_HalfSize(HalfSize)
{
	ConstructInertiaTensor();
}

RigidBox::RigidBox()
{
	// Default 1 x 1 x 1 box
	m_HalfSize = vec3(0.5f);
	ConstructInertiaTensor();
}

void RigidBox::ConstructInertiaTensor()
{
	mat3 InertiaTensor(0.0f);

	float x_sq = 4 * m_HalfSize[0] * m_HalfSize[0];
	float y_sq = 4 * m_HalfSize[1] * m_HalfSize[1];
	float z_sq = 4 * m_HalfSize[2] * m_HalfSize[2];
	InertiaTensor[0][0] = y_sq + z_sq;
	InertiaTensor[1][1] = x_sq + z_sq;
	InertiaTensor[2][2] = x_sq + y_sq;

	InertiaTensor /= 12.0f;

	SetInertirTensor(InertiaTensor);
}

