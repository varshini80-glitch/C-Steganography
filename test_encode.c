#include <stdio.h>
#include "encode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;

    //-> call check_operation_type(argv[1][1]) == e_encode
    if (check_operation_type(argv[1][1]) == e_encode)
    {
        /*
            ->call read_and_validate_encode_args(argv, &encInfo) == e_success
                => call do_encoding(&encInfo)
                    print "Encoding is success"
        */

        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            if (do_encoding(&encInfo) == e_success)
            {
                printf("Encoding is success\n");
            }
        }
    }

    return 0;
}

OperationType check_operation_type(char opt)
{
    /*
        *check opt is 'e'
            return e_encode;
        *check opt is 'd'
            return e_decode;
        *else
            return e_unsupported;
    */

    if (opt == 'e')
    {
        return e_encode;
    }
    else if (opt == 'd')
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
