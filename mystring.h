#ifndef __MY_STRING_H__
#define __MY_STRING_H__

// Strip trailing characters
extern char *strstrip(char *str, char *trail);

// Chomp trailing CR/LF
extern char *strchomp(char *str);

// Return string as a file name
extern char *strtofile(const char *str, const char *ext);

#endif//__MY_STRING_H__
