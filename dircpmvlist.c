#define _XOPEN_SOURCE 500 //needed for nftw callbacks
#define _POSIX_C_SOURCE 1 //needed for PATH MAX
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>

void copyFile(const char *src, const char *dst, const char *ext[], int extCount)
{
    // Make the destination directory if it doesn't exist
    mkdir(dst, 0777);

    // opening a directory pointer to the root of the source and checking if the directory exists
    DIR *srcDir = opendir(src);
    if (srcDir == NULL)
    {
        printf("The source directory does not exist.\n");
        return;
    }

    // if the directory exists then opening a file entry to store the directory stream
    struct dirent *fileEntry;

    //as long as a file is read in the source that matches an entry in the fileEntry stream
    while ((fileEntry = readdir(srcDir)) != NULL)
    {
        //skipping the current and parent directory to avoid infitine loops
        if (strcmp(fileEntry->d_name, ".") == 0 || strcmp(fileEntry->d_name, "..") == 0)
        {
            continue;
        }

        //creating the source and destination file paths
        char srcFilepath[PATH_MAX];
        snprintf(srcFilepath, PATH_MAX, "%s/%s", src, fileEntry->d_name);

        char destFilepath[PATH_MAX];
        snprintf(destFilepath, PATH_MAX, "%s/%s", dst, fileEntry->d_name);

        //reading the source file status and saving the status info into a buffer
        struct stat statBuf;
        if (lstat(srcFilepath, &statBuf) == -1)
        {
            printf("%s failure to receive file status\n", srcFilepath);
            continue;
        }

        //checking to see if the file is a directory from the status buffer information
        if (S_ISDIR(statBuf.st_mode))
        {
            //if directory then recursively calling the same function
            copyFile(srcFilepath, destFilepath, ext, extCount);
        }
        else
        {
            int cpFile = 0;

            // if extensions provided then perform filtering
            if (extCount > 0)
            {
                //extracting the file extension
                char *fileExtension = strrchr(fileEntry->d_name, '.');
                if (fileExtension != NULL)
                {
                    int i;
                    for (i = 0; i < extCount; i++)
                    {
                        //comparing extracted extension with each provided extension
                        if (strcmp(fileExtension, ext[i]) == 0)
                        {
                            //if extensions match then set the copy flag to 1 and break out
                            cpFile = 1;
                            break;
                        }
                    }
                }
            }
            else
            {
                //if no extensions provided then set copy flag to 1 so that all files can be copied
                cpFile = 1;
            }

            //if no extension provided then copy the file without checking
            if (cpFile)
            {
                //opening the source file in binary read mode
                FILE *srcFile = fopen(srcFilepath, "rb");
                if (srcFile == NULL)
                {
                    printf("Failed to open source file: %s\n", srcFilepath);
                    continue;
                }

                // opening the destination file in binary write mode
                FILE *destFile = fopen(destFilepath, "wb");
                if (destFile == NULL)
                {
                    printf("Failed to create destination file: %s\n", destFilepath);
                    fclose(srcFile);
                    continue;
                }

                //setting the buffer size
                char buffer[4096];
                size_t size;

                //reading the contents of the source file in buffer size chunks to the destination file in buffer sized chunks
                while ((size = fread(buffer, 1, sizeof(buffer), srcFile)) > 0)
                {
                    fwrite(buffer, 1, size, destFile);
                }
                fclose(srcFile);
                fclose(destFile);
            }
        }
    }
    closedir(srcDir);
}

int removeFiles(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    //if pathname removal is not successful
    if(remove(pathname) < 0)
    {
        perror("ERROR: remove");
        return -1;
    }
    return 0;
}

