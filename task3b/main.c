#include <stdio.h>

int digit_ctn(char *str){
    int count = 0;
    while (*str){
        if ('0' <= *str && '9' >= *str){
            count++;
        }
        str++;
    }
    return count;
}

int main(int argc, char **argv)
{
    printf("The number of digits in the string is: %d\n", digit_ctn(argv[1]));
    return 0;
}