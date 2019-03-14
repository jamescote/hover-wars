#pragma once
#include "stdafx.h"
#include "InteractableEntity.h"
#include "MeshManager.h"
#include "EntityComponentHeaders/RenderComponent.h"
#include "EntityComponentHeaders/PhysicsComponent.h"

// Name: InteractableEntity
// Written by: James Cote
// Description: General Entity for objects that players can interact with.
//        These types of entities can be objects such as pickups or killzones (i.e. flame paths).
//        Idea: these entities, when a player collides with them, will send a message to that player 
//            telling them what happens as a result of this collision. Different messages can be generated
//            by inheriting from this class and defining their own message.
class FlameTrail
    : public InteractableEntity
{
public:
    FlameTrail(int iID, int iOwnerID, const vec3* vPosition,
                float fHeight, float fWidth);
    virtual ~FlameTrail();

    // Implementation of inherited functionality
    virtual void initialize();
    void update(float fTimeInMilliseconds);
    void handleCollision(Entity* pOther, unsigned int iColliderMsg, unsigned int iVictimMsg, PxVec3 vNormal, PxVec3 vPosition);
    void getSpatialDimensions(vec3* pNegativeCorner, vec3* pPositiveCorner) const;

    // Game Specific Logic for Interacting with Object
    void getInteractionResult() const { /*Not Implemented*/ }

    void spawnFlame(const vec3* vNormal, const vec3* vPosition);

private:
    float m_fHeight, m_fWidth;
};
