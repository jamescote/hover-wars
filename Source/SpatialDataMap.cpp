#include "SpatialDataMap.h"
#include "ShaderManager.h"

/*************\
 * Constants *
\*************/
const float ALPHA       = 0.5f;
const vec4 GRID_COLOR   = vec4(1.0f, 0.1568627450980392f, 0.0f, ALPHA);                                // Ferrari Red
const vec4 POP_COLOR    = vec4(0.4941176470588235f, 0.9764705882352941f, 1.0f, ALPHA);                 // Electric Blue
const vec4 SPOT_COLOR   = vec4(0.3764705882352941f, 0.1843137254901961f, 0.4196078431372549f, ALPHA);  // Deep Purple
const vec4 POINT_COLOR  = vec4(0.9411764705882353f, 1.0f, 0.9411764705882353f, ALPHA);                  // Azure White
float OVERLAY_HEIGHT = 0.1f;

// Default Constructor for Data Map
SpatialDataMap::SpatialDataMap()
{
    m_bIsInitialized = false;
}

// Destructor for Data Map
SpatialDataMap::~SpatialDataMap()
{
    // Reset Map
    clearMap();
}

// Clears the Map to a fresh state for reinitialization
void SpatialDataMap::clearMap()
{
    // Clear the unordered Map
    m_pEntityMap.clear();

    // Iterate through each row in the spatial Map and clear the elements.
    for (vector<vector<sSpatialCell>>::iterator iter = m_pSpatialMap.begin();
        iter != m_pSpatialMap.end();
        ++iter)
        iter->clear();

    // clear the base vector of its elements
    m_pSpatialMap.clear();

    // Delete VBOs and VAOs
    glDeleteBuffers(1, &m_iMapIndicesBuffer);
    glDeleteBuffers(1, &m_iMapVertexBuffer);
    glDeleteBuffers(1, &m_iMapInstanceBuffer);
    glDeleteBuffers(1, &m_iPopulatedIndicesBuffer);
    glDeleteVertexArrays(1, &m_iMapVertexArray);

    m_bIsInitialized = false;
}


// Initialize the Map dimensions 
void SpatialDataMap::initializeMap(float fLength, float fWidth, float fTileSize)
{
    // Local Variables
    float fHalfLength = fLength * 0.5f;
    float fHalfWidth = fWidth * 0.5f;
    m_fTileSize = fTileSize;
    m_iMaxX = static_cast<unsigned int>(floor(fWidth / fTileSize));
    m_iMaxY = static_cast<unsigned int>(floor(fLength / fTileSize));

    // Set the Origin position for reference wrt world coordinates as well as the maximum limit of the map.
    m_vOriginPos = vec2(-fHalfWidth, -fHalfLength);
    m_vMaxDimensions = vec2(fWidth, fLength) + m_vOriginPos;

    // Initialize Map Arrays
    m_pSpatialMap.resize(m_iMaxX, vector<sSpatialCell>(m_iMaxY));
    vec2 vOriginOffset = m_vOriginPos;

    // Set the relative Origin Position for each cell.
    for (unsigned int x = 0; x < m_iMaxX; ++x)
    {
        // Go through each column
        for (unsigned int y = 0; y < m_iMaxY; ++y)
        {
            // Set the Relative Position and update to the next column position.
            m_pSpatialMap[x][y].vOriginPos = vOriginOffset;
            m_pSpatialMap[x][y].iStaticSize = 0;
            vOriginOffset.y += m_fTileSize;
        }

        // Reset the Offset Counter for the loop.
        vOriginOffset.y = m_vOriginPos.y;
        vOriginOffset.x += m_fTileSize;
    }

    // Generate the VBOs for drawing the map.
    generateGridVBOs();

    // Set the Initialized Flag.
    m_bIsInitialized = true;
}

