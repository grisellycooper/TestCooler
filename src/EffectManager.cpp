#include "../include/EffectManager.h"
#include "../include/GraphicsCommon.h"
#include "../include/GraphicsConfig.h"
#include "../include/ProductFactory.h"
#include "../include/Effect.h"
#include <assert.h>

using namespace Cooler;
CEffectManager::CEffectManager()
{
}

CEffectManager::~CEffectManager()
{

}

//***********************************************************
//FUNCTION::
bool Cooler::CEffectManager::createAllEffect(const CGraphicsConfig* vEffectConfig, std::map<std::string, CEffect*>& voEffectMap)
{
	assert(vEffectConfig);
	for (unsigned int i = 0; i < vEffectConfig->getSubConfigNum(); i++)
	{
		CGraphicsConfig* pConfig = vEffectConfig->fetchSubConfigAt(i);
		std::string EffectSig = pConfig->getName();
		CEffect* pEffect = dynamic_cast<CEffect*>(CProductFactoryData::getInstance()->createProduct(EffectSig));
		assert(pEffect);
		if (!pEffect->initEffectWithConfig(pConfig)) continue;
		voEffectMap.insert(std::make_pair(EffectSig, pEffect));
	}

	return true;
}
