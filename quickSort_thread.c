#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
typedef struct{
	int sortData[1000000];
	int left;
	int right;
}Data;

void quickSort(Data* data);
void medianLeft(Data* data)//(int sortData[], int left, int right)
{
	int mid;
	int hold;
	int i;

	mid=(data->left+data->right)/2;
	if(data->sortData[data->left]>data->sortData[mid])
	{
		hold=data->sortData[data->left];
		data->sortData[data->left]=data->sortData[mid];
		data->sortData[mid]=hold;
	}
	if(data->sortData[data->left]>data->sortData[data->right])
	{
		hold=data->sortData[data->left];
		data->sortData[data->left]=data->sortData[data->right];
		data->sortData[data->right]=hold;
	}
	if(data->sortData[mid]>data->sortData[data->right])
	{
		hold=data->sortData[mid];
		data->sortData[mid]=data->sortData[data->right];
		data->sortData[data->right]=hold;
	}

	hold=data->sortData[data->left];
	data->sortData[data->left]=data->sortData[mid];
	data->sortData[mid]=hold;

	return;
}

/*void quickInsertion(int data[], int first, int last)
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
*/

void quickSort_after(Data* data)//(int sortData[],int left, int right)
{
#define MIN_SIZE 16
	int sortLeft;
	int sortRight;
	int pivot;
	int hold;
	int i;
	int hold_Left;
	int hold_Right;

	medianLeft(data);//(sortData,left,right);

	pivot	=data->sortData[data->left];
	sortLeft=(data->left)+1;
	sortRight=(data->right);
	while(sortLeft<=sortRight)
	{
		while(data->sortData[sortLeft]<pivot)
			sortLeft=sortLeft+1;
		while(data->sortData[sortRight]>=pivot)
			sortRight=sortRight-1;
		if(sortLeft<=sortRight)
		{

			hold	=data->sortData[sortLeft];
			data->sortData[sortLeft]=data->sortData[sortRight];
			data->sortData[sortRight]=hold;
			sortLeft=sortLeft+1;
			sortRight=sortRight-1;
		}
	}
	data->sortData[data->left]	=data->sortData[sortLeft-1];
	data->sortData[sortLeft-1]=pivot;
	hold_Left = data->left;
	hold_Right = data->right;
	if(data->left<sortRight){
		sortRight=sortRight-1;
		data->right=sortRight;
		quickSort_after(data);//(sortData,left,sortRight-1);
	}
	data->right = hold_Right;
	if(sortLeft<data->right){
		data->left=sortLeft;
		quickSort_after(data);//(sortData,sortLeft,right);
	}

	return ;
}
void * quickSort_thread(void *arg)//(int sortData[],int left, int right)
{
#define MIN_SIZE 16
	int sortLeft;
	int sortRight;
	int pivot;
	int hold;
	int i;
	int hold_Left;
	int hold_Right;
	Data *data;

	data = (Data*)malloc(sizeof(Data));
	data = (Data*)arg;

	medianLeft(data);//(sortData,left,right);

	pivot	=data->sortData[data->left];
	sortLeft=(data->left)+1;
	sortRight=(data->right);
	while(sortLeft<=sortRight)
	{
		while(data->sortData[sortLeft]<pivot)
			sortLeft=sortLeft+1;
		while(data->sortData[sortRight]>=pivot)
			sortRight=sortRight-1;
		if(sortLeft<=sortRight)
		{

			hold	=data->sortData[sortLeft];
			data->sortData[sortLeft]=data->sortData[sortRight];
			data->sortData[sortRight]=hold;
			sortLeft=sortLeft+1;
			sortRight=sortRight-1;
		}
	}
	data->sortData[data->left]	=data->sortData[sortLeft-1];
	data->sortData[sortLeft-1]=pivot;
	hold_Left = data->left;
	hold_Right = data->right;
	if(data->left<sortRight){
		sortRight=sortRight-1;
		data->right=sortRight;
		quickSort_after(data);//(sortData,left,sortRight-1);
	}
	data->right = hold_Right;
	if(sortLeft<data->right){
		data->left=sortLeft;
		quickSort_after(data);//(sortData,sortLeft,right);
	}

	return ;
}
void quickSort(Data* data)//(int sortData[],int left, int right)
{
#define MIN_SIZE 16
	int sortLeft;
	int sortRight;
	int pivot;
	int hold;
	int i;
	int hold_Left;
	int hold_Right;
	pthread_t thread_id1,thread_id2;
	int thread1,thread2;
	medianLeft(data);//(sortData,left,right);

	pivot	=data->sortData[data->left];
	sortLeft=(data->left)+1;
	sortRight=(data->right);
	while(sortLeft<=sortRight)
	{
		while(data->sortData[sortLeft]<pivot)
			sortLeft=sortLeft+1;
		while(data->sortData[sortRight]>=pivot)
			sortRight=sortRight-1;
		if(sortLeft<=sortRight)
		{

			hold	=data->sortData[sortLeft];
			data->sortData[sortLeft]=data->sortData[sortRight];
			data->sortData[sortRight]=hold;
			sortLeft=sortLeft+1;
			sortRight=sortRight-1;
		}
	}
	data->sortData[data->left]	=data->sortData[sortLeft-1];
	data->sortData[sortLeft-1]=pivot;
	hold_Left = data->left;
	hold_Right = data->right;
	if(data->left<sortRight){
		sortRight=sortRight-1;
		data->right=sortRight;
		thread1 = pthread_create(&thread_id1,NULL,&quickSort_thread,(void*)data);
		thread1 = pthread_join(thread_id1,NULL);
	}
	data->right = hold_Right;
	if(sortLeft<data->right){
		data->left=sortLeft;
		thread2 = pthread_create(&thread_id2,NULL,&quickSort_thread,(void*)data);
		thread2 = pthread_join(thread_id2,NULL);
	}

	return ;
}
int main(void)
{
	int arr[1000000]={0};
	int i;
	clock_t start,end;
	FILE* fp;
	Data* data;
	data = (Data*)malloc(sizeof(Data));
	data->left=0;
	data->right=999999;
	fp = fopen("10000000array.txt","r");
	printf("3\n");
	i=0;
	while(fscanf(fp,"%d",&arr[i])!=EOF)
			i++;
	printf("4\n");

	for(i=0;i<1000000;i++)
		data->sortData[i]=arr[i];
	start=clock();



	quickSort(data);


	end=clock();
	for(i=0;i<10;i++)
		printf("%d ",data->sortData[i]);
	fclose(fp);

	printf("\nRuntime is %lf msec, %lf sec\n",(double)(end-start),(end-start)/(double)1000);
	return 0;
}

