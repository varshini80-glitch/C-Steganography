 #ifndef ENCODE_H
#define ENCODE_H

#include <stdio.h>
#include "types.h"

typedef struct
{
    char *src_image_fname;
    char *secret_fname;
    char *stego_image_fname;

    FILE *fptr_src_image;
    FILE *fptr_secret;
    FILE *fptr_stego_image;

    uint image_capacity;
    long size_secret_file;

    char extn_secret_file[50];

} EncodeInfo;


/* Function prototypes */

uint get_image_size_for_bmp(FILE *fptr_image);

Status open_files(EncodeInfo *encInfo);

Status read_and_validate_encode_args(
    char *argv[],
    EncodeInfo *encInfo
);

Status do_encoding(EncodeInfo *encInfo);

Status check_capacity(EncodeInfo *encInfo);

uint get_file_size(FILE *fptr);

Status copy_bmp_header(
    FILE *fptr_src_image,
    FILE *fptr_dest_image
);

Status encode_magic_string(
    const char *magic_string,
    EncodeInfo *encInfo
);

Status encode_byte_to_lsb(
    char data,
    char *image_buffer
);

Status encode_secret_file_extn_size(
    int size,
    EncodeInfo *encInfo
);

Status encode_size_to_lsb(
    int size,
    char *image_buffer
);

Status encode_secret_file_extn(
    const char *file_extn,
    EncodeInfo *encInfo
);

Status encode_secret_file_size(
    long file_size,
    EncodeInfo *encInfo
);

Status encode_secret_file_data(
    EncodeInfo *encInfo
);

Status copy_remaining_img_data(
    FILE *fptr_src,
    FILE *fptr_dest
);

#endif