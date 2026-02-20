#include "physics_server.h"


PhysicsServer::PhysicsServer()
{
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();

    JPH::RegisterTypes();

    temp_allocator = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
    job_system = std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

    const uint32_t cMaxBodies = 1024;
    const uint32_t cNumBodyMutexes = 0;
    const uint32_t cMaxBodyPairs = 1024;
    const uint32_t cMaxContactConstraints = 1024;

    physics_system.Init(
        cMaxBodies ,
        cNumBodyMutexes,
        cMaxBodyPairs,
        cMaxContactConstraints,
        broad_phase_layer_interface_,
        object_vs_broad_phase_layer_filter_,
        object_layer_pair_filter_
    );
}

PhysicsServer::~PhysicsServer() {
    JPH::UnregisterTypes();

    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}


void PhysicsServer::Update(float delta_time) {
    time_since_last_update_ += delta_time;

    float step_count = floor(time_since_last_update_ / time_between_updates_);
    float delta = step_count * time_between_updates_;
    time_since_last_update_ -= delta;

    int i_step_count = static_cast<int>(step_count);

    for (size_t i = 0; i < i_step_count; i++)
    {
        physics_system.Update(delta, i_step_count, temp_allocator.get(), job_system.get());
        step_count += i_step_count;
    }
}