void moveFile(const char *src, const char *dst, const char *ext[], int extCount)
{
    // Make the destination directory if it doesn't exist
    mkdir(dst, 0777);

    // opening a directory pointer to the root of the source and checking if the directory exists
    DIR *srcDir = opendir(src);
    if (srcDir == NULL)
    {
        printf("The source directory does not exist.\n");
        return;
    }

    // if the directory exists then opening a file entry to store the directory stream
    struct dirent *fileEntry;

    //as long as a file is read in the source that matches an entry in the fileEntry stream
    while ((fileEntry = readdir(srcDir)) != NULL)
    {
        //skipping the current and parent directory to avoid infitine loops
        if (strcmp(fileEntry->d_name, ".") == 0 || strcmp(fileEntry->d_name, "..") == 0)
        {
            continue;
        }

        //creating the source and destination file paths
        char srcFilepath[PATH_MAX];
        snprintf(srcFilepath, PATH_MAX, "%s/%s", src, fileEntry->d_name);

        char destFilepath[PATH_MAX];
        snprintf(destFilepath, PATH_MAX, "%s/%s", dst, fileEntry->d_name);

        //reading the source file status and saving the status info into a buffer
        struct stat statBuf;
        if (lstat(srcFilepath, &statBuf) == -1)
        {
            printf("%s failure to receive file status\n", srcFilepath);
            continue;
        }

        //checking to see if the file is a directory from the status buffer information
        if (S_ISDIR(statBuf.st_mode))
        {
            //if directory then recursively calling the same function
            moveFile(srcFilepath, destFilepath, ext, extCount);
        }
        else
        {
            int mvFile = 0;

            // if extensions provided then perform filtering
            if (extCount > 0)
            {
                //extracting the file extension
                char *fileExtension = strrchr(fileEntry->d_name, '.');
                if (fileExtension != NULL)
                {
                    int i;
                    for (i = 0; i < extCount; i++)
                    {
                        //comparing extracted extension with each provided extension
                        if (strcmp(fileExtension, ext[i]) == 0)
                        {
                            //if extensions match then set the move flag to 1 and break out
                            mvFile = 1;
                            break;
                        }
                    }
                }
            }
            else
            {
                //if no extensions provided then set move flag to 1 so that all files can be moved
                mvFile = 1;
            }

            //if no extension provided then move the file without checking
            if (mvFile)
            {
                //opening the source file in binary read mode
                FILE *srcFile = fopen(srcFilepath, "rb");
                if (srcFile == NULL)
                {
                    printf("Failed to open source file: %s\n", srcFilepath);
                    continue;
                }

                // opening the destination file in binary write mode
                FILE *destFile = fopen(destFilepath, "wb");
                if (destFile == NULL)
                {
                    printf("Failed to create destination file: %s\n", destFilepath);
                    fclose(srcFile);
                    continue;
                }

                //setting the buffer size
                char buffer[4096];
                size_t size;

                //reading the contents of the source file in buffer size chunks to the destination file in buffer sized chunks
                while ((size = fread(buffer, 1, sizeof(buffer), srcFile)) > 0)
                {
                    fwrite(buffer, 1, size, destFile);
                }
                fclose(srcFile);
                fclose(destFile);

                // if extensions are provided then this if is invoked
                if (extCount > 0)
                {
                    // ............................................................
                    // Delete the directory and its contents by traversing the tree in reverse order

                    if (nftw(srcFilepath, removeFiles,10, FTW_DEPTH|FTW_MOUNT|FTW_PHYS) < 0)
                    {
                        perror("ERROR: ntfw");
                        exit(1);
                    }
                    //..................................................................
                }

            }
        }

        //this works when no extension passed
        if (!(extCount> 0))
        {
            // ............................................................
            // Delete the directory and its contents by traversing the tree in reverse order

            if (nftw(srcFilepath, removeFiles,10, FTW_DEPTH|FTW_MOUNT|FTW_PHYS) < 0)
            {
                perror("ERROR: ntfw");
                exit(1);
            }
            //..................................................................
        }



    }
    closedir(srcDir);


}

//here argv is a pointer array that points to each argument passed to the program
// argc refers to the number of arguments passed
int main(int argc, char *argv[])
{
    //if less than 4 arguments passed then we are missing arguments
    if (argc < 4)
    {
        printf("Not Enough Arguments Provided.\n");
        return 1;
    }


    //if more than 10 arguments passed then we are over the limit for max arguments
    if (argc > 10)
    {
        printf("Too Many Arguments Provided.\n");
        return 1;
    }

    // saving the provided source/destination directories and option parameters into respective pointer variables
    const char *srcDir = argv[1];
    const char *destDir = argv[2];
    const char *option = argv[3];

    //initializing copy and move params
    int copy = 0;
    int move = 0;

    // if provided option is copy then enable copy flag
    if (strcmp(option, "-cp") == 0)
    {
        copy = 1;
    }
    else if (strcmp(option, "-mv") == 0) // if provided option is move then enable move flag
    {
        move = 1;
    }
    else
    {
        printf("Invalid option entered. Please pass -cp for copy or -mv for move.\n");
        return 1;
    }

    //extracting the passed extension counts after -cp/-mv command
    int extCount = argc - 4;

    const char *ext[6];
    int i;
    //extracting the extensions and moving them into a separate array
    for (i = 4; i < argc; i++)
    {
        ext[i - 4] = argv[i];
    }

    //perform operations as per options provided
    if (copy)
    {
        copyFile(srcDir, destDir, ext, extCount);
        printf("Copy Operation Performed Successfully.\n");

    }
    else if (move)
    {
        moveFile(srcDir, destDir, ext, extCount);
        printf("Move Operation Performed Successfully.\n");
    }
    return 0;
}

// Submitted by Ali Iktider
// Student ID: 110122251
