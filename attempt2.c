#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* These are external references to the symbols created by OBJCOPY */
extern char _binary_test_txt_start[];
extern char _binary_test_txt_end[];
extern char _binary_test_txt_size[];

int main()
{
    char *data_start     = _binary_test_txt_start;
    char *data_end       = _binary_test_txt_end;
    size_t data_size  = (size_t)_binary_test_txt_size;

    /* Print out the pointers and size */
    printf ("data_start %p\n", data_start);
    printf ("data_end   %p\n", data_end);
    printf ("data_size  %zu\n", data_size);

    /* Print out each byte until we reach the end */
    //while (data_start < data_end) {
        //system("~/sandbox/perl6_2/bin/perl6 -e \"say 'Hello world!'\"");
    //    printf("%s\n", data_start++);
    //}

    //char command[50];
    //strcpy("~/sandbox/perl6_2/bin/perl6 -v", command);
    //printf("%s", *command);
    char* command = calloc(100,sizeof(char));
    sprintf(command, "~/sandbox/perl6_2/bin/perl6 -e '%s'", data_start);
    system(command);

    return 0;
}