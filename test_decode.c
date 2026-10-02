#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    DecodeInfo decInfo;

    if(argc == 1)
    {
        printf("\n");
        printf("DECODING HELP MENU\n");
        printf("\n");
        printf("Usage:\n");
        printf("  ./decode -d <stego.bmp> [output_file]\n");

        return 0;
    }

    if(strcmp(argv[1], "-d") != 0)
    {
        printf("ERROR: Invalid option\n");
        return 1;
    }

    if(argc > 4)
    {
        printf("ERROR: Too many arguments\n");
        return 1;
    }

    if(read_and_validate_decode_args(argv, &decInfo) == e_failure)
    {
        printf("ERROR: Read and validate failed\n");
        return 1;
    }

    if(do_decoding(&decInfo) == e_failure)
    {
        printf("ERROR: Decoding failed\n");
        return 1;
    }

    printf("INFO: Decoding successful\n");

    return 0;
}