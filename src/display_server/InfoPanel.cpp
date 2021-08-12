// InfoPanel.cpp

#include <string> 
#include <iostream> 

#include "DEV_Config.h"
#include "LCD_1in3.h"
#include "GUI_Paint.h"
#include "GUI_BMP.h"
#include "InfoPanel.h"

using namespace std;

void InfoPanel::_devInit() {
	if(DEV_ModuleInit() != 0){
		DEV_ModuleExit();
		cerr << "LCD module init failed." << endl; 
		return;
	}

	LCD_1in3_Init(HORIZONTAL);
	LCD_1in3_Clear(BLACK);
	return;
}

void InfoPanel::_devDeinit() {
	DEV_ModuleExit();
	return;
}

void InfoPanel::_destroyCanvas() {
	free( (void*)_pBlackImage );
	_pBlackImage = NULL;
	return;
}

void InfoPanel::_prepareCanvas() {

	UWORD *BlackImage;
	UDOUBLE Imagesize = LCD_HEIGHT*LCD_WIDTH*2;
	//printf("Imagesize = %d\r\n", Imagesize);
	if((BlackImage = (UWORD *)malloc(Imagesize)) == NULL) {
		//printf("Failed to apply for black memory...\r\n");
		cerr << "Malloc for lcd failed." << endl; 
		DEV_ModuleExit();
		//exit(0);
	}
	// printf("size = %d\r\n",sizeof(BlackImage) / sizeof(UWORD));
	// /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
	Paint_NewImage(BlackImage, LCD_WIDTH, LCD_HEIGHT, 0, BLACK, 16);
	Paint_Clear(BLACK);

	_pBlackImage = BlackImage;
	return;
}

// show canvas to display
void InfoPanel::_refreshCanvas() {

	LCD_1in3_Display(_pBlackImage);
	DEV_Delay_ms(2000);
	return;
}

void InfoPanel::init() {
	if( _initialized )
		return;

	_devInit();

	_initialized = true;
	return;
}

void InfoPanel::deinit() {
	if( !_initialized )
		return;

	_devDeinit();
	_initialized = false;
	return;
}

void InfoPanel::_drawSolidLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color ) {
	if( !_initialized )
		return;
	
	Paint_DrawLine(x1, y1, x2, y2, color, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	return;
}

void InfoPanel::_drawDottedLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color ) {
	if( !_initialized )
		return;

	Paint_DrawLine(x1, y1, x2, y2, color, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
	return;
}

void InfoPanel::_drawText( uint16_t x, uint16_t y, uint16_t color, char *text ) {
	if( !_initialized )
		return;

	Paint_DrawString_EN(x, y, text, &Font16, color, GRAY);
	return;
}

void InfoPanel::drawInfoPanel( 
			const string& header, uint16_t headerColor, 
			const string& info1, uint16_t info1Color, 
			const string& info2, uint16_t info2Color,
			const string& info3, uint16_t info3Color,
			const string& footer, uint16_t footerColor 
	) {

	if( !_initialized )
		return;
	
	_prepareCanvas();

	Paint_DrawString_EN( 10,  10, header.c_str(), &Font24, BLACK, headerColor);
	_drawSolidLine( 10, 40, 230, 40, WHITE );
	Paint_DrawString_EN( 10,  70, info1.c_str(), &Font24, BLACK, info1Color);
	Paint_DrawString_EN( 10, 120, info2.c_str(), &Font24, BLACK, info2Color);
	Paint_DrawString_EN( 10, 170, info3.c_str(), &Font24, BLACK, info3Color);
	Paint_DrawString_EN( 10, 210, footer.c_str(), &Font16, BLACK, footerColor);

	//clog << "header: " << header.c_str() << " ,color: " << headerColor << endl; 
	//clog << "info1: " << info1.c_str() << " ,color: " << info1Color << endl; 
	//clog << "info2: " << info2.c_str() << " ,color: " << info2Color << endl; 
	//clog << "info3: " << info3.c_str() << " ,color: " << info3Color << endl; 
	//clog << "footer: " << footer.c_str() << " ,color: " << footerColor << endl; 

	_refreshCanvas(); // show!
	_destroyCanvas();

	return;
}

void InfoPanel::drawInfoNewSession() {

	if( !_initialized )
		return;
	
	_prepareCanvas();

	string str1( "NEW SESSION" );
	string str2( "..." );
	string str3( "..." );
	string str4( "..." );
	Paint_DrawString_EN( 10, 10, str1.c_str(), &Font24, BLACK, WHITE);
	//sleep(1);
	Paint_DrawString_EN( 10, 70, str2.c_str(), &Font24, BLACK, WHITE);
	//sleep(1);
	Paint_DrawString_EN( 10, 120, str3.c_str(), &Font24, BLACK, WHITE);
	//sleep(1);
	Paint_DrawString_EN( 10, 170, str4.c_str(), &Font24, BLACK, WHITE);
	//sleep(1);

	_refreshCanvas(); // show!
	_destroyCanvas();

	return;
}

bool InfoPanel::isRequestNewSession() const {
	bool key1Pressed = false;
	bool key2Pressed = false;
	static unsigned countPressed = 0;

  if( !_initialized )
    return false;

	//clog << "GET_KEY1" << GET_KEY1 << endl;
	//clog << "GET_KEY2" << GET_KEY2 << endl;
	key1Pressed = ( GET_KEY1 == 0 ? true : false );
	key2Pressed = ( GET_KEY2 == 0 ? true : false );

	if( key1Pressed && key2Pressed ) {
		countPressed++;
	} else {
		countPressed = 0;
	}	

	if( countPressed >= 3 ) {
		countPressed = 0;
		clog << "----------------------------\n";
		clog << "CAUGHT NEW SESSION REQUEST!!\n";
		clog << "----------------------------\n";
		return true;	
	}
	
	return false;
}

// EOF

