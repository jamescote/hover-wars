#include "MeshManager.h"

MeshManager* MeshManager::pInstance = nullptr;

MeshManager::MeshManager()
{
	// Nothing to Construct
}

MeshManager* MeshManager::getInstance()
{
	if ( nullptr == pInstance )
		pInstance = new MeshManager();

	return pInstance;
}


// Destructor: Clear the Mesh Cache and release the memory.
MeshManager::~MeshManager()
{
	unloadAllMeshes();
}

// unloads all current Meshes to create a clean slate.
//		Since the meshes are unique pointers, calling clear on the container will properly
//			call the destructors of the meshes.
void MeshManager::unloadAllMeshes()
{
	m_pMeshCache.clear();
}

// loadMeshFromFile:	Takes in a FileName. Load in a mesh from that filename if possible.
// Return:				Returns a pointer to the desired mesh from the specified file.
// Parameters:			sFileName - The location of the file to load.
// Written by:			James Cote
Mesh* MeshManager::loadMeshFromFile( const string& sFileName )
{
	// Attempt to grab it from the texture cache if it already exists
	Mesh* pReturnMesh = nullptr;

	// Found an existing Mesh from that file.
	if ( m_pMeshCache.end() != m_pMeshCache.find( sFileName ) )
	{
		// Grab the Mesh from the Cache
		pReturnMesh = m_pMeshCache[ sFileName ].get();
	}
	else // Create the New Texture in the Texture Cache, attach the User to the Texture and return the newly created texture.
	{
		// Generate Mesh smart pointer
		unique_ptr<Mesh> pNewMesh = make_unique<Mesh>( sFileName, Mesh::manager_cookie() );

		if ( !initializeMesh( pNewMesh.get(), sFileName ) )
		{
			if ( sFileName != "" )
				cout << "Error, unable to load texture: " << sFileName << endl;
			pNewMesh.reset();
		}
		else
		{
			// Return Newly Created Mesh.
			pReturnMesh = pNewMesh.get();
			// Attach Mesh to the Cache
			m_pMeshCache.insert(make_pair( sFileName, move(pNewMesh) ));
		}
	}

	return pReturnMesh;
}

// genereatePlaneMesh:	Searches the Cache for a similar mesh with handle "Plane<iHeight><iWidth>"
//							or creates a new Plane Mesh if one hasn't been created yet.
// Returns:				Generated plane mesh or nullptr if no mesh was able to be generated.
// Written by:			James Cote
Mesh* MeshManager::generatePlaneMesh(int iHeight, int iWidth, 
									 vec3 vPosition, vec3 vNormal)
{
	// Local Variables
	string sHashHandle = "Plane" + to_string(iHeight) + to_string(iWidth) + 
							glm::to_string(vPosition) + glm::to_string(vNormal);
	Mesh* pReturnMesh = nullptr;

	// Found a plane of this size that already exists, return that.
	if (m_pMeshCache.end() != m_pMeshCache.find(sHashHandle))
	{
		pReturnMesh = m_pMeshCache[sHashHandle].get();
	}
	else // Generate a new Plane Mesh of height iHeight and width iWidth
	{
		unique_ptr<Mesh> pNewPlane = make_unique<Mesh>(sHashHandle, Mesh::manager_cookie());
		pNewPlane->genPlane(iHeight, iWidth, vPosition, vNormal);	// Generate Pane
		pReturnMesh = pNewPlane.get();	// Return raw pointer to managed Mesh.
		m_pMeshCache.insert(make_pair(sHashHandle, move(pNewPlane)));	// Insert into Mesh Cache
	}

	return pReturnMesh;
}

// Attempts to Initialize and return a new mesh object from a given object file.
// Returns: Mesh Object created or nullptr if mesh failed to create.
//			Bool: Returns true on Success, False on failure.
bool MeshManager::initializeMesh( Mesh* pReturnMesh, const string& sFileName )
{
	// Return Value
	bool bReturnValue = ( nullptr != pReturnMesh );

	if ( bReturnValue )
		bReturnValue = pReturnMesh->genMesh( sFileName );

	// Return result.
	return bReturnValue;
}