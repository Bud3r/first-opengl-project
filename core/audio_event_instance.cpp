#include "audio_event_instance.h"
#include "engine.h"

AudioEventInstance::AudioEventInstance(const char* event_path) {
	FMOD::Studio::EventDescription* event_description;
	FMOD_RESULT err = Engine::GetFmodSystem()->getEvent(event_path, &event_description);
	event_description->createInstance(&event_instance_);
	if (err != FMOD_RESULT::FMOD_OK) {
		std::cerr << err << std::endl;
	}
}

AudioEventInstance::AudioEventInstance(FMOD::Studio::EventDescription* event_description) {
	FMOD_RESULT err = event_description->createInstance(&event_instance_);
	if (err != FMOD_RESULT::FMOD_OK) {
		std::cerr << err << std::endl;
	}
}

AudioEventInstance::~AudioEventInstance() {
	event_instance_->release();
}

void AudioEventInstance::Play() {
	FMOD_RESULT err = event_instance_->start();
	std::cout << "Valid: " << event_instance_->isValid() << std::endl;
	if (err != FMOD_RESULT::FMOD_OK) {
		std::cerr << err << std::endl;
	}
}

void AudioEventInstance::SetPosition(glm::vec3 position) {
	FMOD_3D_ATTRIBUTES attribs{
		FMOD_VECTOR { position.x, position.y, position.z },
		FMOD_VECTOR { 0.0f, 0.0f, 0.0f },
		FMOD_VECTOR { 0.0f, 0.0f, 1.0f },
		FMOD_VECTOR { 0.0f, 1.0f, 0.0f }
	};
	event_instance_->set3DAttributes(&attribs);
}
