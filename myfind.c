#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#ifndef DT_DIR
#define DT_DIR 4 //Redefine DT_DIR, to avoid red underlines
#endif

int flag_R = 0;
int flag_i = 0; 

void PrintUsage();
void SimpleFind(char* path, char** files, int fileCount); 
void RecursiveFind(char* path, char** files, int fileCount); 

int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

int main(int argc, char*argv[]) {
    int arg; 
    char* path = malloc(argc* sizeof(char*)); //Path to Searchdirectory
    char** files = malloc(argc * sizeof(char*)); //Array of Filename strings

    while ((arg = getopt(argc, argv, "Ri")) != EOF){
        switch(arg){
            case 'R':
                if(flag_R){
                    PrintUsage();
                    break;
                } 
                flag_R++; 
            break;
            case 'i':
                if(flag_i){
                    PrintUsage();
                    break;
                } 
                flag_i++; 
            break; 
            default: assert(0);
        } 
    }

    // falsche Anzahl an Optionen 
    if (argc<optind+2) { 
        PrintUsage();
    }

    int numFiles = -1; 
    for(; optind < argc; optind++) {
        if(numFiles == -1){
            strcpy(path, argv[optind]); 
        }else{
            files[numFiles] = malloc(strlen(argv[optind])+1);
            strcpy(files[numFiles], argv[optind]);
        }
        numFiles++;  
    }

    if(flag_R)
        RecursiveFind(path, files, numFiles);
    else
        SimpleFind(path, files, numFiles);

    // // Output-Test
    // if(flag_i) printf("-i : case-insensitive search\n");
    // if(flag_R) printf("-R : recursive search\n"); 
    // printf("path: %s\n", path); 
    // for(int i = 0; i < numFiles; i++){
    //     printf("%d - %s\n", i, files[i]); 
    // }    

    free(path); 
    free(*files); 
    return EXIT_SUCCESS; 
}

void PrintUsage(){
    printf("Invalid arguments, please use:\n\tmyfind [-R] [-i] <path> <file1> <file2> ... <fileN>\n"); 
}

void SimpleFind(char* path, char** files, int fileCount){
    struct dirent *dirEntry; 
    DIR *dir; 

    if((dir = opendir(path)) == NULL){
        printf("Failed to open directory\n");
        return;
    } 

    while((dirEntry = readdir(dir)) != NULL){
        // only check files
        if(strcmp(dirEntry->d_name, ".") && strcmp(dirEntry->d_name, "..") && dirEntry->d_type != DT_DIR){  
            for(int i = 0; i < fileCount; i++){
                //Check if flag -i is set
                if(flag_i ? (!strncasecmp(files[i], dirEntry->d_name, sizeof(files))) : (!strcmp(files[i], dirEntry->d_name))){
                    //Change to current directory  
                    chdir(path); 
                    char cwd[255]; 
                    printf("<pid>: %s: %s\n", files[i], getcwd(cwd, 255)); 
                }
            }
        } 
    } 
    free(dir);
    free(dirEntry);
}

void RecursiveFind(char* path, char** files, int fileCount){
    
}