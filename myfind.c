#include "myfind.h"

#define IS_DIR 4 // dirent.h DT_DIR not recognized
#define MAX_PATH 255

int flag_R = 0;
int flag_i = 0; 

void PrintUsage();
void SimpleFind(char* path, char** files, int fileCount); 

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

    SimpleFind(path, files, numFiles);

    free(path); 
    free(*files); 
    return EXIT_SUCCESS; 
}

void PrintUsage(){
    printf("Invalid arguments, please use:\n\tmyfind [-R] [-i] <path> <file1> <file2> ... <fileN>\n"); 
}

void SimpleFind(char* path, char** files, int fileCount){
    char* nextPath = malloc(sizeof(path));
    char* foundPath = malloc(sizeof(path));
    struct dirent *dirEntry; 
    DIR *dir; 

    char asd[100]; 
    printf("trying to open dir: %s\n", path);
    printf("current wd: %s\n", getcwd(asd,100)); 

    if((dir = opendir(path)) == NULL){
        printf("Failed to open directory\n");
        return; 
    } 

    printf("opened dir: %s\n", path);

    while((dirEntry = readdir(dir)) != NULL){
        // only check files
        if(strcmp(dirEntry->d_name, ".") && strcmp(dirEntry->d_name, "..")){ 
            if(flag_R){
                if(dirEntry->d_type == IS_DIR){
                    strcpy(nextPath, dirEntry->d_name);

                    SimpleFind(nextPath, files, fileCount);
                }
            }
            if(dirEntry->d_type != IS_DIR){ 
                for(int i = 0; i < fileCount; i++){
                    //Check if flag -i is set
                    if(flag_i ? (!strncasecmp(files[i], dirEntry->d_name, sizeof(files))) : (!strcmp(files[i], dirEntry->d_name))){
                        //Change to current directory 
                        strcpy(foundPath, path);  
                        chdir(foundPath); 
                        char cwd[MAX_PATH]; 
                        printf("\n<pid>: %s: %s\n\n", dirEntry->d_name, getcwd(cwd, MAX_PATH)); 
                    }
                }
            }
        } 
    }

    free(dir);
    free(dirEntry);
}