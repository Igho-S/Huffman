//Хаззан Омотола
#include <stdio.h>
#include <stdlib.h>

/*По сути, чтобы построить дерево Хаффмана, мы сортируем в порядке убывания все 
различные символы по их вероятности.
Затем мы берем два символа с наименьшей вероятностью и создаем новый общий узел 
с вероятностью, равной сумме двух вероятностей. Мы продолжаем моделировать любые
два самых маленьких узла, а затем добавляем новый узел в таблицу вместо двух 
самых нижних узлов.
Мы повторяем шаги и после этого выделяем 0 на все левые ветви дерева и 1 на все 
правые ветви дерева.
Чтобы получить код для каждого символа, мы читаем с вершины дерева 
соответствующий код в соответствии с ветвями до символа, код которого мы читаем.*/

struct Node{
    char symbol; //для наших символов
    unsigned frequency; //наша частота не может быть отрицательной, поэтому мы используем unsigned
    struct Node *left, *right;//здесь мы определяем левый и правый узлы
};

struct MinHeap {//минимальные узлы кучи
    unsigned size;//размер и емкость нашей минимальной кучи
    unsigned capacity;
    struct Node** array;//здесь мы создаем массив узлов минимальной кучи
};

struct Node* newNode(char symbol, unsigned frequency){//выделить новый узел минимальной кучи с заданным символом и частотой символа
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    //malloc выделить блок размером байт памяти и вернуть указатель на начало блока
    
    temp->left = temp->right = NULL;//чтобы обнулить значения в левой и правой ветвях
    temp->symbol = symbol;
    temp->frequency = frequency;
  
    return temp;
}

struct MinHeap* createMinHeap(unsigned capacity){//создать минимальную кучу емкости
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;//чтобы обнулить размер
    minHeap->capacity = capacity;
    minHeap->array = (struct Node**)malloc(minHeap->capacity * sizeof(struct Node*));
    return minHeap;
}

void swapNode(struct Node** a, struct Node** b){//поменять местами два узла кучи min
    struct Node* z = *a;
    *a = *b;
    *b = z;
}

void minHeapify(struct MinHeap* minHeap, int curr){//"Heapify" минимальные значения
    int smallest = curr;
    int left = 2 * curr + 1;
    int right = 2 * curr + 2;
  
    if (left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
        smallest = left;
  
    if (right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
        smallest = right;
  
    if (smallest != curr) {
        swapNode(&minHeap->array[smallest], &minHeap->array[curr]);
        minHeapify(minHeap, smallest);
    }
}

struct Node* extractMin(struct MinHeap* minHeap){//чтобы получить узел минимального значения из кучи
    struct Node* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
  
    return temp;
}

int one(struct MinHeap* minHeap){return (minHeap->size == 1);}
////проверить, равен ли размер кучи 1
void insertMinHeap(struct MinHeap* minHeap, struct Node* Node){
//вставить новый узел в минимальную кучу
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && Node->frequency < minHeap->array[(i - 1) / 2]->frequency){
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = Node;
}

void buildMinHeap(struct MinHeap* minHeap){//построить минимальную кучу
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

void printArr(int arr[], int n){
    for (int i = 0; i < n; ++i) printf("%d", arr[i]);
    printf("\n");
}

int isLast(struct Node* root){return !(root->left) && !(root->right);}//чтобы проверить, является ли узел листом
  
// Creates a min heap of capacity
// equal to size and inserts all character of
// symbol[] in min heap. Initially size of
// min heap is equal to capacity
struct MinHeap* createAndBuildMinHeap(char symbol[],int frequency[], int size){
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(symbol[i], frequency[i]);
  
    minHeap->size = size;
    buildMinHeap(minHeap);
  
    return minHeap;
}

struct Node* buildHuffmanTree(char symbol[], int frequency[], int size){//построить дерево
    struct Node *left, *right, *top;

    struct MinHeap* minHeap = createAndBuildMinHeap(symbol, frequency, size);
    //мы создаем минимальную кучу емкости, равной размеру
    while(!one(minHeap)){//пока мы не исчерпали значения в массиве

        left = extractMin(minHeap);//мы берем любые два минимальных значения из кучи
        right = extractMin(minHeap);
        top = newNode('#', right->frequency + left->frequency);
        //мы создаем новый внутренний узел с частотой, равной сумме частот двух узлов.
        //и, следовательно, сделать два извлеченных узла левым и правым дочерними элементами нового узла.
        //а затем добавьте этот узел в минимальную кучу
        //#это специальное значение для внутренних узлов
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

void printCodes(struct Node* root, int arr[], int top){//arr[] хранит коды
    if (root->left) {//присвоить 0 всем левым краям
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    if (root->right){//присвоить 1 всем правым краям
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
    if (isLast(root)) {
        printf("%c: ", root->symbol);
        printArr(arr, top);
    }
}

void HuffmanCodes(char symbol[], int frequency[], int size){//построение дерева и получение кодов
    struct Node* root = buildHuffmanTree(symbol, frequency, size);
    int arr[10], top = 0;//10 как максимальная высота дерева
  
    printCodes(root, arr, top);
}

int main(void){
    //char arr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    //int frequency[] = {30, 30, 12, 10, 8, 5, 5};
    
    char arr[] = {"the quick brown fox"};
    int frequency[0];
    int size = sizeof(arr) / sizeof(arr[0]);
    HuffmanCodes(arr, frequency, size);
  
    return 0;
}


