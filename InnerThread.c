#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct{
	int *sortData;
	int left;
	int sortLeft;
}Data;

void medianLeft(int sortData[], int left, int right)
{
	int mid;
	int hold;


	mid=(left+right)/2;

	if(sortData[left]>sortData[mid])
	{
		hold=sortData[left];
		sortData[left]=sortData[mid];
		sortData[mid]=hold;
	}
	if(sortData[left]>sortData[right])
	{
		hold=sortData[left];
		sortData[left]=sortData[right];
		sortData[right]=hold;
	}
	if(sortData[mid]>sortData[right])
	{
		hold=sortData[mid];
		sortData[mid]=sortData[right];
		sortData[right]=hold;
	}

	hold=sortData[left];
	sortData[left]=sortData[mid];
	sortData[mid]=hold;

	return;
}

void quickInsertion(int data[], int first, int last)
{
	int hold;
	int walker;
	int current;

	for(current=first+1;current<=last;current++)
	{
		hold	= data[current];
		walker	= current -1;
		while(walker >= first && hold<data[walker])
		{
			data[walker+1]=data[walker];
			walker=walker-1;
		}
		data[walker+1]=hold;
	}
	return;
}
void *findsortLeft(void *arg)//(int sortData[],int left,int *sortLeft)
{
	Data *data;
	data=(Data*)malloc(sizeof(Data));
	data=(Data*)arg;
	int pivot=data->sortData[data->left];

	while(data->sortData[data->sortLeft]<pivot)
		data->sortLeft=data->sortLeft+1;
	return ;
}

void quickSort(int sortData[],int left, int right)
{
#define MIN_SIZE 16
	int sortLeft;
	int sortRight;
	int pivot;
	int hold;
	pthread_t thread_id;
	int thread;

	Data *data;
	data = (Data*)malloc(sizeof(Data));
	data->sortData=&sortData[left];

	medianLeft(sortData,left,right);

	pivot	=sortData[left];
	sortLeft=left+1;
	sortRight=right;

	data->left = left;
	data->sortLeft = sortLeft;

	while(sortLeft<=sortRight)
	{
//		while(sortData[sortLeft]<pivot)
//			sortLeft=sortLeft+1;
		thread = pthread_create(&thread_id,NULL,&findsortLeft,(void*)data);
		thread = pthread_join(thread_id,NULL);
		while(sortData[sortRight]>=pivot)
			sortRight=sortRight-1;
		sortLeft = data->sortLeft;
		if(sortLeft<=sortRight)
		{
			hold	=sortData[sortLeft];
			sortData[sortLeft]=sortData[sortRight];
			sortData[sortRight]=hold;
			sortLeft=sortLeft+1;
			sortRight=sortRight-1;
		}
	}
	sortData[left]	=sortData[sortLeft-1];
	sortData[sortLeft-1]=pivot;
	if(left<sortRight)
		quickSort(sortData,left,sortRight-1);
	if(sortLeft<right)
		quickSort(sortData,sortLeft,right);

	return ;
}

int main(void)
{
	int arr[10]={33,44,11,88,324,1,3490,23,211,19};
	int i;
	clock_t start,end;
//	FILE* fp;

//	fp = fopen("array.txt","r");

//	while(fscanf(fp,"%d",&arr[i]))
			i++;

	start=clock();

	quickSort(arr,0,9);

	end=clock();

	for(i=0;i<10;i++)
		printf("%d ",arr[i]);
	printf("\n");
//	fclose(fp);

	printf("\nRuntime is %lf msec, %lf sec\n",(double)(end-start),(end-start)/(double)1000);
	return 0;
}

