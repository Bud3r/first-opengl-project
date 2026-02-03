#pragma once


#include "physics_server.h"


class PhysicsBody
{
public:
	PhysicsBody(PhysicsServer* p_physics_server, BodyCreationSettings p_body_creation_settings);
	~PhysicsBody();
	RVec3 GetPosition() const;
	Quat GetRotation() const;
	BodyID GetID() const;
	operator BodyID() const { return m_body_id; }
private:
	PhysicsServer* m_physics_server;
	BodyID m_body_id;
};

PhysicsBody::PhysicsBody(PhysicsServer* p_physics_server, BodyCreationSettings p_body_creation_settings)
{
	m_physics_server = p_physics_server;
	BodyInterface& body_interface = p_physics_server->m_physics_system.GetBodyInterface();
	m_body_id = body_interface.CreateAndAddBody(p_body_creation_settings, EActivation::Activate);
}

PhysicsBody::~PhysicsBody()
{
	BodyInterface& body_interface = m_physics_server->m_physics_system.GetBodyInterface();
	body_interface.RemoveBody(m_body_id);
	body_interface.DestroyBody(m_body_id);
}

BodyID PhysicsBody::GetID() const {
	return m_body_id;
}

RVec3 PhysicsBody::GetPosition() const {
	return m_physics_server->m_physics_system.GetBodyInterface().GetPosition(m_body_id);
}

Quat PhysicsBody::GetRotation() const {
	return m_physics_server->m_physics_system.GetBodyInterface().GetRotation(m_body_id);
}

