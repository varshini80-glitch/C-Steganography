#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;

    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);

    // Return image capacity
    return width * height * 3;
}


/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");

    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",
                encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");

    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",
                encInfo->secret_fname);

        fclose(encInfo->fptr_src_image);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");

    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",
                encInfo->stego_image_fname);

        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}


Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    /*
        -> check argv[2] have ".bmp" as last 4 char
            * If not, print error msg, return e_failure
        encInfo -> src_image_fname = argv[2]

        encInfo -> secret_fname = argv[3]

        -> check argv[4] == NULL
        encInfo -> stego_image_fname = "output.bmp"
        -> else
            * validate argv[4] is ".bmp"
                    => if not, print error msg, return e_failure
            * encInfo -> stego_image_fname = argv[4]

        -> call open_file(encInfo) == e_failure
            return e_failure
        return e_success
    */

    int len;

    /* Check source image */
    if (argv[2] == NULL)
    {
        printf("ERROR: Source image not provided\n");
        return e_failure;
    }

    len = strlen(argv[2]);

    if (len < 4 || strcmp(argv[2] + len - 4, ".bmp") != 0)
    {
        printf("ERROR: Source image should be a .bmp file\n");
        return e_failure;
    }

    encInfo->src_image_fname = argv[2];

    /* Check secret file */
    if (argv[3] == NULL)
    {
        printf("ERROR: Secret file not provided\n");
        return e_failure;
    }

    encInfo->secret_fname = argv[3];

    /* Check output file */
    if (argv[4] == NULL)
    {
        encInfo->stego_image_fname = "output.bmp";
    }
    else
    {
        len = strlen(argv[4]);

        if (len < 4 || strcmp(argv[4] + len - 4, ".bmp") != 0)
        {
            printf("ERROR: Output image should be a .bmp file\n");
            return e_failure;
        }

        encInfo->stego_image_fname = argv[4];
    }

    // Call open_file
    if (open_files(encInfo) == e_failure)
    {
        return e_failure;
    }

    return e_success;
}


Status do_encoding(EncodeInfo *encInfo)
{
    /*
        //call check_capacity(encInfo) == e_failure
            print error msg, return e_failure

        // call copy_bmp_header(fptr_src_file, fptr_dest_file) == e_failure
            print error msg, return e_failure

        // call encode_magic_string(MAGIC_STRING, encInfo) == e_failure
            print error msg, return e_failure

        // call encode_secret_file_extn(encInfo) == e_failure
            print error msg, return e_failure

        // call encode_secret_file_size(extn_size_secret_file, encInfo) == e_failure
            print error msg, return e_failure

        // call encode_secret_file_extn(size_secret_file, encInfo) == e_failure
            print error msg, return e_failure

        // call encode_secret_file_data(encInfo) == e_failure
            print error, return e_failure

        // call copy_remaining_img_data(fptr_src_file, fptr_dest_file) == e_failure
            print error, return e_failure

        return e_success;
    */

    if (check_capacity(encInfo) == e_failure)
    {
        printf("ERROR: Insufficient image capacity\n");
        return e_failure;
    }

    if (copy_bmp_header(encInfo->fptr_src_image,
                        encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: Unable to copy BMP header\n");
        return e_failure;
    }

    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        printf("ERROR: Unable to encode magic string\n");
        return e_failure;
    }

    if (encode_secret_file_extn(encInfo->extn_secret_file,
                                encInfo) == e_failure)
    {
        printf("ERROR: Unable to encode secret file extension\n");
        return e_failure;
    }

    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),
                                      encInfo) == e_failure)
    {
        printf("ERROR: Unable to encode extension size\n");
        return e_failure;
    }

    if (encode_secret_file_size(encInfo->size_secret_file,
                                encInfo) == e_failure)
    {
        printf("ERROR: Unable to encode secret file size\n");
        return e_failure;
    }

    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("ERROR: Unable to encode secret file data\n");
        return e_failure;
    }

    if (copy_remaining_img_data(encInfo->fptr_src_image,
                                encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: Unable to copy remaining image data\n");
        return e_failure;
    }

    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);

    return e_success;
}


Status check_capacity(EncodeInfo *encInfo)
{
    /*
        -> call get_image_size_for_bmp(encode -> fptr_src_image)
            image_capacity = get_image_size()

        -> call get_file_size(encode -> fptr_src_image)
            size_secret_file = get_file_size()

        ->check ((14 + size_secret_file) * 8) > image_capacity
            return e_failure

        ->return e_success
    */

    int total_size;
    char *dot;

    encInfo->image_capacity =
        get_image_size_for_bmp(encInfo->fptr_src_image);

    encInfo->size_secret_file =
        get_file_size(encInfo->fptr_secret);

    dot = strrchr(encInfo->secret_fname, '.');

    if (dot != NULL)
    {
        strcpy(encInfo->extn_secret_file, dot);
    }
    else
    {
        encInfo->extn_secret_file[0] = '\0';
    }

    total_size = strlen(MAGIC_STRING) +
                 4 +
                 strlen(encInfo->extn_secret_file) +
                 4 +
                 encInfo->size_secret_file;

    if ((total_size * 8) > encInfo->image_capacity)
    {
        return e_failure;
    }

    return e_success;
}


