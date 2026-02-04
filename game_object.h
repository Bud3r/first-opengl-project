#pragma once


class Engine;


class GameObject
{
public:
	GameObject() { };
	virtual void AddedToEngine() { };
	virtual void Process(double deltaTime) { };
	/// <summary>
	/// Engine pointer is set after constructor but before the AddedToEngine().
	/// </summary>
	Engine* engine = nullptr;
};