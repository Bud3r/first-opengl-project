#pragma once


class Engine;


class ProcessObject
{
public:
	ProcessObject() { };
	virtual void AddedToEngine() { };
	virtual void Process(double deltaTime) { };
	Engine* engine = nullptr;
};