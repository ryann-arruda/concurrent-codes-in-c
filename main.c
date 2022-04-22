#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void create_dict(){
    FILE *file = fopen("dict.txt", "w");

    if(file){
        for(int i = 0; i < 26; i++){
            char name[11];
        
            sprintf(name, "part%d.txt", i);

            FILE *aux = fopen(name, "r");

            if(aux){
                char text[7];
                
                while((fgets(text, 7, aux)) > 0){
                    fputs(text, file);
                }

                fclose(aux);
                remove(name);
            }
        }

        fclose(file);
    }
}

void file_process(int parent_pid){
    int pids[26];

    for(int i = 0, j = 65; i < 26; i++, j++){

        if(getpid() == parent_pid){
            int aux = fork();
            pids[i] = aux;
        }
       
        if(getpid() != parent_pid){
            char file_name[11];
            
            sprintf(file_name, "part%d.txt", i);
            
            FILE* part = fopen(file_name, "w");

            if(part){
                for(int y = 65; y < 91; y++){
                    for(int k = 65; k < 91; k++){
                        for(int h = 65; h < 91; h++){
                            char aux[6];
                            sprintf(aux, "%c%c%c%c\n", j,h,k,y);
                            fputs(aux, part);
                        }
                    }
                }

                fclose(part);

                exit(0);
            }
        
            exit(1);
        }
    } 

    for(int i = 0; i < 26; i++){
        waitpid(pids[i], NULL, 0);
    }

    create_dict();
}

void* file_thread(void*p){

    for(int i = 0, j = 65; i < 26; i++, j++){ 
        char file_name[11];
        
        sprintf(file_name, "part%d.txt", i);
        
        FILE* part = fopen(file_name, "w");

        if(part){
            for(int y = 65; y < 91; y++){
                for(int k = 65; k < 91; k++){
                    for(int h = 65; h < 91; h++){
                        char aux[6];
                        sprintf(aux, "%c%c%c%c\n", j,h,k,y);
                        fputs(aux, part);
                    }
                }
            }

            fclose(part);
        }
    } 
}

int main(int argc, char** argv){
    int parent_pid = getpid();

    if(argc < 2){
        printf("%s: poucos parâmetros\n", argv[0]);
    }
    else if(argc > 2){
        printf("%s: muitos parâmetros\n", argv[0]);
    }
    else{
        if(strcmp(argv[1], "-p") == 0){
            file_process(parent_pid);
        }

        else if(strcmp(argv[1], "-t") == 0){
            pthread_t threads[26];

            for(int i = 0; i < 26; i++){
                pthread_t thread;
                pthread_create(&thread, NULL, file_thread, NULL);
                threads[i] = thread;
            }

            for(int i = 0; i < 26; i++){
                pthread_join(threads[i], NULL);
            }

            create_dict();
        }

        else{
            printf("%s: comando não encontrado\n", argv[1]);
        }
    }

    return 0;
}