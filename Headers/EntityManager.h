#pragma once
#include "stdafx.h"
#include "EntityHeaders/PointLight.h"
#include "EntityHeaders/DirectionalLight.h"
#include "EntityHeaders/SpotLight.h"
#include "SceneLoader.h"
#include "EmitterEngine.h"
#include "EntityHeaders/InteractableEntity.h"
#include "EntityHeaders/Entity.h"
#include "EntityHeaders/PlayerEntity.h"
#include "EntityHeaders/Camera.h"
#include "EntityComponentHeaders/EntityComponent.h"
#include "EntityComponentHeaders/CameraComponent.h"
#include "EntityComponentHeaders/RenderComponent.h"
#include "EntityComponentHeaders/LightingComponent.h"
#include "EntityComponentHeaders/PhysicsComponent.h"
#include "EntityComponentHeaders/AnimationComponent.h"
#include "Physics/PhysicsManager.h"
#include "SpatialDataMap.h"

// Environment Manager
// Manages all objects in an environment
// Written by: James Coté, Evan Quan
class EntityManager
{
public:
    // Singleton instance retrieval and Destructor
    static EntityManager* getInstance();
    ~EntityManager();

    // Init and Pause Functionality
    void initializeEnvironment(string sFileName);

    // Scene Management Functionality
    void pause() { m_bPause = !m_bPause; }
    void toggleBBDrawing() { m_bDrawBoundingBoxes = !m_bDrawBoundingBoxes; }
    void toggleSpatialMapDrawing() { m_bDrawSpatialMap = !m_bDrawSpatialMap; }
    bool doBoundingBoxDrawing() { return m_bDrawBoundingBoxes; }
    void initializeSpatialMap(float fLength, float fWidth, float fTileSize);

    // Entity Functions
    Camera* generateCameraEntity();
    void generateStaticPlane(const ObjectInfo* pObjectProperties, int iHeight, int iWidth, const vec3* vNormal, const string& sShaderType = "");
    void generateStaticSphere(const ObjectInfo* pObjectProperties, float fRadius, const string& sShaderType = "");
    void generateStaticMesh(const ObjectInfo* pObjectProperties, const string& sMeshLocation, float fScale, const string& sShaderType = "" );
    void generateStaticPointLight(const ObjectInfo* pObjectProperties, float fPower, const vec3* vColor, const string& sMeshLocation = "", float m_fMeshScale = 1.0);
    void generateDirectionalLight( const vec3* vDirection, const vec3* vAmbientColor, const vec3* vDiffuseColor, const vec3* vSpecularColor );
    void generateStaticSpotLight(const ObjectInfo* pObjectProperties, float fPhi, float fSoftPhi, const vec3* vColor, const vec3* vDirection, const string& sMeshLocation = "", float m_fMeshScale = 1.0);
    void generatePlayerEntity(const ObjectInfo* pObjectProperties, const string& sMeshLocation, float fScale, const string& sShaderType = "");
    InteractableEntity* generateInteractableEntity(const vec3* vPosition);
    vec3 getEntityPosition(int iEntityID);

    // Entity Component functions
    CameraComponent* generateCameraComponent(int iEntityID);
    RenderComponent* generateRenderComponent(int iEntityID, Mesh const* pMeshKey, bool bStaticDraw, ShaderManager::eShaderType eType, GLenum eMode);
    LightingComponent* generateLightingComponent(int iEntityID);
    PhysicsComponent* generatePhysicsComponent(int iEntityID);
    AnimationComponent* generateAnimationComponent(int iEntityID);

    // Camera Management
    void updateHxW(int iHeight, int iWidth);

    // The GameManager instead retrieves the camera components from the player
    // entities to set as its active cameras
    const CameraComponent* getActiveCameraComponent() { return m_pActiveCameraComponent; }

    // Clears the Environment so a new one can be loaded.
    void purgeEnvironment();
    void renderEnvironment( );
    void updateEnvironment(const Time& pTimer);
    
    /*
    The command handler can get all the players to directly communicate to.
    */
    PlayerEntity* getPlayer(ePlayer player);
    bool playerExists(ePlayer);

private:
    EntityManager();
    EntityManager(const EntityManager* pCopy);
    static EntityManager* m_pInstance;

    // Entity Managing
    int m_iEntityIDPool, m_iComponentIDPool;
    int m_iHeight, m_iWidth;
    inline int getNewEntityID() { return ++m_iEntityIDPool; }
    inline int getNewComponentID() { return ++m_iComponentIDPool; }
    vector<PhysicsComponent*>                       m_pPhysicsComponents;   // PHYSICSTODO: If this isn't necessary, remove it.
    vector<unique_ptr<EntityComponent>>             m_pMasterComponentList;
    vector<PlayerEntity*>                           m_pPlayerEntityList;    
    vector<unique_ptr<Entity>>                      m_pMasterEntityList;
    unordered_map<Mesh const*, RenderComponent*>    m_pRenderingComponents;
    vector<CameraComponent*>                        m_pCameraComponents;
    vector<LightingComponent*>                      m_pLights;
    CameraComponent*                                m_pActiveCameraComponent;
    vector<AnimationComponent*>                     m_pAnimationComponents;
    DirectionalLight*                               m_pDirectionalLight;
    InteractableEntity*                             m_pBillboardTesting;

    // Manage Pointers for Deletion.
    MeshManager*                m_pMshMngr;
    TextureManager*             m_pTxtMngr;
    SceneLoader*                m_pScnLdr;
    EmitterEngine*              m_pEmtrEngn;
    PhysicsManager*             m_pPhysxMngr;
    SpatialDataMap*             m_pSpatialMap;   
    
    

    // Scene Management toggling
    bool m_bPause, m_bDrawBoundingBoxes, m_bDrawSpatialMap;
};

