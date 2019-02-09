#include "../include/OutputToScreen.h"
#include "../include/ProductFactory.h"
#include <iostream>

using namespace Cooler;
CProductFactory<COutputToScreen> theCreator(OUTPUT_EVENT_2_SCREEN);

COutputToScreen::COutputToScreen()
{
}

//*********************************************************************************
//FUNCTION:
void COutputToScreen::_outputEventV(const std::string& vEvent)
{
	std::cout << vEvent << std::endl;
}