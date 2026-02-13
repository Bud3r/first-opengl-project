 #pragma once


#include "assert.h"


class Engine;
struct InputEvent;


class GameObject
{
 public:
	GameObject() { };

	virtual void AddedToEngine() { };
	virtual void Process(double deltaTime) { };
	virtual void ProcessInput(InputEvent& event) { };

	inline Engine& GetEngine() {
		assert(engine != nullptr && "engine pointer is null FILE: " __FILE__);
		return *engine;
	};

	/// <summary>
	/// Engine pointer is set after constructor but before the AddedToEngine().
	/// </summary>
	Engine* engine = nullptr;
};