// Populate Spatial Data Map with Entities in the scene.
void SpatialDataMap::populateStaticMap(const vector<unique_ptr<Entity>>* pMasterEntityList)
{
    // Local Variables
    unsigned int iX, iY; // Indices for determining Entity Position.
    unsigned int iXIndex;
    bool bValidEntity = true;
    vec3 vPosition;

    for (vector<unique_ptr<Entity>>::const_iterator iter = pMasterEntityList->begin();
        iter != pMasterEntityList->end();
        ++iter)
    {
        // Get Entity's World Position
        vPosition = (*iter)->getPosition();

        // Add Entity to Spatial Map
        if (getMapIndices(&vPosition, &iX, &iY)) // Verify the Indices received are valid.
        {
            switch ((*iter)->getType())
            {
            case INTERACTABLE_ENTITY:
                m_pSpatialMap[iX][iY].pLocalInteractableEntities.push_back(static_cast<InteractableEntity*>(iter->get()));  // Push the Interactable Entity into the spatial map.
                break;
            case POINT_LIGHT_ENTITY:
                m_pSpatialMap[iX][iY].pLocalPointLights.push_back(static_cast<PointLight*>(iter->get()));                   // Push the Static Entity into the spatial map.
                break;
            case SPOT_LIGHT_ENTITY:
                m_pSpatialMap[iX][iY].pLocalSpotLights.push_back(static_cast<SpotLight*>(iter->get()));                     // Push the Static Entity into the spatial map.
                break;
            case STATIC_ENTITY:
                m_pSpatialMap[iX][iY].pLocalEntities.push_back(static_cast<StaticEntity*>(iter->get()));                    // Push the Static Entity into the spatial map.
                break;
            default:
                bValidEntity = false;
                break;
            }

            if (bValidEntity)
            {
                m_pEntityMap.insert(make_pair((*iter)->getID(), make_pair(iX, iY)));    // Store that the Entity Resides inside the specified cell at iX, iY
                m_pSpatialMap[iX][iY].iStaticSize++;                                    // Track Static size of the square.

                // Add Indices for a populated square if this is the first Entity added to the list.
                if (1 == m_pSpatialMap[iX][iY].iStaticSize )
                {
                    // Add Indices for coloring the populated square
                    /*
                        1--3
                        | /|
                        |/ |
                        2--4
                    */
                    iXIndex = iX * (m_iMaxX + 1);
                    m_pPopulatedIndices.push_back(iXIndex + iY);                // 1
                    m_pPopulatedIndices.push_back(iXIndex + iY + 1);            // 3
                    m_pPopulatedIndices.push_back(iXIndex + m_iMaxX + iY + 1);  // 2
                    m_pPopulatedIndices.push_back(iXIndex + m_iMaxX + iY + 2);  // 4

                    // Store reference for the square to determine color.
                    m_pPopulatedSquareReference.push_back(make_pair(iX, iY));
                }
            }

            // Reset Entity Flag for next loop
            bValidEntity = true;
        }
    }

    // Add Populated Indices list to the GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iPopulatedIndicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_pPopulatedIndices.size() * sizeof(unsigned int), m_pPopulatedIndices.data(), GL_STATIC_DRAW);
}

// Draw the Data Map for Debugging
void SpatialDataMap::drawMap()
{
    if (m_bIsInitialized)
    {
        // Bind Vertex Array and set Program
        glBindVertexArray(m_iMapVertexArray);
        glUseProgram(SHADER_MANAGER->getProgram(ShaderManager::eShaderType::DEBUG_SHDR));

        // Set the color for the Spacial Map Outline
        SHADER_MANAGER->setUniformVec4(ShaderManager::eShaderType::DEBUG_SHDR, "vColor", &GRID_COLOR);

        // Draw the Map
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iMapIndicesBuffer);
        glDrawElementsInstanced(GL_LINES, m_pGridIndices.size(), GL_UNSIGNED_INT, 0, 1);

        // Draw the Populated Squares
        vec4 const*vColor = &POP_COLOR;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iPopulatedIndicesBuffer);
        for (unsigned int i = 0; i < m_pPopulatedSquareReference.size(); ++i)
        {
            if (!m_pSpatialMap[m_pPopulatedSquareReference[i].first][m_pPopulatedSquareReference[i].second].pLocalEntities.empty())
                vColor = &GRID_COLOR;
            else if (!m_pSpatialMap[m_pPopulatedSquareReference[i].first][m_pPopulatedSquareReference[i].second].pLocalPointLights.empty())
                vColor = &POINT_COLOR;
            else if (!m_pSpatialMap[m_pPopulatedSquareReference[i].first][m_pPopulatedSquareReference[i].second].pLocalSpotLights.empty())
                vColor = &SPOT_COLOR;

            SHADER_MANAGER->setUniformVec4(ShaderManager::eShaderType::DEBUG_SHDR, "vColor", vColor);
            glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void*)((i << 2) * sizeof(unsigned int)), 1);
            vColor = &POP_COLOR;
        }

    }
}

