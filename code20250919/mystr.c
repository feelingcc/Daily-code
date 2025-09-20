#include "mystr.h"

int mystrlen(const char * str) {
	char* end = str;
	while (*end) { end++; }
	return end - str;
}
