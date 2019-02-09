#include "../include/ModelManager.h"
#include <GL/glew.h>
#include "../include/Model.h"
#include "../include/GraphicsCommon.h"
#include "../include/GraphicsConfig.h"
#include <assert.h>

using namespace Cooler;

CModelManager::CModelManager()
{
}

CModelManager::~CModelManager()
{
}

//***********************************************************
//FUNCTION:
Cooler::IResource* Cooler::CModelManager::__createResouceV(const CGraphicsConfig* vConfig) const
{
//	_ASSERTE(glGetError() == GL_NO_ERROR);
	assert(vConfig);

	if (vConfig->getType() == CONFIG_KEYWORD::MODEL)
	{
		CModel* pModel = new CModel;
		pModel->setModelSig(vConfig->getName());
		pModel->loadModelFile(DEFAULT_CONFIG::MODEL_PATH + vConfig->getAttribute<std::string>(CONFIG_KEYWORD::MODEL_FILE));

		return pModel;
	}
//	_ASSERTE(glGetError() == GL_NO_ERROR);
	return nullptr;
}
