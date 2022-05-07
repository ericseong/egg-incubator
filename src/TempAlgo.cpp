// TempAlgo.cpp
#include "TempAlgo.h"

time_t TempAlgo::lastTick = 0;

// kicks the measured temperature into the record-keeper 
void TempAlgo::_kickIn( time_t tick, float temp ) {
	element e;	
	e.tick = tick;
	e.temp = temp;
	if( _record.empty() ) {
		for( int i=0 ; i < (int)NumRecords ; i++ )
			_record.push_back( e ); 
		clog << "In TempAlgo::_kickIn(), record is stuffed with initial value: " << e.tick << ", " << e.temp << endl; 
	}
	else {
		_record.erase( _record.begin() ); // pop front!
		_record.push_back( e ); 
	}

	return;
} 

// check if it's elapsed for RecInterval
void TempAlgo::tryGoOnRec( time_t tick, float temp ) {
	if( tick - lastTick > RecInterval ) {
		_kickIn( tick, temp );
		lastTick = tick;
	}	
	else if( _record.empty() ) {
		_kickIn( tick, temp );
		lastTick = tick;
	}

	return;
}

float TempAlgo::_getMedian( float *arr, unsigned size ) const {
	sort( arr, arr+size );
	return arr[size/2];	
}

// this gets a new pair of target temperatures for a given interval, which'll be used to ramp up the temperature slowly
// input: t1/t2 from policy or config
// returns new t1/t2: new target using the past values 
// note that this is expected to be called after tryGoOnRec().
pair<float, float> TempAlgo::getPair( float t1, float t2 ) const {
	pair<float, float> tp;
	float arr[1024];

	// just in case
	if( _record.empty() ) {
		tp = { t1, t2 };
		return tp;	
	}

	//clog << "In TempAlgo::getPair():" << endl;
	for( int i=0 ; i< (int)NumRecords ; i++ ) {
		//clog << _record[i].temp << endl;
		arr[i] = _record[i].temp; 
	}
	//for( int i=0 ; i< 3 ; i++ )
	//	arr[i] = _record[(int)NumRecords/2-1+i].temp; 
	//float med = _getMedian( arr, 3 ); // to suppress some possible noisy values
	float med = _getMedian( arr, NumRecords ); // to suppress some possible noisy values
	float gap = t2-t1;

	if( med+TempIncrement >= t1 )
		tp = {t1, t2};
	else
		tp = { med + TempIncrement, med + TempIncrement + gap };
		
	return tp;	
}

// eof

