#pragma once
//#include <AntTweakBar/AntTweakBar.h>
#include "AntTweakBar.h"
#include "assert.h"

namespace Cooler
{
	class CAntTweakBar
	{
	public:
		CAntTweakBar();
		~CAntTweakBar();

		void createTweakBar();
		void renderTweakBar();

		bool __TweakBarMouseCB(int vButton, int vAction);
		bool __TweakBarMotionCB(int vPosX, int vPosY);

		TwBar* fetchTweakBar() const { assert(m_pTwBar); return m_pTwBar; }

	private:
		

		TwBar* m_pTwBar;
	};
}