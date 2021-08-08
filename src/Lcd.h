// Lcd.h

#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>

class Lcd {
	bool _initialized;
	uint16_t *_pBlackImage;
	void _devInit();
	void _devDeinit();
	void _prepareCanvas();
	void _refreshCanvas();
	void _destroyCanvas();
public:
	Lcd() : _initialized(false) {}
	virtual ~Lcd() {}
	void drawSolidLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color );
	void drawDottedLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color );
	void drawText( uint16_t x, uint16_t y, uint16_t color, char *text );
	void init();
	void deinit();
};

#endif

