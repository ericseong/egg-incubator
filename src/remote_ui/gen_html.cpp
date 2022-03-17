// orginal code from:
//   https://gist.github.com/laobubu/d6d0e9beb934b60b2e552c2d03e1409e
//
#include <cstdio>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <string.h>
#include "gen_html.h"
#include "../LockedFileAccess.h"

static const char* header = 
"<head> \
	<meta charset='UTF-8' http-equiv='refresh' content='20'> \
	<style> \
	.large_red { \
		font_family: Verdana, sans-serif; \
		font-size: 24pt; \
		color: #FF66CC; \
	} \
	.large_blue { \
		font_family: Verdana, sans-serif; \
		font-size: 24pt; \
		color: #87CEFF; \
	} \
	.large { \
		font_family: Verdana, sans-serif; \
		font-size: 24pt; \
		color: #DDDDDD; \
	} \
	.mid { \
		font_family: Verdana, sans-serif; \
		font-size: 20pt; \
		color: #DDDDDD; \
	} \
	.small { \
		font_family: Verdana, sans-serif; \
		font-size: 10pt; \
		color: #DDDDDD; \
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
		function make_no_sound() {;}; \
		function make_sound( key ) { \
			audioCtx.resume(); \
			cnt = 0; \
			while( cnt < 10 ) { \
				if( key == 'low' ) { setTimeout( function(){ beep( 500, 300, 1.5, 'sine', null ); }, 1000*cnt ); } \
				else if( key == 'high' ) { setTimeout( function(){ beep( 500, 1000, 0.5, 'sine', null ); }, 1000*cnt ); } \
				cnt += 1; \
			} \
		}; \
	</script> \
	</head> \
";

static const char* gen_header() {
	return header;	
}

static char body[32768];

static char* gen_body( unsigned days, unsigned hours, float temp, float temp_high, float temp_low, float humid, float humid_high, float humid_low, unsigned roller_count, const char* date_time, bool is_phl ) {
	memset( body, '\0', sizeof(body) );

	int i = 0;

#if 0
	i  = sprintf( body, "%s", 
		( !is_phl ? "<body style=\'background-color: #808080;\'>" :
			"<body style=\'background-color:#202020;\'>"
		)
	);
#else
	i  = sprintf( body, "%s", 
		( !is_phl ? "<body style=\'background-image: linear-gradient(to bottom, #808080, #c0c0c0);\'>" :
			"<body style=\'background-color:#202020;\'>"
		)
	);
#endif

	/* days passed */
	i += sprintf( body+i, "%s%2d%s%d%s%s", 
		"<div class=mid>경과한 날: ",
		days,
		"일 ",
		hours,
		"시간",
		"</div><hr><br>"
	);

	/* temperature */
	i += sprintf( body+i, "%s%s%.2f%s%s",
		( temp < temp_low ? "<div class=large_blue>" : temp > temp_high ? "<div class=large_red>" : "<div class=large>" ),
		"온도 ",
		temp,
		" &#x2103",
		"</div>"	
	);

	/* humidity */
	i += sprintf( body+i, "%s%s%.2f%s%s",
		( humid < humid_low ? "<div class=large_blue>" : humid > humid_high ? "<div class=large_red>" : "<div class=large>" ),
		"습도 ",
		humid,
		" %",
		"</div>"	
	);

	/* roller_count */
	i += sprintf( body+i, "%s%u%s%s",
		"<div class=large>",
		roller_count,
		"번 굴렸습니다.",
		"</div><br>"	
	);

	/* last update */
	i += sprintf( body+i, "%s%s%s",
		"<div class=small style=\'font-style:italic;\'>최근 갱신: ",
		date_time,
		"</div>"
	);

	/* dummy javascript to avoid unallowed autoplay and is to be called by js injection on target device after the page is loaded */
	i += sprintf( body+i, "%s",
		( !is_phl && ( temp < temp_low-0.5 || temp > temp_high+0.5 || humid < humid_low-5. || humid > humid_high+5. ) ) ?
		"	<button id=\'hidden_button\' onclick=\'make_some_sound()\' hidden></button> \
			<script type=\'text/javascript\'> \
				function make_some_sound() { make_sound( \'low\' ); } \
			</script>" :
		"	<button id=\'hidden_button\' onclick=\'make_some_sound()\' hidden></button> \
			<script type=\'text/javascript\'> \
				function make_some_sound() { make_no_sound(); } \
			</script>"
	);

	i += sprintf( body+i, "%s",
	"	</body>"
	);

	return body;
}

static char html[65536];

static char* gen_html( unsigned days, unsigned hours, float temp, float temp_high, float temp_low, float humid, float humid_high, float humid_low, unsigned roller_count, const char* date_time, bool is_phl ) {
	strcat( html, gen_header() );
	strcat( html, gen_body( days, hours, temp, temp_high, temp_low, humid, humid_high, humid_low, roller_count, date_time, is_phl ) );
	
	return html;
}

static bool iequals(const std::string& a, const std::string& b) {
	return std::equal(a.begin(), a.end(),
		b.begin(), b.end(),
		[](char a, char b) {
		return tolower(a) == tolower(b);
	});
}

// external interface
char* gen_html_from_stat_file() {
	bool succ_file_read = true;
	memset( html, '\0', sizeof(html) );

  LockedFileAccess lfa( CUR_SESSION_STAT_FILE_NAME );
  std::string str;
	
	if( lfa.readFile( str ) ) {
		succ_file_read = false;
		std::cerr << "fileRead() failed from " << CUR_SESSION_STAT_FILE_NAME << std::endl;
	}

  /*
  format string for 10 items, or 11 space separated fields (space is there between date and time)
    elapsed_tick,
    date/time,
    temp,
    temp_high,
    temp_low,
    humid,
    humid_high,
    humid_low,
    roller_count,
    bool(periodic_hen_leaving)
**/
	static unsigned elapsed_tick, roller_count, days, hours;
	static float temp, temp_high, temp_low, humid, humid_high, humid_low; 
	static std::string date, time;
	static std::string phl;
	static bool is_phl=false;
	static std::istringstream ss(str);
	if( succ_file_read ) { // intention is to reuse the previous values if we can't read from stat file.
		ss >> elapsed_tick >> date >> time >> temp >> temp_high >> temp_low >> humid >> humid_high >> humid_low >> roller_count >> phl;
		days = elapsed_tick / (60*60*24);
		hours = ( elapsed_tick % (60*60*24) ) / 3600;
		date += " "+time;
		is_phl = iequals( phl, "true" ); 
	}
	
	return gen_html( days, hours, temp, temp_high, temp_low, humid, humid_high, humid_low, roller_count, date.c_str(), is_phl );
}

/* eof */

