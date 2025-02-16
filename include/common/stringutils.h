#ifndef INCLUDE_COMMON_STRINGUTILS_H_
#define INCLUDE_COMMON_STRINGUTILS_H_

char* ltrim(char *str, const char *seps);
char* rtrim(char *str, const char *seps);
char* trim(char *str, const char *seps);
char* next_token(char *buffer, char *str, char delimiter);

#endif
