#include"common/stringutils.h"
#include"kernel_libc/string.h"

// http://www.martinbroadhurst.com/trim-a-string-in-c.html
char *ltrim(char *str, const char *seps){
	int totrim;
	    if (seps == 0) {
	        seps = "\t\n\v\f\r ";
	    }
	    totrim = strspn(str, seps);
	    if (totrim > 0) {
	    	int len = strlen(str);
	        if (totrim == len) {
	            str[0] = '\0';
	        }
	        else {
	            memcpy(str, str + totrim, len + 1 - totrim);
	        }
	    }
	    return str;
}
char *rtrim(char *str, const char *seps){
	int i;
	    if (seps == 0) {
	        seps = "\t\n\v\f\r ";
	    }
	    i = strlen(str) - 1;
	    while (i >= 0 && strchr(seps, str[i]) != 0) {
	        str[i] = '\0';
	        i--;
	    }
	    return str;
}
char *trim(char *str, const char *seps){
	return ltrim(rtrim(str, seps), seps);
}



