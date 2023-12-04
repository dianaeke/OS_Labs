// C program for implementation of Simulation 
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "process.h"
#include "util.h"

void printMetrics(ProcessType plist[], int n);

// Function to find the waiting time for all processes
void findWaitingTimeRR(ProcessType plist[], int n, int quantum)
{
    int remaining_time[n];
    int i;
    for (i = 0; i < n; i++)
    {
        remaining_time[i] = plist[i].bt;
        plist[i].wt = 0;
    }

    int t = 0;
    while (1)
    {
        int done = 1;
        for (i = 0; i < n; i++)
        {
            if (remaining_time[i] > 0)
            {
                done = 0;

                if (remaining_time[i] > quantum)
                {
                    t += quantum;
                    remaining_time[i] -= quantum;
                }
                else
                {
                    t += remaining_time[i];
                    plist[i].wt = t - plist[i].bt;
                    remaining_time[i] = 0;
                }
            }
        }

        if (done == 1)
        {
            break;
        }
    }
}

// Function to find the waiting time for all processes
void findWaitingTimeSJF(ProcessType plist[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        int min_index = i;
        int j; // Added missing semicolon here
        for (j = i + 1; j < n; j++)
        {
            if (plist[j].bt < plist[min_index].bt)
            {
                min_index = j;
            }
        }

        ProcessType temp = plist[min_index];
        plist[min_index] = plist[i];
        plist[i] = temp;

        plist[i].wt = 0;
        int k;
        for (k = 0; k < i; k++)
        {
            plist[i].wt += plist[k].bt;
        }
    }
}

// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
    // waiting time for first process is 0, or the arrival time if not 
    plist[0].wt = 0 +  plist[0].art; 
  
    // calculating waiting time 
    int i;
    for (i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    int i;
    for (i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
int my_comparer(const void *a, const void *b)
{ 
  return ((ProcessType*)a)->pri - ((ProcessType*)b)->pri;
} 

//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
void findavgTimePriority( ProcessType plist[], int n) 
{ 
  qsort(plist, n, sizeof(ProcessType), my_comparer);
  
  //Display processes along with all details 
  printf("\n*********\nPriority\n");
  printMetrics(plist, n);
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    int i;
    for (i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}

// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 