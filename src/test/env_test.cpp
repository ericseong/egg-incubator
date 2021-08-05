#include "../Env.h"

#include <iostream>
#include <json/json.h>
config_t cfg;

void print_config( config_t& c ) {

	cout << c.name << endl;
	cout << c.days << endl;
	cout << c.maxDays << endl;

	for( int i = 0 ; i < NUM_PHASES ; i++ ) {
		cout << c.phases[i].name << endl;
		cout << c.phases[i].startDate << endl;
		cout << c.phases[i].endDate << endl;
		cout << c.phases[i].tempLowerLimit << endl;
		cout << c.phases[i].tempHigherLimit << endl;
		cout << c.phases[i].humidLowerLimit << endl;
		cout << c.phases[i].humidHigherLimit << endl;
		cout << c.phases[i].airFlowLevel << endl;
		cout << c.phases[i].rollInterval << endl;
	}

	return;
}

int main() {

	Env& env = Env::getInstance();
	config_t cfg;

	if( !env.getConfig( cfg ) )
	 cout << "getConfig() success." << endl;
	else
		cout << "getConfig() fail." << endl;		
	
	print_config( cfg );

	return 0;
}

// EOF

