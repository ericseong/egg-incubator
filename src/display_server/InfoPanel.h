// InfoPanel.h

#ifndef __INFOPANEL_H__
#define __INFOPANEL_H__ 

#include <string>
#include <stdint.h>
#include <string>

// from lib/GUI/GUI_Paint.h of 1.3" LCD HAT library
#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define RED            0xF800

class InfoPanel {
	bool _initialized;
	uint16_t *_pBlackImage;
	void _devInit();
	void _devDeinit();
	void _prepareCanvas();
	void _refreshCanvas();
	void _destroyCanvas();
	void _drawSolidLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color );
	void _drawDottedLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color );
	void _drawText( uint16_t x, uint16_t y, uint16_t color, char *text );
public:
	InfoPanel() : _initialized(false) {}
	virtual ~InfoPanel() {}
	void drawInfoPanel( 
			const std::string& header, uint16_t headerColor, 
			const std::string& info1, uint16_t info1Color, 
			const std::string& info2, uint16_t info2Color,
			const std::string& info3, uint16_t info3Color,
			const std::string& footer, uint16_t footerColor 
	);
	bool isRequestNewSession() const;
	void drawInfoNewSession();
	void init();
	void deinit();
};

#endif

