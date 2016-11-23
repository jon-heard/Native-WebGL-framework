
#include <iostream>
#include <cstdlib>

using namespace std;

void handleErrors(int error, const char* msg1)
{
	cout << "ERROR: " << error << " :: " << msg1 << endl;
	exit(0);
}

void handleErrors(int error, const char* msg1, const char* msg2)
{
	cout << "ERROR: " << error << " :: " << msg1 << " " << msg2 << endl;
	exit(0);
}

void handleErrors(
		int error, const char* msg1, const char* msg2, const char* msg3)
{
	cout
		<< "ERROR: " << error << " :: "
		<< msg1 << " " << msg2 << " " << msg3 << endl;
	exit(0);
}
