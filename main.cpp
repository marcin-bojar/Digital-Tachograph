#include "tacho.h"
#include "drivercard.h"
#include "time.h"
#include "functions.h"

using namespace std;


int main()
{	
	cout << "Welcome to digital tachograph simulator.\n";
	cout << "This program is based on Regulation (EC) No 561/2006 of the European Parliament defining limits of work- and drive- times for truck drivers in EU.\n\n";
	cout << "Press ENTER to proceed...\n";
	while (cin.get() != '\n')
		continue;
	Tacho tachograph;
	cout << "Current view of tachograph:\n\n";
	tachograph.show();
	showmenu();
	tachograph.use_tacho();

	return 0;

}


