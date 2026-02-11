#pragma once

#include <iostream>
#include <cstdarg>
#include <thread>
#include <Jolt/Jolt.h>
#include <memory>

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

void SetupJolt();
void DismantleJolt();

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
    std::unique_ptr<TempAllocatorImpl> temp_allocator;
    std::unique_ptr<JobSystemThreadPool> job_system;
private:
    MyBroadPhaseLayerInterface broad_phase_layer_interface;
    ObjectVsBroadPhaseLayerFilter object_vs_broad_phase_layer_filter;
    ObjectLayerPairFilter object_layer_pair_filter;
};