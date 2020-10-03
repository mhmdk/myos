#include"kernel_libc/stdlib.h"

// https://www.geeksforgeeks.org/implement-itoa/

void reverse(char str[], int length) {
	int start = 0;
	int end = length - 1;
	while (start < end) {
		char tmp = str[start];
		str[start] = str[end];
		str[end] = tmp;

		start++;
		end--;
	}
}
char* itoa(int num, char *str) {
	int i = 0;
	int negative = 0;

	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	if (num < 0 ) {
		negative = 1;
		num = -num;
	}

	while (num != 0) {
		int rem = num % 10;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / 10;
	}

	if (negative)
		str[i++] = '-';

	str[i] = '\0';

	reverse(str, i);
	return str;
}
int atoi(char* str) {
    int result = 0;
    int sign = 1;
    int digit_index = 0;
    if (str[0] == '-') {
        sign = -1;
        digit_index++;
    }
    for (; str[digit_index] != '\0'; ++digit_index)
        result = result * 10 + str[digit_index] - '0';

    return sign * result;
    }
