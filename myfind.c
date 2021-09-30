#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

void PrintUsage();
void SimpleFind(char* path, char** files); 

int main(int argc, char*argv[]) {
    int arg; 
    int flag_R = 0;
    int flag_i = 0;  

    char* path = malloc(sizeof(char*)); //Path to Searchdirectory
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


    SimpleFind(path, files);

    // // Output-Test
    // if(flag_i) printf("-i : case-insensitive search\n");
    // if(flag_R) printf("-R : recursive search\n"); 
    // printf("path: %s\n", path); 
    // for(int i = 0; i < numFiles; i++){
    //     printf("%d - %s\n", i, files[i]); 
    // }    

    free(path); 
    free(files); 
    return EXIT_SUCCESS; 
}

void PrintUsage(){
    printf("Invalid arguments, please use:\n\tmyfind [-R] [-i] <path> <file1> <file2> ... <fileN>\n"); 
}

void SimpleFind(char* path, char** files){
    struct dirent *dirEntry; 
    DIR *dir;

    if((dir = opendir(path)) == NULL){
        printf("Failed to open directory\n");
        return;
    } 

    while((dirEntry = readdir(dir)) != NULL){
        //Ignore . and .. Directory
        if(strcmp(dirEntry->d_name, ".") && strcmp(dirEntry->d_name, "..")){  
            printf("%s\n", dirEntry->d_name); 
        }
    }
    while ((closedir(dir) == -1) && (errno == EINTR)) ;
}