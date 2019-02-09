#include "../include/Render.h"
#include "../include/GraphicsCommon.h"
#include "../include/GraphicsMicro.h"
#include "../include/GraphicsInterface.h"
#include "../include/GraphicsConfig.h"
#include "../include/XmlParser.h"
#include "../include/DisplayWindow.h"
#include "../include/Effect.h"
#include "../include/EffectManager.h"
#include "../include/ResourceExplorer.h"
#include "../include/Camera.h"
#include "../include/CameraManager.h"
#include "../include/ICallBack.h"
#include "../include/InputTransformer.h"
#include "../include/TweakBar.h"
#include <assert.h>

using namespace Cooler;

IRender::IRender() : m_pDisplayWindow(nullptr), m_pAntTweakBar(nullptr)
{
	m_pInputTransformer = new CInputTransformer;
}

IRender::~IRender()
{
	_SAFE_DELETE(m_pDisplayWindow);
	_SAFE_DELETE(m_pInputTransformer);
	_SAFE_DELETE(m_pAntTweakBar);
	for (auto config : m_ConfigSet) _SAFE_DELETE(config.second);
	for (auto effect : m_EffectMap) _SAFE_DELETE(effect.second);
}

//***********************************************************
//FUNCTION::
void Cooler::IRender::init()
{
	__loadConfig();
	if (!__initEnvironment()) Cooler::outputWarning(__EXCEPTION_SITE__, "Init OpenGL environment fail.");

	assert(glGetError() == GL_NO_ERROR);
	__createResources();
	assert(glGetError() == GL_NO_ERROR);
	__initAllEffect();
	assert(glGetError() == GL_NO_ERROR);
	__updateWindowSize();
	assert(glGetError() == GL_NO_ERROR);

//	_initRenderV();
}

//***********************************************************
//FUNCTION::
void Cooler::IRender::run()
{
	if (!m_pDisplayWindow)
	{
		fprintf(stderr, "Error: can not run without window instance \n");
		return;
	}
	
	while (!m_pDisplayWindow->windowShouldClose())
	{	
		GLdouble CurrentTime = glfwGetTime();
		m_DeltaTime = CurrentTime - m_LastTime;
		m_LastTime = CurrentTime;

		glfwPollEvents();

		m_pInputTransformer->update(m_DeltaTime);
	//	_renderV();

		glViewport(0, 0, m_pDisplayWindow->getWindowWidth(), m_pDisplayWindow->getWindowHeight());

		for (auto effect : m_EffectMap)
			effect.second->loadEffect();

		if (m_pAntTweakBar) m_pAntTweakBar->renderTweakBar();

		glfwSwapBuffers(m_pDisplayWindow->fetchWindowInstance());
	}

	__destroy();
}

//***********************************************************
//FUNCTION:
Cooler::CInputTransformer* Cooler::IRender::fetchInputTransformer() const
{
	assert(m_pInputTransformer);
	return m_pInputTransformer;
}

//***********************************************************
//FUNCTION:
Cooler::CDisplayWindow* Cooler::IRender::fetchDisplayWindow() const
{
	assert(m_pDisplayWindow);
	return m_pDisplayWindow;
}

//***********************************************************
//FUNCTION:
Cooler::CAntTweakBar* Cooler::IRender::fetchAntTweakBar() const
{
	return m_pAntTweakBar;
}

//***********************************************************
//FUNCTION:
bool Cooler::IRender::pointerInput(SInputDeviceType::Enum device, SPointerActionType::Enum action, uint32_t modifiers, int32_t count, CPointerEvent* points, int64_t timestamp /*= 0*/)
{
	m_pInputTransformer->processPointer(device, action, modifiers, count, points);
	
	return true;
}

//***********************************************************
//FUNCTION:
bool Cooler::IRender::keyInput(uint32_t code, SKeyActionType::Enum action)
{
	return m_pInputTransformer->processKey(code, action);
}

//***********************************************************
//FUNCTION::
bool Cooler::IRender::_renderEffect(const std::string& vEffectSig)
{
	if (m_EffectMap.find(vEffectSig) == m_EffectMap.end())
	{
		Cooler::outputWarning(__EXCEPTION_SITE__, vEffectSig + " is not exist.");
		return false;
	}

	m_EffectMap[vEffectSig]->loadEffect();

	return true;
}

//***********************************************************
//FUNCTION::
void Cooler::IRender::__loadConfig()
{
	CGraphicsConfig* pEffectConfig = new CGraphicsConfig;
	assert(pEffectConfig);
	CXmlParser::getInstance()->parseXmlFile(Cooler::DEFAULT_CONFIG::EFFECT_PATH, pEffectConfig);
	m_ConfigSet[Cooler::DEFAULT_CONFIG::EFFECT_PATH] = pEffectConfig;

	CGraphicsConfig* pResourceConfig = new CGraphicsConfig;
	assert(pResourceConfig);
	CXmlParser::getInstance()->parseXmlFile(Cooler::DEFAULT_CONFIG::RESOURCE_PATH, pResourceConfig);
	m_ConfigSet[Cooler::DEFAULT_CONFIG::RESOURCE_PATH] = pResourceConfig;

	CGraphicsConfig* pSceneConfig = new CGraphicsConfig;
	assert(pSceneConfig);
	CXmlParser::getInstance()->parseXmlFile(Cooler::DEFAULT_CONFIG::SCENE_PATH, pSceneConfig);
	m_ConfigSet[Cooler::DEFAULT_CONFIG::SCENE_PATH] = pSceneConfig;
}

