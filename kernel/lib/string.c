#include <types.h>

void* 	memcpy(void* s, const void* ct, size_t n)
{
	char *dest,*src;
	size_t i;

	dest = (char*)s;
	src = (char*)ct;
	for ( i = 0; i < n; i++)
		dest[i] = src[i];
	
	return s;
}

int 	memcmp(const void* s, const void* d, size_t n)
{
	size_t i;

	for ( i = 0; i < n; i++ )
	{
		if ( ((const char*)s)[i] <  ((const char*)d)[i] ) return -1;
		if ( ((const char*)s)[i] >  ((const char*)d)[i] ) return 1;
	}

	return 0;
}


void* 	memset(void* s, int c, size_t n)
{
	size_t i;
	for ( i = 0; i < n ; i++)
		((char*)s)[i] = c;
	
	return s;
}


char* 	strcpy(char* s, const char* ct)
{
	size_t i = 0;
	do { s[i] = ct[i]; } 
		while (ct[i++] != '\0');
	return s;
}

char* 	strncpy(char* s, const char* ct, size_t n)
{
	size_t i;

	for (i = 0 ; i < n && ct[i] != '\0' ; i++)
		s[i] = ct[i];
	for ( ; i < n ; i++)
		s[i] = '\0';

	return s;
}

 
size_t 	strlen(const char* cs)
{
	size_t ans = 0;
	while ( cs[ans++] != '\0' ) {}
	return (ans-1);
}


int strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i = 0;

	while ( i < n )
	{
		if ( s1[i] < s2[i] ) return -1;
		if ( s2[i] < s1[i] ) return +1;

		if ( s1[i] == 0 ) break;

		i += 1;
	}

	return 0;
}



extern char __bss_start;
extern char __bss_end;

void zero_bss()
{
  char *bss = &__bss_start;
  while ( bss != &__bss_end ) *bss++ = 0;
}



/** @} */

