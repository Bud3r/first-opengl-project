#include "physics_server.h"


PhysicsServer::PhysicsServer()
{
    RegisterDefaultAllocator();
    Factory::sInstance = new Factory();

    RegisterTypes();

    temp_allocator = std::make_unique<TempAllocatorImpl>(10 * 1024 * 1024);
    job_system = std::make_unique<JobSystemThreadPool>(cMaxPhysicsJobs, cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

    const uint cMaxBodies = 1024;
    const uint cNumBodyMutexes = 0;
    const uint cMaxBodyPairs = 1024;
    const uint cMaxContactConstraints = 1024;

    m_physics_system.Init(
        cMaxBodies ,
        cNumBodyMutexes,
        cMaxBodyPairs,
        cMaxContactConstraints,
        broad_phase_layer_interface,
        object_vs_broad_phase_layer_filter,
        object_layer_pair_filter
    );
}

PhysicsServer::~PhysicsServer() {
    UnregisterTypes();

    delete Factory::sInstance;
    Factory::sInstance = nullptr;
}


void PhysicsServer::Update(float deltaTime) {
    const int cCollisionSteps = 1;

    m_physics_system.Update(deltaTime, cCollisionSteps, temp_allocator.get(), job_system.get());

    m_step_count += cCollisionSteps;
}