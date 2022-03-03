#include <stdio.h>
#include <string.h>
#include "gen_html.h"

static char* header = 
"<head> \
	<meta http-equiv='refresh' content='20'> \
	<style> \
	.large_red { \
		font-size: 30pt; \
		color: red; \
	} \
	.large_blue { \
		font-size: 30pt; \
		color: blue; \
	} \
	.large { \
		font-size: 30pt; \
		color: white; \
	} \
	.mid { \
		font-size: 20pt; \
		color: white; \
	} \
	.small { \
		font-size: 10pt; \
		color: white; \
	} \
	</style> \
	<script type='text/javascript'> \
		var audioCtx = new (window.AudioContext || window.webkitAudioContext || window.audioContext); \
		function beep(duration, frequency, volume, type, callback) { \
			var oscillator = audioCtx.createOscillator(); \
			var gainNode = audioCtx.createGain(); \
			oscillator.connect(gainNode); \
			gainNode.connect(audioCtx.destination); \
			if (volume){gainNode.gain.value = volume;} \
			if (frequency){oscillator.frequency.value = frequency;} \
			if (type){oscillator.type = type;} \
			if (callback){oscillator.onended = callback;} \
			oscillator.start(audioCtx.currentTime); \
			oscillator.stop(audioCtx.currentTime + ((duration || 500) / 1000)); \
		}; \
		function make_sound( key ) { \
			audioCtx.resume(); \
			cnt = 0; \
			while( cnt < 10 ) { \
				if( key == 'low' ) { setTimeout( function(){ beep( 500, 300, 0.5, 'sine', null ); }, 1000*cnt ); } \
				else if( key == 'high' ) { setTimeout( function(){ beep( 500, 1000, 0.5, 'sine', null ); }, 1000*cnt ); } \
				cnt += 1; \
			} \
		}; \
	</script> \
	</head> \
";

char* gen_header() {
	return header;	
}

static char body[32768];

char* gen_body( unsigned days, float temp, float temp_high, float temp_low, float humid, float humid_high, float humid_low, unsigned roller_count, char* date_time ) {
	memset( body, '\0', sizeof(body) );

	int i = 0;

	i  = sprintf( body, "%s", 
		"<body style=\'background-color:#000000;\'>"
	);

	/* days passed */
	i += sprintf( body+i, "%s%2d%s", 
		"<div class=mid>Day ",
		days,
		"</div><hr><br>"
	);

	/* temperature */
	i += sprintf( body+i, "%s%s%.2f%s",
		( temp < temp_low-1. ? "<div class=large_blue>" : temp > temp_high+1. ? "<div class=large_red>" : "<div class=large>" ),
		"T: ",
		temp,
		"</div>"	
	);

	/* humidity */
	i += sprintf( body+i, "%s%s%.2f%s",
		( humid < humid_low-5. ? "<div class=large_blue>" : humid > humid_high+5. ? "<div class=large_red>" : "<div class=large>" ),
		"H: ",
		humid,
		"</div>"	
	);

	/* roller_count */
	i += sprintf( body+i, "%s%s%u%s",
		"<div class=large>",
		"R: ",
		roller_count,
		"</div><br>"	
	);

	/* last update */
	i += sprintf( body+i, "%s%s%s",
		"<div class=small>Last update: ",
		date_time,
		"</div>"
	);

	/* dummy javascript to avoid unallowed autoplay */
	i += sprintf( body+i, "%s",
		"	<button id=\'hidden_button\' onclick=\'make_some_sound()\' hidden></button> \
			<script type=\'text/javascript\'> \
			function make_some_sound() { make_sound( \'low\' ); } \
			</script>"
	);

	i += sprintf( body+i, "%s",
	"	</body>"
	);

	return body;
}

static char html[65536];

char* gen_html( unsigned days, float temp, float temp_high, float temp_low, float humid, float humid_high, float humid_low, unsigned roller_count, char* date_time ) {
	memset( html, '\0', sizeof(html) );
	strcat( html, gen_header() );
	strcat( html, gen_body( days, temp, temp_high, temp_low, humid, humid_high, humid_low, roller_count, date_time ) );
	
	return html;
}

/* eof */

