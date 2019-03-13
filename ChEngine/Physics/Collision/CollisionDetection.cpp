#include "CollisionDetection.h"
#include "PhysicsManager.h"

CollisionDetection::CollisionDetection(PhysicsManager* PhysicsManager)
	: m_PhysicsManager(PhysicsManager),
	  m_BroadPhaseTest(this),
	  m_NarrowPhaseTest(this)
{
	
}

void CollisionDetection::RunCollisionDetection(CollisionInfo *Data)
{
	m_BroadPhaseTest.RunTest();

	m_NarrowPhaseTest.RunTest(Data);
}

void CollisionDetection::RegisterCollisionPrimitive(CollisionPrimitive_sp PrimitiveToAdd)
{
	if (!PrimitiveToAdd->GetBody())
	{
		for (auto primitive : m_PrimitiveWithoutBody)
		{
			if (primitive == PrimitiveToAdd)
			{
				return;
			}
		}
		m_PrimitiveWithoutBody.push_back(PrimitiveToAdd);
	}
}

void CollisionDetection::RunBroadPhase()
{

}

void CollisionDetection::RunNarrowPhase()
{

}

