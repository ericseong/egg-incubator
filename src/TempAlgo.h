#ifndef __TEMPALGO_H__
#define __TEMPALGO_H__ 
 
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <tuple>
#include <time.h>
using namespace std;

// for PI (Proportional Integral) controller-like temperature controller 
// registers some past values and t1, t2 is calculated based on those values
class TempAlgo {
	const unsigned NumRecords; // 7 (now, now-10min, now-20min, ... now-60min)
	const time_t RecInterval; // 600 = 10 min. 
	const float TempIncrement; // 3.0 = target temperature increment for a given interval.
	static time_t lastTick;

	struct element {
		time_t tick; // seconds
		float temp; // celsius
	}; 

	vector<element> _record;

	void _kickIn( time_t tick, float temp);
	float _getMedian( float *arr, unsigned size ) const;

public:
	TempAlgo() : NumRecords(61), RecInterval(10), TempIncrement(0.5) {} // change TempIncrement to modify temp curve. less value will increse the time required to warm-up.
//	TempAlgo() : NumRecords(7), RecInterval(600), TempIncrement(3.0) {} // debug
//	TempAlgo() : NumRecords(7), RecInterval(6), TempIncrement(3.0) {} // debug
	virtual ~TempAlgo() { 
		_record.clear();
		lastTick = 0;
	}
	pair<float,float> getPair( float t1, float t2 ) const;
	void tryGoOnRec( time_t tick, float temp ); 
	void printRecord() { // debug
		cout << "---------------------" << endl;
		for( int i = 0 ; i < (int)NumRecords ; i++ ) {
			cout << "[" << i << "] " << _record[i].tick << ", " << _record[i].temp << endl;
		}
		cout << "---------------------" << endl;
	}
};

#endif

// EOF

