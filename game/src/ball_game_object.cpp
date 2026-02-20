#include "ball_game_object.h"

void Ball::Process(double delta_time) {
	PhysicsModelGameObject::Process(delta_time);

	time_ += delta_time;

	const double kTimeBetweenJumps = 1.0;

	if (time_ >= kTimeBetweenJumps) {
		time_ -= kTimeBetweenJumps;
		body_.ApplyImpulse({ 0.0, 1.0, 0.0 });
	}
}
