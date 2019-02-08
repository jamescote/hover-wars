#pragma once
#include "stdafx.h"
#include "ShaderManager.h"

class Texture
{
private:
	// Private Copy Constructor and Assignment Operator
	Texture(const Texture& pCopyTexture);
	Texture& operator=(const Texture& pRHS);

	// OpenGL names for array buffer objects, vertex array object
	GLuint  m_TextureName;

	// dimensions of the image stored in this texture
	GLuint  m_uiWidth, m_uiHeight;

	// Reference to texture's File Name
	string m_sManagerKey;

	// Friend class is TextureManager so only this Manager can create Texture Objects
	friend class TextureManager;
	// Private Manager Cookie so only TextureManager can construct a Texture
	//	but make_unique<Texture> still has access to the constructor it needs.
	struct manager_cookie {};

public:
	Texture(const string& sFileName, manager_cookie);
	virtual ~Texture();
	
	// Public Functions
	void genTexture( const void* pBits, GLuint uiWidth, GLuint uiHeight, GLenum eFormat, GLenum eType );
	void bindTexture( ShaderManager::eShaderType eType, string sVarName );
	void unbindTexture();

	// Gets the Hash Key for the texture for referencing in the Texture Manager
	const string& getFileName() { return m_sManagerKey; }
};
