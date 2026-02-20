#include "physics_body.h"


PhysicsBody::PhysicsBody(PhysicsServer* p_physics_server, const JPH::BodyCreationSettings& p_body_creation_settings) {
	physics_server_ = p_physics_server;
	JPH::BodyInterface& body_interface = p_physics_server->physics_system.GetBodyInterface();
	body_id_ = body_interface.CreateAndAddBody(p_body_creation_settings, JPH::EActivation::Activate);
}

PhysicsBody::~PhysicsBody() {
	Destroy();
}

void PhysicsBody::Init(PhysicsServer* p_physics_server, const JPH::BodyCreationSettings& p_body_creation_settings) {
	Destroy();
	physics_server_ = p_physics_server;
	JPH::BodyInterface& body_interface = p_physics_server->physics_system.GetBodyInterface();
	body_id_ = body_interface.CreateAndAddBody(p_body_creation_settings, JPH::EActivation::Activate);
}

void PhysicsBody::Destroy() {
	if (body_id_.IsInvalid()) {
		return;
	}

	JPH::BodyInterface& body_interface = physics_server_->physics_system.GetBodyInterface();
	body_interface.RemoveBody(body_id_);
	body_interface.DestroyBody(body_id_);
}

void PhysicsBody::SetLinearVelocity(JPH::Vec3Arg vec) const {
	physics_server_->physics_system.GetBodyInterface().SetLinearVelocity(body_id_, vec);
}

void PhysicsBody::ApplyForce(JPH::Vec3Arg force) {
	physics_server_->physics_system.GetBodyInterface().AddForce(body_id_, force);
}

void PhysicsBody::ApplyImpulse(JPH::Vec3Arg impulse) {
	physics_server_->physics_system.GetBodyInterface().AddImpulse(body_id_, impulse);
}

JPH::BodyID PhysicsBody::GetID() const {
	return body_id_;
}

JPH::RVec3 PhysicsBody::GetPosition() const {
	return physics_server_->physics_system.GetBodyInterface().GetPosition(body_id_);
}

JPH::Quat PhysicsBody::GetRotation() const {
	return physics_server_->physics_system.GetBodyInterface().GetRotation(body_id_);
}

void PhysicsBody::SetPosition(JPH::RVec3Arg position, JPH::EActivation activation_mode) {
	physics_server_->physics_system.GetBodyInterface().SetPosition(body_id_, position, activation_mode);
}

void PhysicsBody::SetRotation(JPH::QuatArg rotation, JPH::EActivation activation_mode) {
	physics_server_->physics_system.GetBodyInterface().SetRotation(body_id_, rotation, activation_mode);
}
