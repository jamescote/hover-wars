#pragma once
#include "stdafx.h"
#include "Entity.h"
#include "MeshManager.h"
#include "EntityComponentHeaders/LightingComponent.h"
#include "EntityComponentHeaders/RenderComponent.h"

// Provides an Interface for a Directional Light Component.
//	the Directional Light simulates a light that is being emitted from a source that is infinitely far away.
//	Possible modifications might be:
//		Simulating a Day/Night change by modifying direction, and color values.
// Written by: James Cote
class SpotLight :
	 public Entity
{
public:
	SpotLight(int iID, const vec3* vPosition);
	virtual ~SpotLight();

	// Function to Initialize the Light.
	void initialize(float fPhi, float fSoftPhi, bool bStatic, const vec3* vColor, const vec3* vDirection, const string& sMeshLocation, const Material* sMaterial = nullptr);

	const LightingComponent* getLightingComponent() const { return m_pLightingComponent; }

private:
	// Private Light Copy constructor and assignment operator
	SpotLight( const SpotLight* newLight );
	SpotLight& operator=(const SpotLight& pCopy);

	// Private Variables
	vec3 m_pColor;
	Mesh* m_pMesh;
	RenderComponent* m_pRenderComponent;
	LightingComponent* m_pLightingComponent;

};
