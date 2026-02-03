#pragma once

#include <iostream>
#include <cstdarg>
#include <thread>
#include <Jolt/Jolt.h>

// Jolt includes
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>


using namespace JPH;
using namespace JPH::literals;

namespace Layers {
    static constexpr ObjectLayer STATIC(0);
    static constexpr ObjectLayer MOVING(1);
    static constexpr ObjectLayer COUNT(2);
}

namespace BroadPhaseLayers {
    static constexpr BroadPhaseLayer STATIC(0);
    static constexpr BroadPhaseLayer MOVING(1);
    static constexpr int COUNT(2);
}

class MyBroadPhaseLayerInterface final : public BroadPhaseLayerInterface
{
public:
    MyBroadPhaseLayerInterface()
    {
        mObjectToBroadPhase[Layers::STATIC] = BroadPhaseLayers::STATIC;
        mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
    };
private:
    virtual uint GetNumBroadPhaseLayers() const override
    {
        return BroadPhaseLayers::COUNT;
    }

    virtual BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer inLayer) const override
    {
        return mObjectToBroadPhase[inLayer];
    }
private:
    BroadPhaseLayer	mObjectToBroadPhase[BroadPhaseLayers::COUNT];
};

class PhysicsServer
{
public:
    PhysicsServer();
    ~PhysicsServer();
    void Update(float deltaTime);
    uint32_t m_step_count = 0;
    PhysicsSystem m_physics_system;
    TempAllocatorImpl* temp_allocator;
    JobSystemThreadPool* job_system;
private:
    MyBroadPhaseLayerInterface broad_phase_layer_interface;
    ObjectVsBroadPhaseLayerFilter object_vs_broad_phase_layer_filter;
    ObjectLayerPairFilter object_layer_pair_filter;
};


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


//void example(Physci) {
//    BodyInterface& body_interface = m_physics_system.GetBodyInterface();
//    BoxShapeSettings floor_shape_settings(Vec3(100.0f, 1.0f, 100.0f));
//    ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
//    ShapeRefC floor_shape = floor_shape_result.Get();
//    BodyCreationSettings floor_settings(floor_shape, RVec3(0.0, -1.0, 0.0), Quat::sIdentity(), EMotionType::Static, Layers::MOVING);
//    Body* floor = body_interface.CreateBody(floor_settings);
//
//    body_interface.AddBody(floor->GetID(), EActivation::Activate);
//
//    BodyCreationSettings sphere_settings(new SphereShape(0.5f), RVec3(0.0f, 2.0f, 0.0f), Quat::sIdentity(), EMotionType::Dynamic, Layers::STATIC);
//    BodyID sphere_id = body_interface.CreateAndAddBody(sphere_settings, EActivation::Activate);
//    body_interface.SetLinearVelocity(sphere_id, Vec3(0.0f, -5.0f, 0.0f));
//}