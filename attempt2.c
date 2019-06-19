#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* These are external references to the symbols created by OBJCOPY */
extern char _binary_test_txt_start[];
extern char _binary_test_txt_end[];
//extern char _binary_test_txt_size[];

int main()
{
    char *data_start     = _binary_test_txt_start;
    char *data_end       = _binary_test_txt_end;
    size_t data_size  = _binary_test_txt_end - _binary_test_txt_start;

    /* Print out the pointers and size */
    printf ("data_start %p\n", data_start);
    printf ("data_end   %p\n", data_end);
    printf ("data_size  %lx\n", data_size);

    char* command = calloc(data_size,sizeof(char));
    sprintf(command, "~/sandbox/perl6_2/bin/perl6 -e '%s'", data_start);
    int err = system(command);

    return 0;
}