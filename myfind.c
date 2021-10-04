#include "myfind.h"
#define IS_DIR 4 // dirent.h DT_DIR not recognized
#define STRING_MAX 4294967291

int flag_R = 0;
int flag_i = 0; 

void PrintUsage();
void Find(char* path, char* file); 

int main(int argc, char*argv[]) {
    int arg; 
    int status; 
    char* path = malloc(argc * sizeof(char*)); //Path to Searchdirectory

    while ((arg = getopt(argc, argv, "Ri")) != EOF){
        switch(arg){
            case 'R':
                if(flag_R){
                    PrintUsage();
                    exit(EXIT_FAILURE);
                } 
                flag_R++; 
            break;
            case 'i':
                if(flag_i){
                    PrintUsage();
                    exit(EXIT_FAILURE);
                } 
                flag_i++; 
            break; 
            default: exit(EXIT_FAILURE);
        } 
    }

    if (argc<optind+2) { // Invalid number of arguments
        PrintUsage();
    }

    strcpy(path, argv[optind]); // set path variable
    optind++;

    for(; optind < argc; optind++){
        pid_t pid; 
        switch(pid = fork()){
            case -1: 
                printf("Error, could not start child process.\n"); 
                exit(EXIT_FAILURE);
                break; 
            case 0: 
                Find(path, argv[optind]);
                return EXIT_SUCCESS;
                break; 
            default:
                break; 
        }
        if(pid){
            pid = wait(&status); // wait for other processes to finish
        }
    }

    free(path); 
    return EXIT_SUCCESS; 
}

void PrintUsage(){
    printf("Invalid arguments, please use:\n\tmyfind [-R] [-i] <path> <file1> <file2> ... <fileN>\n"); 
}

void Find(char* path, char* file){
    char* nextPath = malloc(STRING_MAX);
    struct dirent *dirEntry; 
    DIR *dir = opendir(path); 

    if(!dir){
        // fprintf(stderr, "%s: %s\n", path, strerror(errno));
        return; 
    } 
    while((dirEntry = readdir(dir))){
        if(strcmp(dirEntry->d_name, ".") && strcmp(dirEntry->d_name, "..")){ 
            // Check if flag -R is set
            if(flag_R){
                if(dirEntry->d_type == IS_DIR){
                    strcpy(nextPath, path);
                    if(nextPath[strlen(nextPath)+1] != '/')
                        strcat(nextPath, "/");
                    strcat(nextPath, dirEntry->d_name); 
                    Find(nextPath, file); // Recursively search in subfolders
                }
            }
            if(dirEntry->d_type != IS_DIR){ 
                // Check if flag -i is set
                if(flag_i ? (!strncasecmp(file, dirEntry->d_name, sizeof(char*))) : (!strcmp(file, dirEntry->d_name))){
                    //Get path to found file
                    char cwd[CHAR_MAX];
                    realpath(path, cwd);
                    printf("%d: %s: %s\n", getpid(), dirEntry->d_name, cwd); 
                }
            }
        } 
    }
    closedir(dir);
    free(dirEntry); 
    free(nextPath); 
}