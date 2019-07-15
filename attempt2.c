#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

/* These are external references to the symbols created by OBJCOPY */
extern char _binary_test_txt_start[];
extern char _binary_test_txt_end[];
//extern char _binary_test_txt_size[];

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Input is of format './a.out <perl6>'\n");
        exit(0);
    }

    char *data_start     = _binary_test_txt_start;
    char *data_end       = _binary_test_txt_end;
    size_t data_size  = _binary_test_txt_end - _binary_test_txt_start;

    char temp_name[] = "p6_XXXXXX";
    int temp = mkstemp(temp_name);
    FILE* sfd = fdopen(temp, "w+");
    //char temp_file_name[L_tmpnam + 1];
    //tmpnam(temp_file_name);
    fwrite(data_start, data_size, 1, sfd);

    char* command = calloc(strlen(temp_name) + strlen(argv[1]) + 2,sizeof(char));
    //sprintf(command, "%s -b '%s'", argv[1], data_start);
    sprintf(command, "%s %s", argv[1], temp_name);
    printf("%s\n", command);
    int err = system(command);
    printf("%d\n", err);

    fclose(sfd);
    //remove(&temp_file_name);

    return 0;
}