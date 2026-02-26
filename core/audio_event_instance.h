#pragma once

#include "fmod_studio.hpp"
#include <glm\glm.hpp>

class AudioEventInstance {
public:
	AudioEventInstance(const char* event_path);
	AudioEventInstance(FMOD::Studio::EventDescription* event_description);
	~AudioEventInstance();
	void SetPosition(glm::vec3 position);
	void Play();
private:
	FMOD::Studio::EventInstance* event_instance_;


};
