#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <memory.h>
#include <ctype.h>
#include <unistd.h>

#include "funktionen.h"

void printFile(pid_t pID, char *fileName, char *absPath){
    printf("%d: %s: %s\n", pID, fileName, absPath);
}

char *getAbsPath(const char *path){
    char *realPath = realpath(path, NULL);
    return realPath;
}

int cmpFileNames(char *str1, char *str2, int caseIns){
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if(len1 != len2){
        return 0;
    }

    for(size_t i = 0; i < len1; i++){
        char c1 = caseIns ? (char)tolower(str1[i]) : str2[i];
        char c2 = caseIns ? (char)tolower(str2[i]) : str2[i];

        if(c1 != c2){
            return 0;
        }
    }

    return 1;
}

char *buildNewPath(const char *oldPath, char *fileName){
    char *newPath = calloc(strlen(oldPath) + strlen(fileName) + 3, 1);
    strcat(newPath, oldPath);
    strcat(newPath, "/");
    strcat(newPath, fileName);
    return newPath;
}

void searchFile(const char *dir, char *toSearch, int rec, int caseIns){
    DIR *directory;
    struct dirent *entry;

    if(!(directory = opendir(dir))){
        perror(dir);
        return;
    }

    while((entry = readdir(directory)) != NULL){
        char *fileName = entry -> d_name;

        switch(entry -> d_type){
            case DT_DIR: {
                if(rec){
                    if(!strcmp(fileName, ".") == 0 && !strcmp(fileName, "..") == 0){
                        char *newPath = buildNewPath(dir, fileName);
                        searchFile(newPath, toSearch, rec, caseIns);
                        free(newPath);
                    }
                }
                break;
            }
            case DT_REG: {
                int equal = cmpFileNames(fileName, toSearch, caseIns);
                if(equal){
                    char *combinedName = buildNewPath(dir, fileName);
                    char *absPath = getAbsPath(combinedName);
                    printFile(getpid(), toSearch, absPath);
                    free(combinedName);
                    free(absPath);
                }
                break;
            }
            default:
                break;
        }
    }
    while((closedir(directory) == -1 && (errno = EINTR)));
}

pid_t forkSearch(const char *dir, char *toSearch, int rec, int caseIns){
    pid_t pID = fork();

    if(pID== 0){
        searchFile(dir, toSearch, rec, caseIns);
        exit(EXIT_SUCCESS);
    }
    else if(pID == -1){
        fprintf(stderr, "Unable to start child");
        exit(EXIT_FAILURE);
    }
    return pID;
}