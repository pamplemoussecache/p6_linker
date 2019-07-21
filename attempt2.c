#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

/* These are external references to the symbols created by OBJCOPY */
extern char _binary_test_txt_start[];
extern char _binary_test_txt_end[];
//extern char _binary_test_txt_size[];

int err;

int main(int argc, char* argv[])
{
    // if (argc != 2) {
    //     printf("Input is of format './a.out <perl6>'\n");
    //     exit(0);
    // }

    char *data_start     = _binary_test_txt_start;
    char *data_end       = _binary_test_txt_end;
    size_t data_size  = _binary_test_txt_end - _binary_test_txt_start;

    char temp_name[] = "p6_XXXXXX";
    int temp = mkstemp(temp_name);
    FILE* sfd = fdopen(temp, "wb");
    fwrite(data_start, data_size, 1, sfd);
    fclose(sfd);


    char* command = calloc(39 + strlen(temp_name), sizeof(char));
    sprintf(command, "perl_path=`which perl6`; $perl_path -b %s", temp_name);
    err = system(command);

    free(command);
    remove(temp_name);
    remove(temp_name+3);

    return 0;
}