/*********************************************************************************\
* Private Functions                                                              *
\*********************************************************************************/

// Returns the Map Indices from a given Position.
bool SpatialDataMap::getMapIndices(const vec3* vPosition, unsigned int* iX, unsigned int* iY)
{
    // Get Vector from Map Origin to the Position.
    vec2 vVectorToPosition = vec2(vPosition->x, vPosition->z) - m_vOriginPos;

    // Compute Indices
    *iX = static_cast<unsigned int>(floor(vVectorToPosition.x / m_fTileSize));
    *iY = static_cast<unsigned int>(floor(vVectorToPosition.y / m_fTileSize));

    return (*iX < m_iMaxX && *iY < m_iMaxY); // Tell the caller whether the data is valid or not (Is it out of range?)
}

void SpatialDataMap::generateGridVBOs()
{
    // Local Variables
    unsigned int iCurrXIndex, iCurrIndex;

    // Pre-compute sizes of the arrays for Vertices and Indices
    m_pVertices.reserve((m_iMaxX + 1) * (m_iMaxY + 1));
    m_pGridIndices.reserve((m_iMaxX * (m_iMaxY + 1)) * 4);

    // Iterate through all possible vertices and populate the vertices and indices for rendering.
    for (unsigned int x = 0; x <= m_iMaxX; ++x)
    {
        iCurrXIndex = x * (m_iMaxX + 1);    // Set current Row Index;

        // Evaluate Row
        for (unsigned int y = 0; y <= m_iMaxY; ++y)
        {
            // Push back Vertex for this point.
            m_pVertices.push_back(vec3(m_vOriginPos.x + (x * m_fTileSize),
                OVERLAY_HEIGHT,
                m_vOriginPos.y + (y * m_fTileSize)));

            // Store Current Index
            iCurrIndex = iCurrXIndex + y;

            // Line from current row to next row.
            if (m_iMaxX != x)
            {
                m_pGridIndices.push_back(iCurrIndex);
                m_pGridIndices.push_back(m_pGridIndices.back() + m_iMaxX + 1);
            }

            // Line along this row.
            if (m_iMaxY != y)
            {
                m_pGridIndices.push_back(iCurrIndex);
                m_pGridIndices.push_back(m_pGridIndices.back() + 1);
            }
        }
    }

    // Generate Vertex Array
    glGenVertexArrays(1, &m_iMapVertexArray);

    // Generate Vertex Buffer
    m_iMapVertexBuffer = SHADER_MANAGER->genVertexBuffer(m_iMapVertexArray, m_pVertices.data(), m_pVertices.size() * sizeof(vec3), GL_STATIC_DRAW);
    SHADER_MANAGER->setAttrib(m_iMapVertexArray, 0, 3, sizeof(vec3), (void*)0);

    // Generate Indices Buffer
    m_iMapIndicesBuffer = SHADER_MANAGER->genIndicesBuffer(m_iMapVertexArray, m_pGridIndices.data(), m_pGridIndices.size() * sizeof(unsigned int), GL_STATIC_DRAW);
    m_iPopulatedIndicesBuffer = SHADER_MANAGER->genIndicesBuffer(m_iMapVertexArray, (void*)0, 0, GL_STATIC_DRAW);

    // Generate Instance Buffer for Rendering in Debug Shader
    mat4 m4TransformationMatrix = mat4(1.0f);
    m_iMapInstanceBuffer = SHADER_MANAGER->genInstanceBuffer(m_iMapVertexArray, 1, &m4TransformationMatrix, sizeof(mat4), GL_STATIC_DRAW);
}
