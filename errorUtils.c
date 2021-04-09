#include "errorUtils.h"

void
errorExit(int retVal,const char* msg)
{
    if( retVal == -1)
    {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

void
usageErr()
{
    fprintf(stderr,"Usage: (./)merge [OPTIONS] dest_file\nOptions:\n\tn - Number of files to merge\n");
    exit(EXIT_FAILURE);
}