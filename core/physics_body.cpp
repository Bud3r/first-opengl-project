#include "physics_body.h"

PhysicsBody::PhysicsBody(PhysicsServer* p_physics_server, const BodyCreationSettings& p_body_creation_settings) {
	m_physics_server = p_physics_server;
	BodyInterface& body_interface = p_physics_server->m_physics_system.GetBodyInterface();
	m_body_id = body_interface.CreateAndAddBody(p_body_creation_settings, EActivation::Activate);
}

PhysicsBody::~PhysicsBody() {
	Destroy();
}

void PhysicsBody::Init(PhysicsServer* p_physics_server, const BodyCreationSettings& p_body_creation_settings) {
	Destroy();
	m_physics_server = p_physics_server;
	BodyInterface& body_interface = p_physics_server->m_physics_system.GetBodyInterface();
	m_body_id = body_interface.CreateAndAddBody(p_body_creation_settings, EActivation::Activate);
}

void PhysicsBody::Destroy() {
	if (m_body_id.IsInvalid()) {
		return;
	}

	BodyInterface& body_interface = m_physics_server->m_physics_system.GetBodyInterface();
	body_interface.RemoveBody(m_body_id);
	body_interface.DestroyBody(m_body_id);
}

void PhysicsBody::SetLinearVelocity(Vec3Arg vec) const {
	m_physics_server->m_physics_system.GetBodyInterface().SetLinearVelocity(m_body_id, vec);
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