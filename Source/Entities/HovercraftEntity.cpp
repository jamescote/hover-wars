#include "EntityHeaders/HovercraftEntity.h"
#include "MeshManager.h"
#include "EntityManager.h"

HovercraftEntity::HovercraftEntity(int iID, const vec3* vPosition, eEntityTypes entityType)
    : Entity(iID, *vPosition, entityType)
{
    m_pSpatialMap = SPATIAL_DATA_MAP;
    activeCameraIndex = FRONT_CAMERA;
    m_vPositionTotal = *vPosition * PAST_CAMERA_POSITIONS;
    for (unsigned int i = 0; i < PAST_CAMERA_POSITIONS; ++i)
    {
        m_vPastPositions.push(*vPosition);
    }

    initializeCameraLookAts();
}

HovercraftEntity::~HovercraftEntity()
{

}

/****************************************************************\
 * Inherited Pure Virtual Functions                             *
\****************************************************************/

void HovercraftEntity::update(float fTimeInMilliseconds)
{
    // New Transformation Matrix
    mat4 m4NewTransform = mat4(1.0f);

    // Get the Transformation from the Physics component
    m_pPhysicsComponent->getTransformMatrix(&m4NewTransform);

    // If there's a new Transformation, apply it to the Mesh.
    m_pMesh->addInstance(&m4NewTransform);
    m_pMesh->addBBInstance(&m4NewTransform);

    // Check to update Dynamic Position in Spatial Map
    vec3 vNewPosition = m4NewTransform[3];
    if (m_vPosition != vNewPosition)
    {
        m_pSpatialMap->updateDynamicPosition(this, &vNewPosition);
    }

    // Calculate Position Averages for Camera
    m_vPosition = vNewPosition;
    updateCameraLookAts(); // TODO: Need to interpolate positions a bit better.
}

// Fetches the Spatial Dimensions of the Mesh/Bounding Box if applicable.
void HovercraftEntity::getSpatialDimensions(vec3* pNegativeCorner, vec3* pPositiveCorner) const
{
    m_pMesh->getSpatialDimensions(pNegativeCorner, pPositiveCorner);
}

// Initializes Entity information
void HovercraftEntity::initialize(const string& sFileName,
                                  const ObjectInfo* pObjectProperties,
                                  const string& sShaderType,
                                  float fScale)
{
    // Load Mesh and Rendering Component
    m_pMesh = MESH_MANAGER->loadMeshFromFile(sFileName, pObjectProperties, fScale);
    m_pRenderComponent = ENTITY_MANAGER->generateRenderComponent(m_iID, m_pMesh, false, SHADER_MANAGER->getShaderType(sShaderType), GL_TRIANGLES);

    // PHYSICSTODO: Set up Physics Component as a Dynamic Physics Object for a player
    m_pPhysicsComponent = ENTITY_MANAGER->generatePhysicsComponent(m_iID);
    m_pPhysicsComponent->initializeComponent(true, m_pMesh);

    m_pFireTrail = ENTITY_MANAGER->generateInteractableEntity(&m_vPosition);
    m_pFireTrail->loadAsBillboard(FIRE_HEIGHT, FIRE_WIDTH);
    
    // Generate Camera Components
    for (unsigned int i = 0; i < MAX_CAMERAS_PER_PLAYER; ++i)
    {
        m_pCmrComponents[i] = ENTITY_MANAGER->generateCameraComponent(m_iID);
        m_pCmrComponents[i]->setLookAt(m_vPosition);
    }
    
    m_pCmrComponents[FRONT_CAMERA]->setSphericalPos(FRONT_CAMERA_START_VIEW);
    m_pCmrComponents[BACK_CAMERA]->setSphericalPos(BACK_CAMERA_START_VIEW);
}

/********************************************************************************************************\
 * Private Functions                                                                                    *
\********************************************************************************************************/

/*
For the camera to track only the horizontal rolling average, while maintaining the same
vertical angle, it must record and use the initial horizontal 
*/
void HovercraftEntity::initializeCameraLookAts()
{
    // TODO
}
/*
Updates an average for this player's cameras. This is what makes the camera
sway as the player moves.
*/
void HovercraftEntity::updateCameraLookAts()
{
    // Queue new position and add to total
    m_vPastPositions.push(m_vPosition);
    m_vPositionTotal += m_vPosition;

    // Keep Queue within limits of Average
    if (m_vPastPositions.size() > PAST_CAMERA_POSITIONS)
    {
        m_vPositionTotal -= m_vPastPositions.front();
        m_vPastPositions.pop();
    }

    // Calculate Average Position and set new look at for Camera Components
    vec3 vAveragePosition = m_vPositionTotal * AVERAGE_POSITION_MULTIPLIER;

    // Iterpolate between current position and average position to prevent
    // rough camera changes as the average changes
    // TODO This seems to make things smoother. Will need more testing once physics rumbling is solved.
    vAveragePosition = (vAveragePosition + m_vPosition) * 0.5;

    // Update all the camera look at and rotation values based on the averaging calculations.
    quat rotation = m_pPhysicsComponent->getRotation();
    m_pCmrComponents[FRONT_CAMERA]->setLookAt(vAveragePosition + rotation * FRONT_CAMERA_POSITION_OFFSET);
    m_pCmrComponents[FRONT_CAMERA]->setRotationQuat(rotation);
    m_pCmrComponents[BACK_CAMERA]->setLookAt(vAveragePosition + rotation * BACK_CAMERA_POSITION_OFFSET);
    m_pCmrComponents[BACK_CAMERA]->setRotationQuat(rotation);
}

void HovercraftEntity::useAbility(eAbility ability)
{
    switch (ability)
    {
    case ABILITY_ROCKET:
        shootRocket();
        break;
    case ABILITY_SPIKES:
        activateSpikes();
        break;
    case ABILITY_TRAIL:
        activateTrail();
        break;
    case ABILITY_DASH_BACK:
    case ABILITY_DASH_FORWARD:
    case ABILITY_DASH_LEFT:
    case ABILITY_DASH_RIGHT:
        dash(ability);
        break;
    }
}

void HovercraftEntity::move(float x, float y)
{
    PHYSICS_MANAGER->movePlayer(m_iID, x, y);
}

void HovercraftEntity::turn(float x)
{
    PHYSICS_MANAGER->rotatePlayer(m_iID, x);
}

void HovercraftEntity::shootRocket()
{
    EMITTER_ENGINE->generateEmitter(m_vPosition, vec3(0, 1, 0), 60.f, 5.0f, 100, false, 2.0f);
}

void HovercraftEntity::activateSpikes()
{
    GAME_STATS->addScore(PLAYER_1, GameStats::HIT_BOT);
}

void HovercraftEntity::activateTrail()
{
    mat4 m4TransformMat;
    vec3 vNormal;
    m_pPhysicsComponent->getTransformMatrix(&m4TransformMat);
    vNormal = m4TransformMat[1];
    m_pFireTrail->addBillboard(&vNormal, &m_vPosition);
}

void HovercraftEntity::dash(eAbility direction)
{
    switch (direction)
    {
    case ABILITY_DASH_BACK:
        break;
    case ABILITY_DASH_FORWARD:
        break;
    case ABILITY_DASH_LEFT:
        break;
    case ABILITY_DASH_RIGHT:
        break;
    }

}
