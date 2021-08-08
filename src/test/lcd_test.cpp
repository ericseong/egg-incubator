#include <string>
#include <iostream>

#include "DEV_Config.h"
#include "LCD_1in3.h"
#include "GUI_Paint.h"
#include "GUI_BMP.h"
#include "../InfoPanel.h"

using namespace std;

InfoPanel ip;

int main() {
	ip.init();

	//void InfoPanel::drawInfoPanel( const char* header, uint16_t headerColor, const char* info1, uint16_t info1Color, const char* info2, uint16_t info2Color ) 
	ip.drawInfoPanel("Header", RED, "info1", BLUE, "info2", GREEN );

	ip.deinit();
	return 0;
}

// eof


