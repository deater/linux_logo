#include <stdio.h>
#include <string.h>

char *strncpy_truncate(char *dest, const char *src, size_t n) {

	memcpy(dest,src,n);
	dest[n-1]=0;

	return dest;
}
