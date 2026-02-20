#include "physics_model_game_object.h"


class Ball : public PhysicsModelGameObject {
public:
	using PhysicsModelGameObject::PhysicsModelGameObject;
	virtual void Process(double delta_time) override;
private:
	double time_ = 0.0;
};
