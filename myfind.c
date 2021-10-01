#include "myfind.h"
#define IS_DIR 4 // dirent.h DT_DIR not recognized
#define MAX_PATH 100000

int flag_R = 0;
int flag_i = 0; 

void PrintUsage();
void Find(char* path, char** files, int fileCount); 

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

    if (argc<optind+2) { // Invalid number of arguments
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

    Find(path, files, numFiles);

    free(path); 
    free(*files); 
    return EXIT_SUCCESS; 
}

void PrintUsage(){
    printf("Invalid arguments, please use:\n\tmyfind [-R] [-i] <path> <file1> <file2> ... <fileN>\n"); 
}

void Find(char* path, char** files, int fileCount){
    char* nextPath = malloc(MAX_PATH);
    struct dirent *dirEntry; 
    DIR *dir; 

    if((dir = opendir(path)) == NULL){
        // fprintf(stderr, "Failed to open directory: %s\n", path);
        return; 
    } 
    while((dirEntry = readdir(dir)) != NULL){
        // only check files
        if(strcmp(dirEntry->d_name, ".") && strcmp(dirEntry->d_name, "..")){ 
            // Check if flag -R is set
            if(flag_R){
                if(dirEntry->d_type == IS_DIR){
                    strcpy(nextPath, path);
                    if(nextPath[strlen(nextPath)+1] != '/')
                        strcat(nextPath, "/");
                    strcat(nextPath, dirEntry->d_name); 
                    Find(nextPath, files, fileCount); // Recursively search in subfolders
                }
            }
            if(dirEntry->d_type != IS_DIR){ 
                for(int i = 0; i < fileCount; i++){
                    // Check if flag -i is set
                    if(flag_i ? (!strncasecmp(files[i], dirEntry->d_name, sizeof(files))) : (!strcmp(files[i], dirEntry->d_name))){
                        //Get path to found file
                        char cwd[MAX_PATH];
                        realpath(path, cwd);
                        printf("<pid>: %s: %s\n", dirEntry->d_name, cwd); 
                    }
                }
            }
        } 
    }
    closedir(dir);
    free(dirEntry); 
    free(nextPath); 
}