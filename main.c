#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>
#include "errorUtils.h"

static char buffer[BUFSIZ];
#define BUF_SIZE        1024
#define EXTENSION_LEN   4





void
openSrcFiles(char* baseName , int * src_fds, int filesNum);

void 
closeSrcFiles(int * src_fds , int filesNum);

int main(int argc , char* argv[] , char* environp[])
{
    int dest_fd ,*src_fds, opt ,filesNum = 0 ;
    ssize_t rbytes , wbytes;

    if(argc < 4)
        usageErr();

    while( (opt = getopt(argc , argv , "n:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            filesNum = (size_t) atol(optarg);
            break;
        
        default:
            usageErr();
            break;
        }
    }
    
    src_fds = (int*)malloc(filesNum * sizeof(int) );
    errorExit(src_fds == NULL ? FAILED : SUCCESS , "malloc(filesNum)");


    dest_fd = open(argv[optind] , O_CREAT | O_TRUNC | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    errorExit(dest_fd , "open(dest_fd):");

    openSrcFiles(argv[optind] , src_fds , filesNum);                   //Open all the filesNum destination files
    

    for(int i=0 ; i < filesNum ; ++i)
    {
        while( (rbytes = read(src_fds[i] , buffer , BUFSIZ)) )
        {
            errorExit(rbytes , "read():");

            wbytes = write(dest_fd , buffer , rbytes);
            errorExit(wbytes != rbytes ? FAILED : SUCCESS , "write():");
        }
    } 
    
    printf("All %d files merged successfully\n",filesNum);

    //House-keeping
    errorExit( close( dest_fd ) , "close(dest_fd):");
    closeSrcFiles(src_fds,filesNum);
    free(src_fds);
    
    return EXIT_SUCCESS;
}

void
openSrcFiles(char* baseName, int * src_fds, int filesNum)
{
    
    char buf[BUF_SIZE];
    sprintf(buf,"%d",filesNum);             //Convert the int filesNum to a string and store it in buf

    int baseNameLen = strlen(baseName) , fileDigits = strlen(buf) ,fd;

    char* name = (char*)malloc( baseNameLen + fileDigits + EXTENSION_LEN + 1); //Holds the string that will represent a particular filename
    errorExit(name == NULL ? FAILED : SUCCESS , "malloc(strlen(baseName) + strlen(buf)):");

    strcpy(name,baseName);

    for(int i = 0 ; i < filesNum ; ++i)
    {
        sprintf(buf,"%d",i) ;               //Convert index i to a string

        //The two function calls below construct the file name
        strcat( name , buf) ;
        strcat( name , ".prt");

        fd = open(name , O_RDONLY );
        errorExit(fd,"open(.prt):");

        *( src_fds + i ) = fd;

        //Reset the constructed name to its initial state i.e. a '\0' following the baseName
        name[baseNameLen] = '\0';
    }

    free(name);
}

void 
closeSrcFiles(int * src_fds , int filesNum)
{
    for (int i = 0; i < filesNum; i++)
        errorExit( close(src_fds[i]) , "close(source files):");
}