uint get_file_size(FILE *fptr)
{
    /*
        -> move the offset to  last pos
        -> return ftell()
    */

    long size;

    fseek(fptr, 0, SEEK_END);

    size = ftell(fptr);

    rewind(fptr);

    return size;
}


Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    /*
        -> move the file pointers to the SEEK_SET
        -> declare the buff[54]
        -> read 54 bytes from src file
        -> write 45 bytes to dest file
    */

    char buff[54];

    // Move file pointers to beginning
    fseek(fptr_src_image, 0, SEEK_SET);
    fseek(fptr_dest_image, 0, SEEK_SET);

    // Read 54 bytes from source file
    if (fread(buff, 54, 1, fptr_src_image) != 1)
    {
        return e_failure;
    }

    // Write 54 bytes to destination file
    if (fwrite(buff, 54, 1, fptr_dest_image) != 1)
    {
        return e_failure;
    }

    return e_success;
}


Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    /*
        declare a buff of 8 bytes
        -> Loop for (length of magic_string)2 times
        read 8 bytes from the src_file into buff
        encode_byte_to_lsb(magic_string[1], buff)
        write the encoded buff to output_file

        ->return e_success
    */

    char buff[8];
    int i;

    for (i = 0; i < strlen(magic_string); i++)
    {
        // Read 8 bytes from source image
        if (fread(buff, 8, 1, encInfo->fptr_src_image) != 1)
        {
            return e_failure;
        }

        // Encode one character
        encode_byte_to_lsb(magic_string[i], buff);

        // Write encoded data
        if (fwrite(buff, 8, 1, encInfo->fptr_stego_image) != 1)
        {
            return e_failure;
        }
    }

    return e_success;
}


Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /*
        for(int i=7; i>=0; i--)
        {
            -> get the ith bit is or not
                => if set, set the LSB of image_buffer[]
                => if clear, clear the LSB of image_buffer[]

        }
    */

    int i;

    for (i = 7; i >= 0; i--)
    {
        if ((data >> i) & 1)
        {
            image_buffer[7 - i] =
                image_buffer[7 - i] | 1;
        }
        else
        {
            image_buffer[7 - i] =
                image_buffer[7 - i] & 0xFE;
        }
    }

    return e_success;
}


/*
    This function encodes the size of the secret file extension
*/

Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    char buff[32];

    /*
        -> Declare a buff[32]

        -> read 32 bytes from src_file into buff
        -> call encode_size_to_lsb(size, buff)
        -> Write 32 bytes of buff to output_file

        return e_success
    */

    if (fread(buff, 32, 1, encInfo->fptr_src_image) != 1)
    {
        return e_failure;
    }

    encode_size_to_lsb(size, buff);

    if (fwrite(buff, 32, 1, encInfo->fptr_stego_image) != 1)
    {
        return e_failure;
    }

    return e_success;
}


Status encode_size_to_lsb(int size, char *Image_buff)
{
    /*
        for(int i=31; i>=0; i--)
        {
            -> get the ith bit is or not
                => if set, set the LSB of image_buffer[]
                => if clear, clear the LSB of image_buffer[]

        }

        return e_success;
    */

    int i;

    for (i = 31; i >= 0; i--)
    {
        if ((size >> i) & 1)
        {
            Image_buff[31 - i] =
                Image_buff[31 - i] | 1;
        }
        else
        {
            Image_buff[31 - i] =
                Image_buff[31 - i] & 0xFE;
        }
    }

    return e_success;
}


Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    /*
        declare buff[8]

        => loop till EOF of secret_file
            ->Read 8 bytes from srv_image
            ->encode_byte_to_lsb(file_extn[], buff)
            -> Write the 8 bytes of buff to output_file

            return e_success;
    */

    char buff[8];
    int i;

    for (i = 0; file_extn[i] != '\0'; i++)
    {
        // Read 8 bytes from source image
        if (fread(buff, 8, 1, encInfo->fptr_src_image) != 1)
        {
            return e_failure;
        }

        // Encode extension character
        encode_byte_to_lsb(file_extn[i], buff);

        // Write encoded data
        if (fwrite(buff, 8, 1, encInfo->fptr_stego_image) != 1)
        {
            return e_failure;
        }
    }

    return e_success;
}


Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    /*
        -> Declare the buff[32]

        -> Read the 32 bytes from scr_image
        -> encode_size_to_lsb(file_size, buff)
        -> Write the 32 bytes to output_file

        return e_success;
    */

    char buff[32];

    if (fread(buff, 32, 1, encInfo->fptr_src_image) != 1)
    {
        return e_failure;
    }

    encode_size_to_lsb((int)file_size, buff);

    if (fwrite(buff, 32, 1, encInfo->fptr_stego_image) != 1)
    {
        return e_failure;
    }

    return e_success;
}


Status encode_secret_file_data(EncodeInfo *encInfo)
{
    /*
        Declare buff [8]

        => loop till EOF of secret_file
            -> Read the 8 bytes from scr_image
            -> Read the 1 bytes from secret_file
            -> encode_byte_to_lsb(data, buff)

        return e_success;
    */

    char buff[8];
    char data;

    while (fread(&data, 1, 1, encInfo->fptr_secret) == 1)
    {
        // Read 8 bytes from source image
        if (fread(buff, 8, 1, encInfo->fptr_src_image) != 1)
        {
            return e_failure;
        }

        // Encode one secret byte
        encode_byte_to_lsb(data, buff);

        // Write encoded data
        if (fwrite(buff, 8, 1, encInfo->fptr_stego_image) != 1)
        {
            return e_failure;
        }
    }

    return e_success;
}


Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    /*
        Declare a char as data

        -> Read a char from src_file
        -> Write the data to dest_file

        return e_success
    */

    char data;

    while (fread(&data, 1, 1, fptr_src) == 1)
    {
        fwrite(&data, 1, 1, fptr_dest);
    }

    return e_success;
}