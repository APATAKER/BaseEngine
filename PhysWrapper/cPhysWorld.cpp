#include "cPhysWorld.h"


cPhysWorld::~cPhysWorld()
{
	delete world_;
}

void cPhysWorld::Update(float dt)
{
	
	world_->Update(dt);

	
}

bool cPhysWorld::AddComponent(nPhysics::iPhysicsComponent* component)
{
	if (!component)
	{
		return false;
	}
	
	// populating this for checking purpose *not useful
	this->vp_phy_components.push_back(component);


	switch (component->GetComponentType())
	{
	case nPhysics::eComponentType::SPHERE:
		return AddRigidBodies(dynamic_cast<cBallComponent*>(component));
	case nPhysics::eComponentType::PLANE:
		return AddRigidBodies(dynamic_cast<cPlaneComponent*>(component));
	default:
		break;
	}
	
	
	return true;
}

bool cPhysWorld::RemoveComponent(nPhysics::iPhysicsComponent* component)
{

	// just for starting up this function *not useful
	for(int i= 0;i<this->vp_phy_components.size();i++)
	{
		if (vp_phy_components[i] == component)
			vp_phy_components.erase(vp_phy_components.begin() + (i - 1));
	}
	return true;
}

//void cPhysWorld::SetCollisionListener(nPhysics::iCollisionListener* collision_listener)
//{
//	if(m_collision_listener_)
//	{
//		world_->SetCollisionListener(0);
//		delete m_collision_listener_;
//		m_collision_listener_ = 0;
//	}
//	if(collision_listener)
//	{
//		m_collision_listener_ = new cPhysCollisionListener(collision_listener);
//		world_->SetCollisionListener(m_collision_listener_);
//	}
//}

cPhysWorld::cPhysWorld()
{
	world_ = new physLib::cWorld();    
}

bool cPhysWorld::AddRigidBodies(cBallComponent* component)
{
	if(!component)
	{
		return false;
	}
	return world_->AddRigidBody(component->rigid_body);
}

bool cPhysWorld::AddRigidBodies(cPlaneComponent* component)
{
	if (!component)
	{
		return false;
	}
	return world_->AddRigidBody(component->rigid_body);
}
