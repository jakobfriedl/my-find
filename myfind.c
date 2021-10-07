//* MyFind implemented by Philipp Haider (if20b097) & Jakob Friedl (if20b089) - VERTS WS2021

#include "myfind.h"
#define IS_DIR 4 // dirent.h DT_DIR not recognized in WSL 

int flag_R = 0;
int flag_i = 0; 

void PrintUsage();
void Find(char* path, char* file);  

int main(int argc, char*argv[]) {
    int arg, status; 
    char* path = malloc(PATH_MAX); //Path to Searchdirectory

    while ((arg = getopt(argc, argv, "Ri")) != EOF){
        switch(arg){
            case 'R':
                if(flag_R){
                    PrintUsage();
                    return EXIT_FAILURE;
                } 
                flag_R++; 
            break;
            case 'i':
                if(flag_i){
                    PrintUsage();
                    return EXIT_FAILURE;
                } 
                flag_i++; 
            break; 
            default: return EXIT_FAILURE;
        } 
    }

    if (argc<optind+2) { // Invalid number of arguments
        PrintUsage();
        return EXIT_FAILURE; 
    }

    strcpy(path, argv[optind]); // set path variable
    optind++;

    for(; optind < argc; optind++){
        switch(fork()){
            case -1: 
                fprintf(stderr, "Error, could not start child process.\n"); 
                return EXIT_FAILURE;
            case 0: 
                Find(path, argv[optind]);
                return EXIT_SUCCESS;
            default:
                break; 
        }
    }
    while (wait(&status) > 0); // Wait for processes to finish

    free(path); 
    return EXIT_SUCCESS; 
}

void PrintUsage(){
    fprintf(stderr, "Invalid arguments, please use:\n\tmyfind [-R] [-i] <path> <file1> <file2> ... <fileN>\n"); 
}

void Find(char* path, char* file){
    char* nextPath = malloc(PATH_MAX);
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
                    char cwd[PATH_MAX];
                    realpath(path, cwd);
                    fprintf(stdout, "%d: %s: %s/%s\n", getpid(), dirEntry->d_name, cwd, dirEntry->d_name); 
                }
            }
        } 
    } closedir(dir);
    free(dirEntry); 
    free(nextPath); 
}