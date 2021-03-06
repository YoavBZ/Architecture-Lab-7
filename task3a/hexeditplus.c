#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


#define BUFFER_SIZE 100
int unit_size = 1;
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
        printf("Setting unit size to: %d\n", unit_size);
    } else {
        printf("Illegal unit size: %d\n", size);
    }
}

void quit(){
    exit(0);
}

void print_memory(char *header, char *format, int length, char *memory){
    printf("%s", header);
    for (int i = 0; i < unit_size * length; i += unit_size){
        for(int j = unit_size - 1; j >= 0; j--){
            printf(format, memory[i + j]);   
        }
        printf(" ");
    }
    printf("\n");
}

void file_display(){
    if (NULL == filename){
        printf("Filename is null!\n");
        return;
    }
    int fd = open(filename, O_RDONLY);
    if (0 >= fd)
    {
        printf("Cannot open file: %s\n", filename);
        return;
    }
    printf("Please enter <location> <length>:\n");

    int location, length;
    char input[BUFFER_SIZE];

    getchar();
    fgets(input, BUFFER_SIZE, stdin);
    sscanf(input,"%x %d",&location, &length);

    char *memory = malloc(unit_size * length);

    if (0 > lseek(fd, location, SEEK_SET)){
        printf("Cannot seek file: %s, to location: %d\n", filename, location);
        close(fd);
        free(memory);
        return;
    }

    if (0 > read(fd, memory, unit_size * length)){
        printf("Cannot read file: %s\n", filename);
        close(fd);
        free(memory);
        return;
    }
    close(fd);

    // Printing hex
    print_memory("Hexadecimal Representation:\n", "%02hhx", length, memory);
    // Printing dec
    print_memory("Decimal Representation:\n", "%hhu", length, memory);

    free(memory);
}

void file_modify(){
    if (NULL == filename){
        printf("Filename is null!\n");
        return;
    }
    int fd = open(filename, O_RDWR);
    if (0 >= fd)
    {
        printf("Cannot open file: %s\n", filename);
        return;
    }
    printf("Please enter <location> <val>:\n");

    int location, val;
    char input[BUFFER_SIZE];

    getchar();
    fgets(input, BUFFER_SIZE, stdin);
    sscanf(input,"%x %x",&location, &val);

    if (0 > lseek(fd, location, SEEK_SET)){
        printf("Cannot seek file: %s, to location: %d\n", filename, location);
        close(fd);
        return;
    }

    // Modifying
    if (0 > write(fd, &val, unit_size)){
        printf("Cannot write to file: %s\n", filename);
        close(fd);
        return;
    }
    close(fd);
}

void copy_from_file(){
    printf("Please enter <src_file> <src_offset> <dst_offset> <length>:\n");
    
    char input[BUFFER_SIZE];
    char src_file[BUFFER_SIZE];
    int src_offset, dst_offset, length;

    getchar();
    fgets(input, BUFFER_SIZE, stdin);
    if (EOF == sscanf(input, "%s %x %x %d", src_file, &src_offset, &dst_offset, &length)){
        printf("Illegal arguments\n");
        return;
    }

    if (NULL == src_file || NULL == filename){
        printf("Illegal files entered\n");
        return;
    }

    int src_fd = open(src_file, O_RDONLY);
    if (0 >= src_fd){
        printf("Cannot open source file: %s\n", src_file);
        return;
    }
    
    int dst_fd = open(filename, O_RDWR);
    if (0 >= dst_fd){
        printf("Cannot open destination file: %s\n", src_file);
        close(src_fd);
        return;
    }

    if (0 > lseek(dst_fd, dst_offset, SEEK_SET)){
        printf("Cannot seek file: %s, to location: %d\n", filename, dst_offset);
        close(dst_fd);
        close(src_fd);
        return;
    }
    if (0 > lseek(src_fd, src_offset, SEEK_SET)){
        printf("Cannot seek file: %s, to location: %d\n", src_file, dst_offset);
        close(dst_fd);
        close(src_fd);
        return;
    }

    // Reading from source
    char *memory = malloc(unit_size * length);
    if (0 > read(src_fd, memory, unit_size * length)){
        printf("Cannot read file: %s\n", src_file);
        close(dst_fd);
        close(src_fd);
        free(memory);
        return;
    }

    // Writing to destination
    if (0 > write(dst_fd, memory, unit_size * length)){
        printf("Cannot write to file: %s\n", filename);
        close(dst_fd);
        close(src_fd);
        free(memory);
        return;
    }
    printf("Copied %d bytes into FROM %s at %x TO %s at %x\n", unit_size * length, src_file, src_offset, filename, dst_offset);
    free(memory);
}

typedef struct Option {
    char *name;
    void (*function)();
} Option;

int main(int argc, char **argv){
    Option menu[7] = {{"1-Set File Name\n", &set_file_name}, {"2-Set Unit Size\n", &set_unit_size},
                        {"3-File Display\n", &file_display}, {"4-File Modify\n", &file_modify},
                        {"5-Copy From File\n", &copy_from_file}, {"6-Quit\n", &quit}, {NULL, NULL}};
    while (1){
        for (int i = 0; i < 6; i++){
            printf("%s", menu[i].name);
        }
        int selected;
        scanf("%d", &selected);
        if (0 < selected && 7 > selected)
            menu[selected-1].function();
    }
    return 0;
}
