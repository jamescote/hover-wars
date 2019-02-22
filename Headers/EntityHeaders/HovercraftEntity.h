#pragma once
#include "Entity.h"
#include "Mesh.h"
#include "EntityComponentHeaders/RenderComponent.h"
#include "EntityComponentHeaders/PhysicsComponent.h"
#include "EntityComponentHeaders/CameraComponent.h"
#include "InteractableEntity.h"
#include "SpatialDataMap.h"
#include "GameStats.h"
#include <queue>

/***********\
 * Defines *
\***********/
#define MAX_CAMERAS_PER_PLAYER  2 

// Ability Defines
/*
Rocket
Spikes
Trail
Dash - (all 4 directions count as 1 ability for cool down purposes)
*/
#define ABILITY_COUNT           4

/*
Cooldowns

The time the hovercraft must wait until they can use the ability again.
*/
#define ROCKET_COOLDOWN         5.0f    // seconds
#define SPIKES_COOLDOWN         10.0f   // seconds
#define TRAIL_COOLDOWN          0.0f    // seconds
#define DASH_COOLDOWN           5.0f    // seconds

/*
Total time the trail can be activated from full to empty.

@TODO why is the time value so different than all the other time values.
They ALL behave like seconds, but not this.

Unit: seconds
*/
#define TRAIL_GAUGE_FULL        0.01f
/*
Represents the trail gauge is empty. 
*/
#define TRAIL_GAUGE_EMPTY       0.0f
/*
Total time for the trail to recharge from empty to full.

@NOTE: Currently unused. Right now the rate of drain and
recharge are identical.

Unit: seconds
*/
#define TRAIL_RECHARGE          5.0f
/*
The interval of time between each created flame while the trail trail is
activated.

@TODO Flame interval should be based on distance, not time. In some sense, a
line is simply being laid out, of which flame billboards are uniformly
distributed across, meanining that the spacing is time invariant.

Unit: seconds
*/
#define FLAME_INTERVAL          0.075f
/*
Delay time when the trail is deactivate and when the gauge begins to recharge.
This makes spam toggling less effective.

@TODO this is potentially not behaving in seconds, like the charge
drain/recharge. Difficult to tell without visual indicators.

Unit: seconds
*/
#define TRAIL_RECHARGE_COOLDOWN 0.1f

// Fire Defines
#define FIRE_HEIGHT             2.0
#define FIRE_WIDTH              2.0

// Camera Defines
#define FRONT_CAMERA            0
#define BACK_CAMERA             1

#define FRONT_CAMERA_LONGITUDE  -90.0f  // theta    degrees
#define FRONT_CAMERA_LATITUDE   80.0f   // phi      degrees
#define FRONT_RADIUS            10.0f   // r        meters

#define BACK_CAMERA_LONGITUDE   -90.0f  // theta    degrees
#define BACK_CAMERA_LATITUDE    40.0f   // phi      degrees
#define BACK_RADIUS             12.0f   // r        meters

/*
Determines the rate at the which the camera moves to catch up to player
movement. The greater it is, the faster the camera moves (the less it lags
behind).
*/
#define CAMERA_MOVEMENT_MULTIPLIER 150.0f
/*
Determines the rate at which the camera rotates to catch up be behind the
player. The greater it is, the faster the camera rotates (the less it lags
behind).
*/
#define CAMERA_ROTATION_MULTIPLIER 30.0f


const vec3 FRONT_CAMERA_START_VIEW = vec3(FRONT_CAMERA_LONGITUDE, FRONT_CAMERA_LATITUDE, FRONT_RADIUS); // (Theta, Phi, Radius)
const vec3 BACK_CAMERA_START_VIEW = vec3(BACK_CAMERA_LONGITUDE, BACK_CAMERA_LATITUDE, BACK_RADIUS); // (Theta, Phi, Radius)
/*
The position of the camera relative to the position of the player. Both vectors
will be added together to form the final camera position.
*/
const vec3 FRONT_CAMERA_POSITION_OFFSET = vec3(-0, 0, 0);
const vec3 BACK_CAMERA_POSITION_OFFSET = vec3(-10, 0, 0);

class HovercraftEntity :
    public Entity
{
public:
    HovercraftEntity(int iID, const vec3* vPosition, eEntityTypes entityType);
    virtual ~HovercraftEntity();

    // Implementation of inherited functionality
    void update(float fTimeInMilliseconds);
    void getSpatialDimensions(vec3* pNegativeCorner, vec3* pPositiveCorner) const;

    void initialize(const string& sFileName,
                    const ObjectInfo* pObjectProperties,
                    const string& sShaderType,
                    float fScale);

    bool useAbility(eAbility ability);
    void move(float x, float y);
    void turn(float x);

    // TEMPORARY: Returns the directional Angle for cobbled camera controls
    vec3 getCameraPosition() { return m_vCurrentCameraPosition; }
    quat getCameraRotation() { return m_qCurrentCameraRotation; }

    const CameraComponent* getActiveCameraComponent() { return m_pCmrComponents[activeCameraIndex]; }

    // The player has a front and back camera, which can be toggled as the
    // active camera
    void setActiveCameraToBack() { activeCameraIndex = BACK_CAMERA; }
    void setActiveCameraToFront() { activeCameraIndex = FRONT_CAMERA; }
    void toggleActiveCamera() { activeCameraIndex = !activeCameraIndex; }

private:
    // Private Variables
    int activeCameraIndex;
    Mesh* m_pMesh;
    SpatialDataMap* m_pSpatialMap;
    RenderComponent* m_pRenderComponent;
    PhysicsComponent* m_pPhysicsComponent;
    CameraComponent* m_pActiveCameraComponent;
    CameraComponent* m_pCmrComponents[MAX_CAMERAS_PER_PLAYER];
    InteractableEntity* m_pFireTrail;

    /*
    These should lag behind

    m_vPostion : the desired position

    m_pPhysicsComponent->getRotation() : the desired rotation
    */
    vec3 m_vCurrentCameraPosition;
    quat m_qCurrentCameraRotation;

    // Private Functions
    void updateCameraLookAts(float fSecondsSinceLastUpdate);
    void updateCameraPosition(float fSecondsSinceLastUpdate);
    void updateCameraRotation(float fSecondsSinceLastUpdate);
    void updateCooldowns(float fSecondsSinceLastUpdate);

    // Abilities
    void shootRocket();
    void activateSpikes();

    void activateTrail();
    void deactivateTrail();
    void updateTrail(float fSecondsSinceLastUpdate);

    void dash(eAbility direction);

    // Cooldowns
    void initializeCooldowns();
    bool isOnCooldown(eAbility ability);
    float m_fCooldowns[ABILITY_COUNT];

    /*
    Tracks the state of the flame trail

    true: trail is activated and fuel gauge is draining.
    false: trail is deactivated and fuel gauge is recharging.

    Even if there is no fuel available, the trail can still be activated.
    The trail will simply not produce any flames.
    This activation state is based on player input.
    */
    bool m_bTrailActivated;
    float m_fSecondsSinceTrailDeactivated;
    bool m_bTrailReadyToRecharge;
    /*
    Stores the flame trail fuel gauge values.
    Since the gauge drain and recharge rates are in terms of time, so
    is the gauge itself.
    */
    float m_fTrailGauge;
    /*
    To ensure that the flames are generated at constant intervals, we need
    to track the time since the last flame was generated.
    */
    float m_fSecondsSinceLastFlame;
};

