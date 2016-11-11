
#include <iostream>
#include <cstdlib>

using namespace std;

void handleErrors(int error, const char* description)
{
	cout << "ERROR: " << error << " :: " << description << endl;
	exit(0);
}

void handleErrors(int error, const char* description1, const char* description2)
{
	cout << "ERROR: " << error << " :: " << description1 << " " << description2 << endl;
	exit(0);
}
