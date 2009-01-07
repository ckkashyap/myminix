#ifndef __STRING_H__
#define __STRING_H__

#include <types.h>
void*	memcpy(void* s, const void* ct, size_t n);
int	memcmp(const void* s, const void* d, size_t n);
void*	memset(void* s, int c, size_t n);
char*	strcpy(char* s, const char* ct);
char*	strncpy(char* s, const char* ct, size_t n);
size_t	strlen(const char* cs);
int	strncmp(const char *s1, const char *s2, size_t n);
void	zero_bss();

#endif
