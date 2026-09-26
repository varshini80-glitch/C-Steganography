#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"

typedef struct
{
    char *stego_image_fname;
    char *output_fname;

    FILE *fptr_stego_image;
    FILE *fptr_output;

    char extn_secret_file[10];
    int size_secret_file;

} DecodeInfo;

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);
Status do_decoding(DecodeInfo *decInfo);

Status decode_byte_from_lsb(char *image_buffer, char *data);
Status decode_size_from_lsb(char *image_buffer, int *size);
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);
Status decode_secret_file_extn_size(DecodeInfo *decInfo);
Status decode_secret_file_extn(DecodeInfo *decInfo);
Status decode_secret_file_size(DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *decInfo);
Status open_decode_files(DecodeInfo *decInfo);

#endif
