
#include <iostream>
#include <cstdlib>

using namespace std;

void handleErrors(int error, const char *description)
{
	cout << "ERROR: " << error << " :: " << description << endl;
	exit(0);
}
