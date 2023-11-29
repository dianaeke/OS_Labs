// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu"; 
    char fixed_str2[] = "gobison.org";
    char input_str[100]; 
    char input_str2[100];
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
  
        close(fd1[0]);  // Close reading end of pipes 
        write(fd1[1], input_str, strlen(input_str)+1); // Write input string and close writing end of first pipe.
        close(fd1[1]);

        // Wait for child to send a string
        wait(NULL); 
 
        close(fd2[1]);
   
        char concat_str[200];
        read(fd2[0], concat_str, sizeof(concat_str));
  
        printf("Output: %s\n", concat_str);  
        close(fd2[0]); // Close writing end of pipes 
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipes 
        read(fd1[0], input_str2, sizeof(input_str2));
        close(fd1[0]);

        strcat(input_str2, fixed_str);

        // Close the reading ends 
        close(fd2[0]);
      
       write(fd2[1], input_str2, strlen(input_str2)+ 1);

       close(fd2[1]);
  
    } 
    return 0;
} 