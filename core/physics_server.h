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

// REMOVE THESE.
using namespace JPH;
using namespace JPH::literals;

namespace layers {
    static constexpr ObjectLayer kStatic(0);
    static constexpr ObjectLayer kMoving(1);
    static constexpr ObjectLayer kCount(2);
}

namespace broad_phase_layers {
    static constexpr BroadPhaseLayer kStatic(0);
    static constexpr BroadPhaseLayer kMoving(1);
    static constexpr int kCount(2);
}

void SetupJolt();
void DismantleJolt();

class MyBroadPhaseLayerInterface final : public BroadPhaseLayerInterface
{
public:
    MyBroadPhaseLayerInterface()
    {
        mObjectToBroadPhase[layers::kStatic] = broad_phase_layers::kStatic;
        mObjectToBroadPhase[layers::kMoving] = broad_phase_layers::kMoving;
    };
private:
    virtual uint GetNumBroadPhaseLayers() const override
    {
        return broad_phase_layers::kCount;
    }

    virtual BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer inLayer) const override
    {
        return mObjectToBroadPhase[inLayer];
    }
private:
    BroadPhaseLayer	mObjectToBroadPhase[broad_phase_layers::kCount];
};

// Clean up variable names an code.
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