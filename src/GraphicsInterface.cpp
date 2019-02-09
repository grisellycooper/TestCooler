#include "../include/GraphicsInterface.h"
#include "../include/ProductFactoryData.h"
#include "../include/EventLogger.h"
#include "../include/ResourceExplorer.h"
#include "../include/TextureManager.h"
#include "../include/Texture.h"
#include "../include/CameraManager.h"
#include "../include/Camera.h"
#include "../include/Model.h"
#include "../include/ModelManager.h"
#include "../include/Render.h"
#include "../include/TweakBar.h"
#include "../include/FBO.h"
#include "../include/FBOManager.h"
#include "../include/DisplayWindow.h"
#include <assert.h>
#include <iostream>

using namespace Cooler;

//***********************************************************
//FUNCTION:
void Cooler::initEngine()
{
	IRender::getInstance()->init();
}

//***********************************************************
//FUNCTION:
void Cooler::runEngine()
{
	IRender::getInstance()->run();
}

//***********************************************************
//FUNCTION:
CInputTransformer* Cooler::fetchInputTransformer()
{
	return IRender::getInstance()->fetchInputTransformer();
}

//***********************************************************
//FUNCTION:
TwBar* Cooler::fetchTweakBar()
{
	CAntTweakBar* pTweakBar = IRender::getInstance()->fetchAntTweakBar();
	return  pTweakBar->fetchTweakBar();
}

//***********************************************************
//FUNCTION:
CFBO* Cooler::fetchFBO(const std::string& vFBOName)
{
	assert(!vFBOName.empty());
	CFBOManager* pFBOManager = dynamic_cast<CFBOManager*>(CResourceExplorer::getInstance()->findResourceManager(CONFIG_KEYWORD::FRAMEBUFFER));
	assert(pFBOManager);
	CFBO* pFBO = dynamic_cast<CFBO*>(pFBOManager->fetchResource(vFBOName));
	assert(pFBO);
	return pFBO;
}

//***********************************************************
//FUNCTION::
CBaseProduct* Cooler::getOrCreateProduct(const std::string& vSig)
{
	return CProductFactoryData::getInstance()->getOrCreateProduct(vSig);
}

//***********************************************************
//FUNCTION::
CBaseProduct* Cooler::createProduct(const std::string& vSig)
{
	return CProductFactoryData::getInstance()->createProduct(vSig);
}

//***********************************************************
//FUNCTION:
void Cooler::outputWarning(const std::string& vSource, const std::string& vFile, int vLine, const std::string& vDescription)
{
	CEventLogger::getInstance()->outputWarning(vSource, vFile, vLine, vDescription);
}

void Cooler::outputGraphicsError(const std::string& vFileName, unsigned int vLine, const std::string& vError)
{
	//TODO:
}

//***********************************************************
//FUNCTION:
glm::mat4 Cooler::fectchCameraViewMatrix(const std::string& vCameraName)
{
	CCamera* pCamera = graphicsFindCamera(vCameraName);
	return pCamera->getViewMatrix();
}

//***********************************************************
//FUNCTION::
glm::mat4 Cooler::fetchCameraProjectionMatrix(const std::string& vCameraName)
{
	CCamera* pCamera = graphicsFindCamera(vCameraName);
	return pCamera->getProjectionMatrix();
}

//***********************************************************
//FUNCTION:
void Cooler::graphicsRenderModel(const std::string& vModelName)
{
	assert(!vModelName.empty());
	CModelManager* pModelManager = dynamic_cast<CModelManager*>(CResourceExplorer::getInstance()->findResourceManager(CONFIG_KEYWORD::MODEL));
	assert(pModelManager);
	CModel* pModel = dynamic_cast<CModel*>(pModelManager->fetchResource(vModelName));
	assert(pModel);
	pModel->render();
}

//***********************************************************
//FUNCTION::
std::pair<int, int> Cooler::getDisplayWindowsSize()
{
	int Width = IRender::getInstance()->fetchDisplayWindow()->getWindowWidth();
	int Height = IRender::getInstance()->fetchDisplayWindow()->getWindowHeight();
	assert(Width > 0 && Height > 0);
	return std::make_pair(Width, Height);
}

//***********************************************************
//FUNCTION::
Cooler::CTexture* Cooler::graphicsFindTexture(const std::string& vTexName)
{
	assert(!vTexName.empty());
	CTextureManager* pTexManager = dynamic_cast<CTextureManager*>(CResourceExplorer::getInstance()->findResourceManager(CONFIG_KEYWORD::TEXTURE));
	assert(pTexManager);
	CTexture* pTexture = dynamic_cast<CTexture*>(pTexManager->fetchResource(vTexName));
	assert(pTexture);
	return pTexture;
}

//***********************************************************
//FUNCTION::
Cooler::CCamera* Cooler::graphicsFindCamera(const std::string& vCameraName)
{
	assert(!vCameraName.empty());
	CCameraManager* pCameraManager = dynamic_cast<CCameraManager*>(CResourceExplorer::getInstance()->findResourceManager(CONFIG_KEYWORD::CAMERA));
	assert(pCameraManager);
	CCamera* pCamera = dynamic_cast<CCamera*>(pCameraManager->fetchResource(vCameraName));
	assert(pCamera);
	return pCamera;
}

//***********************************************************
//FUNCTION:
GLenum Cooler::graphicsCheckError_(const char *file, int line)
{
	GLenum ErrorCode;
	while ((ErrorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string Error;
		switch (ErrorCode)
		{
		case GL_INVALID_ENUM:                  Error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 Error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             Error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                Error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               Error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 Error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: Error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << Error << " | " << file << " (" << line << ")" << std::endl;
	}

	return ErrorCode;
}
