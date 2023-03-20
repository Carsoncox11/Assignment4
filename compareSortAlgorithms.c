#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	//Memory allocated for the parameter variables
	extraMemoryAllocated += sizeof(&pData) + sizeof(l) + sizeof(r);

	if (l < r)
	{
		//find the midpoint of the array
        int mid = (l + r) / 2;
		extraMemoryAllocated += sizeof(mid);

		//recursive calls for each half of the array
        mergeSort(pData, l, mid);
        mergeSort(pData, mid + 1, r);
  
        int i, j, k;
		extraMemoryAllocated += sizeof(i) + sizeof(j) + sizeof(k);
		int n1 = mid - l + 1;
		extraMemoryAllocated += sizeof(n1);
		int n2 = r - mid;
		extraMemoryAllocated += sizeof(n2);
	
		//malloc space for the temporary arrays
		int * left = (int *) malloc(n1 * sizeof(int));
		extraMemoryAllocated += sizeof(left);
		int * right = (int *) malloc(n2 * sizeof(int));
		extraMemoryAllocated += sizeof(right);
	
		//copy data into both temporary arrays
		for (i = 0; i < n1; i++)
			left[i] = pData[l + i];

		for (j = 0; j < n2; j++)
			right[j] = pData[mid + 1 + j];
	
		//starting index of each array
		i = 0;
		j = 0;
		k = l;

		//merge the arrays back together
		while (i < n1 && j < n2)
		{
			if (left[i] <= right[j])
			{
				pData[k] = left[i];
				i++;
			}
			else
			{
				pData[k] = right[j];
				j++;
			}

			k++;
		}
	
		//if there are remaining values, copy them over
		while (i < n1)
		{
			pData[k] = left[i];
			i++;
			k++;
		}
	
		//if there are remaining values, copy them over
		while (j < n2) 
		{
			pData[k] = right[j];
			j++;
			k++;
		}

		free(left);
		free(right);
    }
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	//Memory allocated for the parameter variables
	extraMemoryAllocated += sizeof(&pData) + sizeof(n);

	int temp;
	extraMemoryAllocated += sizeof(temp);

	for(int i = 1; i < n; i++)
	{
		int j = i;

		//while pData at index j is smaller then what is before it in the array, swap the two
		while(j > 0 && pData[j] < pData[j - 1])
		{
			temp = pData[j];
			pData[j] = pData[j - 1];
			pData[j - 1] = temp;
			j--;
		}
	}

	//Memory allocated for i and j
	extraMemoryAllocated += sizeof(int) * 2;
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	extraMemoryAllocated += sizeof(&pData) + sizeof(n);

	int i, j,temp;
	extraMemoryAllocated += sizeof(i) + sizeof(j) + sizeof(temp);

	for (i = 0; i < n - 1; i++)
	{
		//go until n - i - 1 because the greatest values will be put last and thus already in order
		for (j = 0; j < n - i - 1; j++)
		{
			//if the next value is less than the current value, swap the two
			if (pData[j] > pData[j+1])
			{
				temp = pData[j];
				pData[j] = pData[j+1];
				pData[j+1] = temp;
			}
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	//Memory allocated for the parameter variables
	extraMemoryAllocated += sizeof(&pData) + sizeof(n);

	int temp;
	extraMemoryAllocated += sizeof(temp);

	for(int i = 0; i < n - 1; i++)
	{
		int minIndex = i;

		//begin at the unsorted part of the array
		for(int j = i + 1; j < n; j++)
		{
			if(pData[j] < pData[minIndex])
				minIndex = j; 
		}

		//if the minimum is not at index i, swap index i and the minimum index
		if(minIndex != i)
		{
			temp = pData[minIndex];
			pData[minIndex] = pData[i];
			pData[i] = temp;
		}
	}

	//Extra memory allocated for i, minIndex, and j
	extraMemoryAllocated += sizeof(int) * 3;
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		int data;
		for(int i = 0; i < dataSz; i++)
		{
			fscanf(inFile, "%d", &data); //read from the file
			(*ppData)[i] = data; //store th values read into an array
		}
	}

	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
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
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);

		
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
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