//***********************************************************
//FUNCTION:
void Cooler::IRender::__createResources()
{
	CResourceExplorer::getInstance()->createAllResourceManager();
	CResourceExplorer::getInstance()->createResource(m_ConfigSet[DEFAULT_CONFIG::RESOURCE_PATH]);
	CResourceExplorer::getInstance()->createResource(m_ConfigSet[DEFAULT_CONFIG::SCENE_PATH]);
}

//***********************************************************
//FUNCTION::
bool Cooler::IRender::__initEnvironment()
{
	std::string WindowTitle;
	unsigned int WindowWidth = 0, WindowHeight = 0;

	const CGraphicsConfig* pWindowConfig = m_ConfigSet[DEFAULT_CONFIG::SCENE_PATH]->fetchSubConfigByName(CONFIG_KEYWORD::WINDOW);
	assert(pWindowConfig);
	_GRAPHICS_SIMPLE_IF_ELSE(!pWindowConfig->isAttributeExisted(CONFIG_KEYWORD::WINDOW_TITLE), WindowTitle = DEFAULT_CONFIG::DEFAULT_WIN_TITLE, WindowTitle = pWindowConfig->getAttribute<std::string>(CONFIG_KEYWORD::WINDOW_TITLE));
	_GRAPHICS_SIMPLE_IF_ELSE(!pWindowConfig->isAttributeExisted(CONFIG_KEYWORD::WINDOW_WIDTH), WindowWidth = DEFAULT_CONFIG::DEFAULT_WIN_WIDTH, WindowWidth = pWindowConfig->getAttribute<int>(CONFIG_KEYWORD::WINDOW_WIDTH));
	_GRAPHICS_SIMPLE_IF_ELSE(!pWindowConfig->isAttributeExisted(CONFIG_KEYWORD::WINDOW_HEIGHT), WindowHeight = DEFAULT_CONFIG::DEFAULT_WIN_HEIGHT, WindowHeight = pWindowConfig->getAttribute<int>(CONFIG_KEYWORD::WINDOW_HEIGHT));
	
	if (m_pDisplayWindow) delete m_pDisplayWindow;
	m_pDisplayWindow = new CDisplayWindow;
	m_pDisplayWindow->createWindow(WindowTitle, WindowWidth, WindowHeight);

	glfwMakeContextCurrent(m_pDisplayWindow->fetchWindowInstance());//���ڵ����������õ�ǰ�̵߳�������
	glewExperimental = GL_TRUE;//�����κ�OpenGL����֮ǰ��ʼ��GLEW, glewExperimental����GLEW����OpenGL����ָ�����ʹ���ִ�������
	GLuint Res = glewInit();
	if (Res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(Res));
		return false;
	}
	glGetError();

	if (pWindowConfig->isAttributeExisted(CONFIG_KEYWORD::WIN_TWEAKBAR))
	{
		m_pAntTweakBar = new CAntTweakBar;
		m_pAntTweakBar->createTweakBar();
	}

// 	glfwSetKeyCallback(m_pDisplayWindow->fetchWindowInstance(), __GLFWKeyBoardCB);
//	glfwSetCursorPosCallback(m_pDisplayWindow->fetchWindowInstance(), __GLFWCursorCB);
//	glfwSetScrollCallback(m_pDisplayWindow->fetchWindowInstance(), __GLFWScrollCB);
	__setInputCallbacksGLFW(m_pDisplayWindow->fetchWindowInstance(), this);

#ifdef _DEBUG
	// enable OpenGL debug context if context allows for debug context
	GLint Flags; glGetIntegerv(GL_CONTEXT_FLAGS, &Flags);
	if (Flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);			// makes sure errors are displayed synchronously
		glDebugMessageCallback(glDebugOutput, nullptr); //�����¼�ص�����
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif // _DEBUG

	return true;
}

//***********************************************************
//FUNCTION::
bool Cooler::IRender::__initAllEffect()
{
	CEffectManager::getInstance()->createAllEffect(m_ConfigSet[DEFAULT_CONFIG::EFFECT_PATH], m_EffectMap);
	
	for (auto effect : m_EffectMap)
		effect.second->_initEffectV();
	
	return true;
}

//***********************************************************
//FUNCTION::
void Cooler::IRender::__destroy()
{
	for (auto effect : m_EffectMap)
		effect.second->_destoryEffectV();
	TwTerminate();
	glfwTerminate();
}

//***********************************************************
//FUNCTION:
void Cooler::IRender::__updateWindowSize()
{
	int32_t WindowWidth = 0, WidthHeight = 0;
	glfwGetFramebufferSize(m_pDisplayWindow->fetchWindowInstance(), &WindowWidth, &WidthHeight);
	__GLFWReshapeCB(m_pDisplayWindow->fetchWindowInstance(), WindowWidth, WidthHeight);
}