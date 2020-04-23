#include "cPhysWorld.h"


cPhysWorld::~cPhysWorld()
{
	delete m_dynamics_world_;
	delete m_collision_dispatcher;
	delete m_solver;
	delete m_collision_configuration;
	delete m_overlapping_pair_cache;
}

void cPhysWorld::Update(float dt)
{
	m_dynamics_world_->stepSimulation(dt, 10);

	
	nPhysics::iFlipperComponent* flipper1 = dynamic_cast<nPhysics::iFlipperComponent*>(vp_phy_components[0]);
	nPhysics::iFlipperComponent* flipper2 = dynamic_cast<nPhysics::iFlipperComponent*>(vp_phy_components[1]);

	//if(GetkeyPressed(1) == 'c')
	if(GetkeyPressed(1))
	{
		flipper1->GoToAngle(0.5f, dt,20);
	}
	else
	{
		flipper1->GoToAngle(0.5f, dt,-20);
	}
	//if(GetkeyPressed()=='v')
	//{
	//	flipper1->GoToAngle(2.f, dt,-20);
	//}
	//if(GetkeyPressed()=='z')
	//{
	//	flipper2->GoToAngle(2.f, dt,20);
	//}
	//if(GetkeyPressed(0)=='x')
	if(GetkeyPressed(0))
	{
		flipper2->GoToAngle(0.5f, dt,-20);
	}
	else
	{
		flipper2->GoToAngle(0.5f, dt,20);
	}
	if(m_collision_listener_)
	{
		//TODO
	}
}

bool cPhysWorld::AddComponent(nPhysics::iPhysicsComponent* component)
{
	if (!component)
	{
		return false;
	}
	

	vp_phy_components.push_back(component);
	switch (component->GetComponentType())
	{
	case nPhysics::eComponentType::SPHERE:
		
		return AddRigidBodies(dynamic_cast<cBallComponent*>(component));
	case nPhysics::eComponentType::PLANE:
		return AddRigidBodies(dynamic_cast<cPlaneComponent*>(component));
	case nPhysics::eComponentType::FLIPPER:
		{
		AddHingeConstraint(dynamic_cast<cBtFlipperComponent*>(component));
		return AddRigidBodies(dynamic_cast<cBtFlipperComponent*>(component));
		}
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

void cPhysWorld::SetUpAi()
{
	
}

void cPhysWorld::GetOnPath(bool onPath)
{
}

void cPhysWorld::GetIsFlock(bool is_flock)
{
}

void cPhysWorld::GetIsReverse(bool isReserve)
{
}

void cPhysWorld::GetFormationType(int type)
{
}
bool cPhysWorld::GetkeyPressed(int obj)
{
	if(obj == 0)
	{
		return input_L_;
	}
	else if(obj == 1)
	{
		return input_R_;
	}

	//return input;
}
bool cPhysWorld::SetkeyPressed(bool keyPressed, int obj)
{
	if (obj == 0)
	{
		input_L_ = keyPressed;
	}
	else if (obj == 1)
	{
		input_R_ = keyPressed;
	}
	//input = keyPressed;
	return 1;
}
void cPhysWorld::SetCollisionListener(nPhysics::iCollisionListener* collision_listener)
{
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
	m_collision_listener_ = 0;
	
	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	m_collision_configuration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	 m_collision_dispatcher = new btCollisionDispatcher(m_collision_configuration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	m_overlapping_pair_cache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	 m_solver = new btSequentialImpulseConstraintSolver;

	 m_dynamics_world_ = new btDiscreteDynamicsWorld(m_collision_dispatcher, m_overlapping_pair_cache, m_solver, m_collision_configuration);

	 m_dynamics_world_->setGravity(btVector3(0, -10, 0));

	///-----initialization_end-----

}

bool cPhysWorld::AddRigidBodies(cBallComponent* component)
{
	if(!component)
	{
		return false;
	}
	m_dynamics_world_->addRigidBody(component->m_body_);
	return true;
}

bool cPhysWorld::AddRigidBodies(cPlaneComponent* component)
{
	if (!component)
	{
		return false;
	}
	m_dynamics_world_->addRigidBody(component->m_body_);
	return true;
}

bool cPhysWorld::AddRigidBodies(cBtFlipperComponent* component)
{
	if (!component)
	{
		return false;
	}
	m_dynamics_world_->addRigidBody(component->m_body_);
	return true;
}

bool cPhysWorld::AddHingeConstraint(cBtFlipperComponent* component)
{
	component->m_constraint_ = new btHingeConstraint(*component->m_body_, nConvert::ToBullet(component->m_half_length), nConvert::ToBullet(glm::vec3(0, 0, 1)), true);
	m_dynamics_world_->addConstraint(component->m_constraint_);
	return true;
}
