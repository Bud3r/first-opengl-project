#include "physics_body.h"

PhysicsBody::PhysicsBody(PhysicsServer* p_physics_server, const BodyCreationSettings& p_body_creation_settings) {
	physics_server_ = p_physics_server;
	BodyInterface& body_interface = p_physics_server->m_physics_system.GetBodyInterface();
	body_id_ = body_interface.CreateAndAddBody(p_body_creation_settings, EActivation::Activate);
}

PhysicsBody::~PhysicsBody() {
	Destroy();
}

void PhysicsBody::Init(PhysicsServer* p_physics_server, const BodyCreationSettings& p_body_creation_settings) {
	Destroy();
	physics_server_ = p_physics_server;
	BodyInterface& body_interface = p_physics_server->m_physics_system.GetBodyInterface();
	body_id_ = body_interface.CreateAndAddBody(p_body_creation_settings, EActivation::Activate);
}

void PhysicsBody::Destroy() {
	if (body_id_.IsInvalid()) {
		return;
	}

	BodyInterface& body_interface = physics_server_->m_physics_system.GetBodyInterface();
	body_interface.RemoveBody(body_id_);
	body_interface.DestroyBody(body_id_);
}

void PhysicsBody::SetLinearVelocity(Vec3Arg vec) const {
	physics_server_->m_physics_system.GetBodyInterface().SetLinearVelocity(body_id_, vec);
}

BodyID PhysicsBody::GetID() const {
	return body_id_;
}

RVec3 PhysicsBody::GetPosition() const {
	return physics_server_->m_physics_system.GetBodyInterface().GetPosition(body_id_);
}

Quat PhysicsBody::GetRotation() const {
	return physics_server_->m_physics_system.GetBodyInterface().GetRotation(body_id_);
}