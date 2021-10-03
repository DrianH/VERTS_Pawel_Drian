#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/wait.h>
#include <asm/errno.h>
#include <errno.h>

#include "funktionen.h"

//Gibt aus, wie myfind verwendet werden sollte.
void printUsage (char *programName){
    fprintf(stderr, "Usage: %s [-R] [-i] searchpath filename 1 [filename2] ... [filename n]\n", programName);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    int c;
    int err = 0;
    int rec = 0;
    int caseIns = 0;
    char *programName = argv[0];

    //Logik fuer die Argumente -R und -i
    while((c = getopt(argc, argv, "Ri")) != EOF){
        switch(c){
            case 'R':
                if(rec){
                    fprintf(stderr, "Multiple flags not allowed\n");
                    err = 1;
                    break;
                }
                rec = 1;
                break;
            case 'i':
                if(caseIns){
                    fprintf(stderr, "Multiple flags not allowed\n");
                    err = 1;
                    break;
                }
                caseIns = 1;
                break;
            case '?':
                err = 1;
                break;
            default:
                assert(0);
        }
    }

    //Zeige printUsage, wenn es ein Error gibt.
    if(err){
        printUsage(programName);
    }

    //Wenn eine falsche Zahl an Argumente eingegeben wird.
    if((argc < optind + 2)){
        fprintf(stderr, "False number of args\n");
        printUsage(programName);
    }

    char *dirName = argv[optind];
    optind++;

    pid_t *pIDs = calloc(argc - optind, sizeof(pid_t));
    int i = 0;
    while(optind < argc){
        pIDs[i++] = forkSearch(dirName, argv[optind++], rec, caseIns);
    }

    int status = 0;
    pid_t wPID;

    //Zombie-Processe killen.
    while ((wPID = wait(&status)) > 0){
        if(WIFEXITED(status)){
            printf("Child %d successfully exited with status %d.\n", wPID, WEXITSTATUS(status));
        }
        else{
            printf("Child %d not terminated correctly.\n", wPID);
        }
    }

    free(pIDs);
    exit(EXIT_SUCCESS);
}