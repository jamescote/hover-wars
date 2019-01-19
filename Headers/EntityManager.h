#pragma once
#include "stdafx.h"
#include "Light.h"
#include "BoidEngine.h"
#include "Entity.h"
#include "Camera.h"
#include "EntityComponentHeaders/EntityComponent.h"
#include "EntityComponentHeaders/CameraComponent.h"
#include "EntityComponentHeaders/RenderComponent.h"
#include "EntityComponentHeaders/LightingComponent.h"


// Environment Manager
// Manages all 3D objects in an environment
// Written by: James Cot�
class EntityManager
{
public:
	static EntityManager* getInstance();
	~EntityManager();

	void initializeEnvironment(string sFileName);
	void pause() { m_bPause = !m_bPause; }

	// Entity Functions
	Camera* generateCameraEntity();
	void generateStaticPlane(int iHeight, int iWidth, vec3 vPosition, vec3 vNormal, const string& sTextureLocation = "", const string& sShaderType = "");
	void generateStaticSphere(float fRadius, vec3 vPosition, const string& sTextureLocation = "", const string& sShaderType = "");
	void generateStaticMesh(const string& sMeshLocation, vec3 vPosition, const string& sTextureLocation = "", const string& sShaderType = "" );
	void generateStaticLight( vec3 vPosition, vec3 vColor, const string& sMeshLocation = "",const string& sTextureLocation = "");
	void generatePlayerEntity(vec3 vPosition, const string& sMeshLocation, const string& sTextureLocation = "", const string& sShaderType = "");
	vec3 getEntityPosition(int iEntityID);

	// Entity Component functions
	CameraComponent* generateCameraComponent(int iEntityID);
	RenderComponent* generateRenderComponent(int iEntityID, bool bStaticDraw, ShaderManager::eShaderType eType, GLenum eMode);
	LightingComponent* generateLightingComponent(int iEntityID);

	// Camera Management
	void updateHxW(int iHeight, int iWidth);
	const CameraComponent* getActiveCamera() { return m_pActiveCamera; }

	// Clears the Environment so a new one can be loaded.
	void purgeEnvironment();
	void renderEnvironment( const vec3& vCamLookAt );
	void updateEnvironment(const Time& pTimer);

	// Light Manipulation - TEMPORARY, Needs to be replaced
	void moveLight(vec3 pMoveVec)
	{
		if (nullptr != m_pTestingLight)
			m_pTestingLight->move(pMoveVec);
	}

	// Edge Threshold Getters/Setters
	void setMinThreshold( float fMin ) { m_fMinEdgeThreshold = fMin; }
	float getMinThreshold() { return m_fMinEdgeThreshold; }
	void setMaxThreshold( float fMax ) { m_fMaxEdgeThreshold = fMax; }
	float getMaxThreshold() { return m_fMaxEdgeThreshold; }

	// Boid Methods
	void initializeBoidEngine(vector< string >& sData);

private:
	EntityManager();
	EntityManager(const EntityManager* pCopy);
	static EntityManager* m_pInstance;

	// Object Managing
	BoidEngine* m_pBoidEngine;

	// Entity Managing
	int m_iEntityIDPool, m_iComponentIDPool;
	int m_iHeight, m_iWidth;
	inline int getNewEntityID() { return ++m_iEntityIDPool; }
	inline int getNewComponentID() { return ++m_iComponentIDPool; }
	vector<unique_ptr<EntityComponent>>	m_pMasterComponentList;
	vector<unique_ptr<Entity>>	m_pMasterEntityList;
	vector<RenderComponent*>	m_pRenderingComponents;
	vector<CameraComponent*>	m_pCameraComponents;
	vector<LightingComponent*>	m_pLights;
	CameraComponent*			m_pActiveCamera;
	Light*						m_pTestingLight; // Temporary, Needs to be removed.

	// Manage Pointers for Deletion.
	MeshManager*				m_pMshMngr;
	TextureManager*				m_pTxtMngr;

	// Edge Threshold Implementation
	float m_fMinEdgeThreshold, m_fMaxEdgeThreshold;
	bool m_bPause;
};

