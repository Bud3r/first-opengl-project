#include "physics_server.h"

void SetupJolt() {
    RegisterDefaultAllocator();
    Factory::sInstance = new Factory();

    RegisterTypes();
}

void DismantleJolt() {
    UnregisterTypes();

    delete Factory::sInstance;
    Factory::sInstance = nullptr;
}


PhysicsServer::PhysicsServer()
{
    temp_allocator = new TempAllocatorImpl(10 * 1024 * 1024);
    job_system = new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

    const uint cMaxBodies = 1024;
    const uint cNumBodyMutexes = 0;
    const uint cMaxBodyPairs = 1024;
    const uint cMaxContactConstraints = 1024;

    m_physics_system.Init(
        cMaxBodies,
        cNumBodyMutexes,
        cMaxBodyPairs,
        cMaxContactConstraints,
        broad_phase_layer_interface,
        object_vs_broad_phase_layer_filter,
        object_layer_pair_filter
    );
}

PhysicsServer::~PhysicsServer()
{
    delete temp_allocator;
    delete job_system;
}

void PhysicsServer::Update(float deltaTime) {
    const int cCollisionSteps = 1;

    m_physics_system.Update(deltaTime, cCollisionSteps, temp_allocator, job_system);

    m_step_count += cCollisionSteps;
}