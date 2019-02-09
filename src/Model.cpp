#include "../include/Model.h"
#include "../include/GraphicsInterface.h"
#include "../include/GraphicsMicro.h"
#include "../include/SkinnedMesh.h"
#include <assert.h>

using namespace Cooler;

CModel::CModel() : m_pMesh(nullptr)
{
}

CModel::~CModel()
{
	_SAFE_DELETE(m_pMesh);
}

bool Cooler::CModel::loadModelFile(const std::string& vModelFilePath, bool vWithAdjacencies /*= false*/)
{
	m_pMesh = new CSkinnedMesh;
	if (!m_pMesh->loadMesh(vModelFilePath))
	{
		Cooler::outputWarning(__EXCEPTION_SITE__, _BOOST_STR1("[%s%] model loads fail.", vModelFilePath));
		return false;
	}

	return true;
}

//***********************************************************
//FUNCTION:
void Cooler::CModel::render()
{
	assert(m_pMesh);
	m_pMesh->render();
}