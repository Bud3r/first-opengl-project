#pragma once


class Engine;


class ProcessObject
{
public:
	virtual void Process(double deltaTime);
protected:
	Engine* engine;
};