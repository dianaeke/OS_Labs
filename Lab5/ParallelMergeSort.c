#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 100

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    int i;
    for (i = 0; i < SIZE; i++) {
        array[i] = rand() % 100;
    }
}

void printArray(int arr[SIZE]) {
    int i;
    for (i = 0; i < SIZE; i++) {
        printf("%5d", arr[i]);
    }
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;

// Runs mergesort on the array segment described in the
// argument. Spawns two threads to mergesort each half
// of the array segment, and then merges the results.
void merge(int arr[], int start, int midl, int end) {
    int left_size = midl - start + 1;
    int right_size = end - midl;
    int left[left_size];
    int right[right_size];
    int i;
    for (i = 0; i < left_size; i++) {
        left[i] = arr[start + i];
    }
    int j;
    for (j = 0; j < right_size; j++) {
        right[j] = arr[midl + j + 1];
    }
    int k = start;
    i = 0;
    j = 0;
    while (i < left_size && j < right_size) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    while (i < left_size) {
        arr[k] = left[i];
        i++;
        k++;
    }
    while (j < right_size) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int start, int end) {
    if (start < end) {
        int midl = start + (end - start) / 2;
        mergeSort(arr, start, midl);
        mergeSort(arr, midl + 1, end);
        merge(arr, start, midl, end);
    }
}

void *mergeSortParallel(void *args) {
    StartEndIndexes *seArgs = (StartEndIndexes *)args;
    mergeSort(array, seArgs->start, seArgs->end);
    return NULL;
}

int main() {
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;

    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);

    // 2. Print the array.
    printf("Unsorted array: ");
    printArray(array);

    // 3. Create a thread for merge sort.
    pthread_t mergeSortThread;
    pthread_create(&mergeSortThread, NULL, mergeSortParallel, (void *)&sei);

    // 4. Wait for mergesort to finish.
    pthread_join(mergeSortThread, NULL);

    // 5. Print the sorted array.
    printf("Sorted array:   ");
    printArray(array);

    return 0;
}

