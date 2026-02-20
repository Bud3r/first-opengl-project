#pragma once

#include <iostream>
#include <cstdarg>
#include <thread>
#include <memory>

// Jolt includes
#include <Jolt/Jolt.h>
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

// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
JPH_SUPPRESS_WARNINGS

namespace layers {
    static constexpr JPH::ObjectLayer kStatic(0);
    static constexpr JPH::ObjectLayer kMoving(1);
    static constexpr JPH::ObjectLayer kCount(2);
}

namespace broad_phase_layers {
    static constexpr JPH::BroadPhaseLayer kStatic(0);
    static constexpr JPH::BroadPhaseLayer kMoving(1);
    static constexpr int kCount(2);
}

using namespace JPH::literals;

void SetupJolt();
void DismantleJolt();

class MyBroadPhaseLayerInterface final : public JPH::BroadPhaseLayerInterface
{
public:
    MyBroadPhaseLayerInterface()
    {
        mObjectToBroadPhase[layers::kStatic] = broad_phase_layers::kStatic;
        mObjectToBroadPhase[layers::kMoving] = broad_phase_layers::kMoving;
    };
private:
    virtual uint32_t GetNumBroadPhaseLayers() const override
    {
        return broad_phase_layers::kCount;
    }

    virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
    {
        return mObjectToBroadPhase[inLayer];
    }
private:
    JPH::BroadPhaseLayer	mObjectToBroadPhase[broad_phase_layers::kCount];
};

// Clean up variable names an code.
class PhysicsServer
{
public:
    PhysicsServer();
    ~PhysicsServer();
    void Update(float deltaTime);
    uint32_t step_count = 0;
    JPH::PhysicsSystem physics_system;
    std::unique_ptr<JPH::TempAllocatorImpl> temp_allocator;
    std::unique_ptr<JPH::JobSystemThreadPool> job_system;
private:
    float time_since_last_update_ = 0.0;
    float time_between_updates_ = 1.0f / 60.0f;
    MyBroadPhaseLayerInterface broad_phase_layer_interface_;
    JPH::ObjectVsBroadPhaseLayerFilter object_vs_broad_phase_layer_filter_;
    JPH::ObjectLayerPairFilter object_layer_pair_filter_;
};