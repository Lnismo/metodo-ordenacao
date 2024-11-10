#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

//comparar tempo

void compare(double temp1, double temp2, double temp3, char* res){
    if(temp1 <= temp2 && temp1 <= temp3){
        sprintf(res,"o melhor tempo foi o quick com %f", temp1);
    }else if(temp2 <= temp1 && temp2 <= temp3){
        sprintf(res,"o melhor tempo foi o merge com %fs", temp2);
    }else{
        sprintf(res,"o melhor tempo foi o heap com %fs", temp3);
    }
}

//gerador

void gerador(int tam){
    int *vetor;
    size_t tamanho = tam;

    vetor = (int *)malloc(tamanho * sizeof(int));

    if (vetor == NULL) 
	{
        printf("\nErro ao alocar memória\n");
        return;
    }

    srand(time(NULL));

    for (size_t i = 0; i < tamanho; i++) 
	{
        vetor[i] = rand() % 1000000; 
    }
    
    FILE *arq = fopen("numeros_sorteados.txt", "w");
    if (arq == NULL) 
	{
        printf("\n\tErro ao abrir o arquivo\n");
        free(vetor);
        return;
    }
    for (size_t i = 0; i < tamanho; i++) 
	{
        fprintf(arq, "%d ", vetor[i]);
    }
	    fclose(arq);
    free(vetor);
    return;
}

//quick sort

void swap(int* a, int* b);

// Partition function
int partition(int arr[], int menor, int maior) {
    int pivot = arr[maior];
    
    int i = menor - 1;

    for (int j = menor; j <= maior - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    
    swap(&arr[i + 1], &arr[maior]);  
    return i + 1;
}

void quickSort(int arr[], int menor, int maior) {
    if (menor < maior) {
        int pi = partition(arr, menor, maior);
        quickSort(arr, menor, pi - 1);
        quickSort(arr, pi + 1, maior);
    }
}

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

//merge sort

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

//heap sort

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && arr[left] > arr[largest])
        largest = left;
    
    if (right < n && arr[right] > arr[largest])
        largest = right;
    
    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}

void heapSort(int *arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    
    for (int i = n - 1; i >= 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}

// contar arquivo

int contarNum(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }

    int count = 0;
    int temp;
    while (fscanf(file, "%d", &temp) == 1) {
        count++;
    }

    fclose(file);
    return count;
}

int *lerNum(const char *filename, int size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    int *arr = (int *)malloc(size * sizeof(int));
    if (arr == NULL) {
        printf("Erro ao alocar memória.\n");
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        fscanf(file, "%d", &arr[i]);
    }

    fclose(file);
    return arr;
}

//main

int main(){
    char result[100];
    int quant;
    printf("digite a quantidade de items: ");
    scanf("%d", &quant);
    gerador(quant);

    clock_t start, end;
    double tempo1, tempo2, tempo3;
    const char *filename = "numeros_sorteados.txt";
    int tam = contarNum(filename);

    int *quickArr = lerNum(filename,tam);
    start = clock();
    quickSort(lerNum(filename, tam), 0, tam - 1);
    end = clock();
    printf("\nquick ordenado: ");for(int i = 0; i < tam; i++) printf("%d ", quickArr[i]);
    tempo1 = ((double)(end - start)) / CLOCKS_PER_SEC;

    int *mergeArr = lerNum(filename,tam);
    printf("\n");
    start = clock();
    mergeSort(lerNum(filename, tam), 0 , tam - 1);
    end = clock();
    printf("\nmerge ordenado: ");for(int i = 0; i < tam; i++) printf("%d ", mergeArr[i]);
    tempo2 = ((double)(end - start)) / CLOCKS_PER_SEC;

    int *heapArr = lerNum(filename,tam);
    printf("\n");
    start = clock();
    heapSort(lerNum(filename, tam), tam);
    end = clock();
    printf("\nheap ordenado: ");for(int j = 0; j < tam; j++) printf("%d ", heapArr[j]);
    tempo3 = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n\nquick tempo: %.3fs\nmerge tempo: %.3fs\nheap tempo: %.3fs", tempo1,tempo2,tempo3);
    compare(tempo1,tempo2,tempo3,result);
    printf("\n%s", result);

    Sleep(1);
}
