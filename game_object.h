#pragma once


#include "assert.h"


class Engine;


class GameObject
{
public:
	GameObject() { };
	inline Engine& get_engine() {
		assert(engine != nullptr && "engine pointer is null FILE: " __FILE__);
		return *engine;
	};
	virtual void AddedToEngine() { };
	virtual void Process(double deltaTime) { };
	/// <summary>
	/// Engine pointer is set after constructor but before the AddedToEngine().
	/// Use get_engine for the asse
	/// </summary>
	Engine* engine = nullptr;
};