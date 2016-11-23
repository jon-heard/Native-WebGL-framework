#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

void handleErrors(int error, const char* msg1);
void handleErrors(int error, const char* msg1, const char* msg2);
void handleErrors(
		int error, const char* msg1, const char* msg2, const char* msg3);

#endif // ERRORHANDLING_H
