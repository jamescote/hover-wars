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
// Camera Defines
#define MAX_CAMERAS_PER_PLAYER  2 
#define FRONT_CAMERA            0
#define BACK_CAMERA             1

/*
Cooldowns

The time the hovercraft must wait until they can use the ability again.

Units: seconds
*/
#define ROCKET_COOLDOWN         2.0f
#define SPIKES_COOLDOWN         2.0f
#define TRAIL_COOLDOWN          0.0f
#define DASH_COOLDOWN           2.0f

/*
Once spikes are activated, they are enabled for a duration before deactivating.
*/
#define SPIKES_DURATION         3.0f // 1.0f
/*
Total time the trail can be activated from full to empty.

Unit: seconds
*/
#define TRAIL_GAUGE_FULL        3.0f
/*
Represents the trail gauge is empty.
*/
#define TRAIL_GAUGE_EMPTY       0.0f

/*
Time multiplier for the trail to recharge from empty to full.

= 1: recharge rate is the same as drain rate.
> 1: recharge rate is faster than drain rate
< 1: recharge rate is slower than drain rate

*/
#define TRAIL_RECHARGE_MULTIPLIER 0.5f

/*
The interval of time between each created flame while the trail trail is
activated.

@TODO Flame interval should be based on distance, not time. In some sense, a
line is simply being laid out, of which flame billboards are uniformly
distributed across, meanining that the spacing is time invariant.

Unit: seconds
*/
#define FLAME_INTERVAL          0.10f

/*
Delay time when the trail is deactivate and when the gauge begins to recharge.
This makes spam toggling less effective.

Unit: seconds
*/
#define TRAIL_RECHARGE_COOLDOWN 0.5f

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

    // Get ability statuses for UI
    /*
    Get the status of the flame in percent.
    @return 1.0f if full, 0.0f if empty, or intermediate value if in between
    */
    float getTrailGaugePercent() { return m_fTrailGauge / TRAIL_GAUGE_FULL; };

    /*
    Get all the cooldowns to be used by the UI.
    // NOTE: why not send m_fCooldowns directly (make public)?
    @return an array of all ability cooldowns.
    */
    float* getCooldowns() { return m_fCooldowns; };

    void setLoseControl(float seconds) { outOfControlTime = seconds; isInControl = false; };

    bool hasSpikesActivated() { return m_bSpikesActivated; };
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
    void updateSpikes(float fSecondsSinceLastUpdate);

    void activateTrail();
    void deactivateTrail();
    void updateTrail(float fSecondsSinceLastUpdate);
    void createTrailInstance();

    void dash(eAbility direction);

    // Cooldowns
    void initializeCooldowns();
    bool isOnCooldown(eAbility ability);
    float m_fCooldowns[COOLDOWN_COUNT];

    bool m_bSpikesActivated;
    float m_fSecondsSinceSpikesActivated;

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

    At maximum, this is TRAIL_GAUGE_FULL, at minimum, it is TRAIL_GAUGE_EMPTY.
    */
    float m_fTrailGauge;
    /*
    To ensure that the flames are generated at constant intervals, we need
    to track the time since the last flame was generated.
    */
    float m_fSecondsSinceLastFlame;
    /*
    To keep the distance between flame trail flames consistent at different
    velocities we must track the position of the last flame so we know when to
    place the next one.
    */
    vec3 m_vPositionOfLastFlame;

    float m_fMinimumDistanceBetweenFlames;

    /*
    If true, car is able to receive and act upon movement input.
    Else, not movement input is processed.
    */
    bool isInControl;
    float outOfControlTime;
};

