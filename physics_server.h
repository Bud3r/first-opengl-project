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

void physics_main() {
    RegisterDefaultAllocator();
    Factory::sInstance = new Factory();

    RegisterTypes();
    TempAllocatorImpl temp_allocator(10 * 1024 * 1024);
    JobSystemThreadPool job_system(cMaxPhysicsJobs, cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
    
    const uint32_t cMaxBodies = 1024;
    const uint32_t cNumBodyMutexes = 0;
    const uint32_t cMaxBodyPairs = 1024;
    const uint32_t cMaxContactConstraints = 1024;

    MyBroadPhaseLayerInterface broad_phase_layer_interface;
    ObjectVsBroadPhaseLayerFilter object_vs_broad_phase_layer_filter;
    ObjectLayerPairFilter object_layer_pair_filter;
    
    PhysicsSystem physics_system;

    physics_system.Init(
        cMaxBodies,
        cNumBodyMutexes,
        cMaxBodyPairs,
        cMaxContactConstraints,
        broad_phase_layer_interface,
        object_vs_broad_phase_layer_filter,
        object_layer_pair_filter
    );
    
    BodyInterface& body_interface = physics_system.GetBodyInterface();
    BoxShapeSettings floor_shape_settings(Vec3(100.0f, 1.0f, 100.0f));
    ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
    ShapeRefC floor_shape = floor_shape_result.Get();
    BodyCreationSettings floor_settings(floor_shape, RVec3(0.0, -1.0, 0.0), Quat::sIdentity(), EMotionType::Static, Layers::MOVING);
    Body* floor = body_interface.CreateBody(floor_settings);

    body_interface.AddBody(floor->GetID(), EActivation::Activate);

    BodyCreationSettings sphere_settings(new SphereShape(0.5f), RVec3(0.0f, 2.0f, 0.0f), Quat::sIdentity(), EMotionType::Dynamic, Layers::STATIC);
    BodyID sphere_id = body_interface.CreateAndAddBody(sphere_settings, EActivation::Activate);
    body_interface.SetLinearVelocity(sphere_id, Vec3(0.0f, -5.0f, 0.0f));

    const float cDeltaTime = 1.0f / 60.0f;
    uint step_count = 0;

    while (body_interface.IsActive(sphere_id))
    {
        const int cCollisionSteps = 1;

        auto position = body_interface.GetPosition(sphere_id);
        std::cout << position.GetX() << " " << position.GetY() << " " << position.GetY() << std::endl;
        physics_system.Update(cDeltaTime, cCollisionSteps, &temp_allocator, &job_system);

        step_count += cCollisionSteps;
    }

    body_interface.RemoveBody(sphere_id);
    body_interface.DestroyBody(sphere_id);
    body_interface.RemoveBody(floor->GetID());
    body_interface.DestroyBody(floor->GetID());

    UnregisterTypes();

    delete Factory::sInstance;
    Factory::sInstance = nullptr;

    std::printf("Body not active anymore");
}