#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 100
int unit_size;
char filename[BUFFER_SIZE];

void set_file_name(){
    printf("Please enter a filename:\n");
    scanf("%s", filename);
}

void set_unit_size(){
    int size;
    printf("Please enter unit size (1/2/4):\n");
    scanf("%d", &size);
    if (1 == size || 2 == size || 4 == size) {
        unit_size = size;
    } else {
        printf("Illegal unit size: %d\n", size);
    }
}

void quit(){
    exit(0);
}

typedef struct Option {
    char *name;
    void (*function)();
} Option;

int main(int argc, char **argv)
{
    Option menu[4] = {{"1-Set File Name\n", &set_file_name}, {"2-Set Unit Size\n", &set_unit_size}, {"3-Quit\n", &quit}, {NULL, NULL}};
    while (1){
        for (int i = 0; i < 3; i++){
            printf("%s", menu[i].name);
        }
        int selected;
        scanf("%d", &selected);
        if (0 < selected && 4 > selected)
            menu[selected-1].function();
    }
    return 0;
}
