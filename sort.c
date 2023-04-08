/*
Gary Orsargos
04/05/2023
Submission for "Lab Assignment 8"
*/

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void maxHeapify(int input[], int n, int root){
	int largest = root, temp;
    int l = (2 * root) + 1;
    int r = (2 * root) + 2;
 
	//compare if the left or right is bigger than the root index
    if (l < n && input[l] > input[largest])
        largest = l;

    if (r < n && input[r] > input[largest])
        largest = r;

	//if there is a larger child, then swap and call heapify on the subtree
    if (largest != root) {
        int temp = input[root];
    	input[root] = input[largest];
    	input[largest] = temp;
        maxHeapify(input, n, largest); // recursively call heapify
    }
}
// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	int temp;
	for(int i = n/2 - 1; i >= 0; i--){
		maxHeapify(arr, n, i);
	}
	for(int i = n-1; i >= 0; i--){
		//swap the root node to the end of the array, then re-heapify
		temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;
		maxHeapify(arr, i, 0);
	}
}

//used during the merge sort algorithm
void merge(int arr[], int left, int middle, int right) {
  //set sizes for the different parts of the subarray (left and right)
  int sizeL = middle - left + 1;
  int sizeR = right - middle;
  //allocate memory for subarrays, then insert the items to their respective array
  int x, y, z;
  int *rightArr = (int *)malloc(sizeR * sizeof(int));
  int *leftArr = (int *)malloc(sizeL * sizeof(int));
  extraMemoryAllocated += sizeof(int)*2;
  for (y = 0; y < sizeR; y++)
    rightArr[y] = arr[middle+y+1];
  for (x = 0; x < sizeL; x++)
    leftArr[x] = arr[left + x];
  int leftCount = 0, rightCount = 0, merged = left;
  //merge the two new subarrays back together in ascending number order
  while (rightCount < sizeR && leftCount < sizeL) {
    if (leftArr[leftCount] <= rightArr[rightCount]) {
      arr[merged] = leftArr[leftCount];
      leftCount++;
    } else {
      arr[merged] = rightArr[rightCount];
      rightCount++;
    }
    merged++;
  }
  //if the two arrays are not the same length, these loops will fill pData array with the remaining data
  while (leftCount < sizeL) {
    arr[merged] = leftArr[leftCount];
    leftCount++;
    merged++;
  }
  while (rightCount < sizeR) {
    arr[merged] = rightArr[rightCount];
    rightCount++;
    merged++;
  }
  //free memory
  free(rightArr);
  free(leftArr);
}
// implement merge sort (the above function is also used in this implementation)
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int left, int right) {
  if (left < right) {
    int middle = (left + right)/2;
    mergeSort(pData, middle + 1, right);
    mergeSort(pData, left, middle);
    merge(pData, left, middle, right);
  }
}


// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100, prevData = -1;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		if(pData[i] == 0 && pData[prevData] != 0)
			break;
		printf("%d ",pData[i]);
		prevData = pData[i];
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}