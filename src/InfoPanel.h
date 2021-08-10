// InfoPanel.h

#ifndef __INFOPANEL_H__
#define __INFOPANEL_H__ 

#include <stdint.h>

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
			const string& header, uint16_t headerColor, 
			const string& info1, uint16_t info1Color, 
			const string& info2, uint16_t info2Color,
			const string& info3, uint16_t info3Color,
			const string& footer, uint16_t footerColor 
	);
	void init();
	void deinit();
};

#endif

