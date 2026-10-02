#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

OperationType check_operation_type(char opt);

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    OperationType operation;

    if(argc == 1)
    {
        printf("\n");
        printf("USAGE:\n");
        printf("  ./a.out -e <source.bmp> <secret.txt> [output.bmp]\n");
        printf("  ./a.out -d <stego.bmp> [output_file]\n");
        printf("\n");
        printf("OPTIONS:\n");
        printf("  -e    Encode secret file into BMP image\n");
        printf("  -d    Decode secret file from BMP image\n");
        printf("  -h    Display help menu\n");

        return 0;
    }

    if(strlen(argv[1]) < 2)
    {
        printf("ERROR: Invalid option\n");
        return 1;
    }

    operation = check_operation_type(argv[1][1]);

    if(operation == e_encode)
    {
        if(argc == 2 || argc == 3)
        {
            printf("\n");
            printf("ENCODING HELP MENU\n");
            printf("\n");
            printf("Usage:\n");
            printf("  ./a.out -e <source.bmp> <secret.txt> [output.bmp]\n");

            return 0;
        }

        if(argc > 5)
        {
            printf("ERROR: Too many arguments\n");
            return 1;
        }

        if(read_and_validate_encode_args(argv, &encInfo) == e_failure)
        {
            printf("ERROR: Read and validate failed\n");
            return 1;
        }

        if(do_encoding(&encInfo) == e_failure)
        {
            printf("ERROR: Encoding failed\n");
            return 1;
        }

        printf("INFO: Encoding successful\n");

        return 0;
    }

    if(operation == e_decode)
    {
        if(argc == 2)
        {
            printf("\n");
            printf("DECODING HELP MENU\n");
            printf("\n");
            printf("Usage:\n");
            printf("  ./a.out -d <stego.bmp> [output_file]\n");

            return 0;
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

    printf("\n");
    printf("ERROR: Invalid option\n");
    printf("\n");
    printf("USAGE:\n");
    printf("  ./a.out -e <source.bmp> <secret.txt> [output.bmp]\n");
    printf("  ./a.out -d <stego.bmp> [output_file]\n");
    printf("\n");
    printf("OPTIONS:\n");
    printf("  -e    Encode secret file into BMP image\n");
    printf("  -d    Decode secret file from BMP image\n");
    printf("  -h    Display help menu\n");

    return 1;
}

OperationType check_operation_type(char opt)
{
    if(opt == 'e')
    {
        return e_encode;
    }
    else if(opt == 'd')
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}