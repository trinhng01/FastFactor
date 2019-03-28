/**
 Name: Trinh Nguyen
 SID: 010469836
 CS149 - Operating System
 Assignment #1 - Fast Factor
 02/14/2019
 Version 1.0
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>  // For pipe
#include <unistd.h> // For exit

//Function Protytypes
int isDigit(char * str, char *eptr);

int main(int argc, char * argv[]) {
    long long int numArr [10];
    int size = 0;
    char * eptr;
    int check = 0;
    
    /** Read from file */
    if (argc == 1) {
        char *ptr;
        size_t n = 0;
        int count = 0;
        while (getline(&ptr, &n, stdin)>0){
            check = isDigit(ptr, eptr);
            if (check == -1){
                printf("Invalid Input\n");
                return -1;
            }else if (check == 1){
                numArr[count] = strtoll(ptr, &eptr, 10);
                count++;
            }
        }
        size = count;
    }
    
    /** Read from Command Line */
    else{
        for (int index = 1; index < argc; index++){
            check = isDigit(argv[index], eptr);
            if (check == -1){
                printf("Invalid Input\n");
                return -1;
            }else if (check == 1){
                numArr[index-1] = strtoll(argv[index], &eptr, 10);
            }
        }
        size = argc-1;
    }
    
    /** Factoring */
    for (int index = 0; index < size; index++){
        if (numArr[index] == 0) {
            printf("Invalid Input\n");
            return -1;
        }else{
            long long int value = numArr[index];
            long long int halfValue = value/2;
            
            if (value < 0){
                printf("Invalid Input\n");
                return -1;
            }
            
            char buff1[1000] = "";
            char buff2[1000] = "";
            char buff3[1000] = "";
            
            int fd1[2];
            int fd2[2];
            int fd3[2];
            
            //Create 3 pipe
            pipe(fd1);
            pipe(fd2);
            pipe(fd3);
            
            //Create 2 child processes and 1 grandchild process
            pid_t pid1 = fork();
            pid_t pid2 = fork();
            
            if (pid1 < 0 || pid2 < 0)
            {
                printf("Fork Failed\n");
                return -1;
            }
            
            if (pid1 == 0 && pid2 > 0) {    //Child 1
                char pipe1[1000] = "";
                for (long long int i=1; i<= halfValue/4; i++){
                    if (value%i==0){
                        sprintf(buff1, "%llu", i);
                        strcat(pipe1, buff1);
                        strcat(pipe1, " ");
                    }
                }
                close(fd1[0]);
                write(fd1[1], pipe1 , 1000);
                close(fd1[1]);
                wait(NULL); //wait for child 3
                exit(0);
                
            }else if (pid1 > 0 && pid2 == 0){   //Child 2
                char pipe2[1000] = "";
                for (long long int i=(halfValue/4)+1; i<= halfValue/2;i++){
                    if (value%i==0){
                        sprintf(buff2, "%llu", i);
                        strcat(pipe2, buff2);
                        strcat(pipe2, " " );
                    }
                }
                close(fd2[0]);
                write(fd2[1], pipe2 , strlen(pipe2));
                close(fd2[1]);
                exit(0);
            }else if (pid1 == 0 && pid2 == 0){ //Child 3
                char pipe3[1000] = "";
                for (long long int i=(halfValue/2)+1; i<= 3*(halfValue/4);i++){
                    if (value%i==0){
                        sprintf(buff3, "%llu", i);
                        strcat(pipe3, buff2);
                        strcat(pipe3, " ");
                    }
                }
                close(fd3[0]);
                write(fd3[1], pipe3 , strlen(pipe3));
                close(fd3[1]);
                exit(0);
            }else if (pid1 > 0 && pid2 > 0){ //Parent
                
                char final[10000] = "";
                char final1[10000] = "";
                char final2[10000] = "";
                char final3[10000] = "";
                
                //Read from 3 pipes
                wait(NULL); //wait for child 1
                wait(NULL); //wait for child 2
                
                //Close writing ends
                close(fd1[1]);
                close(fd2[1]);
                close(fd3[1]);
                
                printf("%lld: ", value);
                
                //Read from 3 pipes from children and close read ends
                read(fd1[0], final1, 10000); //Use strlen does not work
                close(fd1[0]);
                printf("%s", final1);
                
                read(fd2[0], final2, 10000);
                close(fd2[0]);
                printf("%s", final2);
                
                read(fd3[0], final2, 10000);
                close(fd3[0]);
                printf("%s", final3);
                
                char buff[1000];
                for (long long int i= 3*(halfValue/4)+1; i<= halfValue;i++){
                    if (value%i==0){
                        sprintf(buff, "%llu", i);
                        strcat(final, buff);
                        strcat(final, " " );
                    }
                }
                printf("%s", final);
                printf("%llu", value);
                printf("\n");
            }
        }
    }
    return 0;
}
//Check each character to find non-number character
int isDigit(char * str, char *eptr){
    long long int num = strtoll(str, &eptr, 10);
    if (num<0) return 1;
    if (num==0) return 1;
    for(int i = 0; i < strlen(str); i ++){
        printf("%c ", str[i]);
        if (str[i] > 57 || str[i] < 48) return -1;
    }
    return 1;
}
