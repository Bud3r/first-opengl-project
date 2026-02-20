#pragma once

#include "physics_server.h"

class PhysicsBody
{
public:
	PhysicsBody() { };
	PhysicsBody(PhysicsServer* p_physics_server, const JPH::BodyCreationSettings& p_body_creation_settings);
	~PhysicsBody();
	void SetLinearVelocity(JPH::Vec3Arg vec) const;
	void Init(PhysicsServer* p_physics_server, const JPH::BodyCreationSettings& p_body_creation_settings);
	void Destroy();
	void ApplyImpulse(JPH::Vec3Arg impulse);
	void ApplyForce(JPH::Vec3Arg force);
	JPH::RVec3 GetPosition() const;
	JPH::Quat GetRotation() const;
	void SetPosition(JPH::RVec3Arg position, JPH::EActivation activation = JPH::EActivation::DontActivate);
	void SetRotation(JPH::QuatArg rotation, JPH::EActivation activation = JPH::EActivation::DontActivate);
	JPH::BodyID GetID() const;
	operator JPH::BodyID() const { return body_id_; }
private:
	PhysicsServer* physics_server_;
	JPH::BodyID body_id_ = JPH::BodyID(JPH::BodyID::cInvalidBodyID);
};





