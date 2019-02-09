#include "../include/ResourceManager.h"
#include "../include/GraphicsMicro.h"
#include "../include/GraphicsConfig.h"
#include "../include/Resource.h"
#include <assert.h>

using namespace Cooler;

IResourceManager::IResourceManager()
{
}

IResourceManager::~IResourceManager()
{
	for (auto resource : m_Key2ResourceMap)
		_SAFE_DELETE(resource.second);
}

//***********************************************************
//FUNCTION:
Cooler::IResource* Cooler::IResourceManager::createResource(const CGraphicsConfig* vConfig)
{
	assert(vConfig);

	std::string ResouceKey = vConfig->getType();
	assert(glGetError() == GL_NO_ERROR);
	IResource* pResouce = __createResouceV(vConfig);
	assert(pResouce);
	m_Key2ResourceMap[vConfig->getName()] = pResouce;
//	pResouce->_onResourceCreatedV(vConfig);

	return pResouce;
}

//***********************************************************
//FUNCTION::
Cooler::IResource* Cooler::IResourceManager::fetchResource(const std::string& vResourceKey)
{
	assert(!vResourceKey.empty());
	if (m_Key2ResourceMap.find(vResourceKey) != m_Key2ResourceMap.end())
		return m_Key2ResourceMap[vResourceKey];
	else
		return nullptr;
}