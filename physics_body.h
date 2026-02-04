#pragma once


#include "physics_server.h"

class PhysicsBody
{
public:
	PhysicsBody() {};
	PhysicsBody(PhysicsServer* p_physics_server, const BodyCreationSettings& p_body_creation_settings);
	~PhysicsBody();
	void SetLinearVelocity(Vec3Arg vec) const;
	void Init(PhysicsServer* p_physics_server, const BodyCreationSettings& p_body_creation_settings);
	void Destroy();
	RVec3 GetPosition() const;
	Quat GetRotation() const;
	BodyID GetID() const;
	operator BodyID() const { return m_body_id; }
private:
	PhysicsServer* m_physics_server;
	BodyID m_body_id = BodyID(BodyID::cInvalidBodyID);
};





