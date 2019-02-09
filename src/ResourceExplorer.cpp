#include "../include/ResourceExplorer.h"
#include "../include/GraphicsCommon.h"
#include "../include/GraphicsConfig.h"
#include "../include/GraphicsMicro.h"
#include "../include/TextureManager.h"
#include "../include/CameraManager.h"
#include "../include/ModelManager.h"
#include "../include/FBOManager.h"
#include <assert.h>

using namespace Cooler;

CResourceExplorer::CResourceExplorer()
{
}

CResourceExplorer::~CResourceExplorer()
{
	for (auto resourceManager : m_ResourceManagerSet)
		_SAFE_DELETE(resourceManager.second);
}

//***********************************************************
//FUNCTION:
void Cooler::CResourceExplorer::createAllResourceManager()
{
	m_ResourceManagerSet[CONFIG_KEYWORD::TEXTURE] = new CTextureManager;
	m_ResourceManagerSet[CONFIG_KEYWORD::FRAMEBUFFER] = new CFBOManager;
	m_ResourceManagerSet[CONFIG_KEYWORD::MODEL] = new CModelManager;
	m_ResourceManagerSet[CONFIG_KEYWORD::CAMERA] = new CCameraManager;
}

//***********************************************************
//FUNCTION:
void Cooler::CResourceExplorer::createResource(const CGraphicsConfig* vConfig) const
{
	assert(vConfig);
	
	for (unsigned int i = 0; i < vConfig->getSubConfigNum(); i++)
	{
		CGraphicsConfig* pSubConfig = vConfig->fetchSubConfigAt(i);
		assert(pSubConfig);
		auto Itr = m_ResourceManagerSet.find(pSubConfig->getType());
		if (Itr != m_ResourceManagerSet.end())
		{
			assert(glGetError() == GL_NO_ERROR);
			Itr->second->createResource(pSubConfig);
		}
	}
}

//***********************************************************
//FUNCTION:
Cooler::IResourceManager* Cooler::CResourceExplorer::findResourceManager(const std::string& vResourceType)
{
	return (m_ResourceManagerSet.count(vResourceType) == 0) ? nullptr : m_ResourceManagerSet[vResourceType];
}
