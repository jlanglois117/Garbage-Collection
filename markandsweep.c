#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Size of the Stack and Heap, 1000 was chosen to adequately simulate the stack and heap time complexity
#define STACKHEAPLENGTH 100

/*Create a node structure that contains some data, a pointer to another node,
and a flag to determine if it is marked or not.
*/
struct Node {
    //void* data;
    int data;
    struct Node* next;
    int markedFlag;
};


//Creates a list structure that contains a root node (Linked List head)
struct List {
    struct Node* root;
    int id;
};

//Takes an integer as input to be held in the node and creates a node struct with NULL pointer and default unmarked flag of 0
struct Node* createNode (int data, int markedFlag) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    newNode->markedFlag = markedFlag;

    return newNode;
}


//Creates the list by taking a node as input and initilizing that node as the root node in the list
struct List* createList (struct Node* root){
    struct List* newList = malloc(sizeof(struct List));
    newList->root = root;

    return newList;
}

//Function that takes a List and a node as input and iterates to the end of the linked list and inserts the node there
void addToList (struct Node* node, struct List* list) {
    struct Node* current = list->root;

    while(current->next != NULL){
        current = current->next;
    }
    current->next = node;
}

//Function that takes the stack as input and iterates through the stack's root nodes,
// marking all reachable nodes with a "Marked" flag
void mark (struct List* stack[]) {
    

    for (int i = 0; i < STACKHEAPLENGTH; i++){
        struct List* list = stack[i];
        if (list == NULL){
            continue;
        }
        struct Node* current = list->root;
        
        while(current != NULL){
            printf("\nStack Data: %i Stack BFlag: %i", current->data, current->markedFlag);
            current->markedFlag = 1;
            current = current->next;
        }
    }
};

//Function that takes the heap as input and iterates through the heap, 
//searching whether each node is marked, frees nodes that are not marked.
void sweep (struct Node* heap[]) {

    for (int i = 0; i < STACKHEAPLENGTH; i++){

        if(heap[i] == NULL){
            continue;
        }
        printf("\nHeap Data: %i Heap Flag: %i", heap[i]->data, heap[i]->markedFlag);
        if (heap[i]->markedFlag == 0){
            free(heap[i]);
            heap[i] = NULL;
        } else {
            heap[i]->markedFlag = 0;
        }
    }
}

//Takes a list as input and prints each node in the list
void printList (struct List* list) {
    struct Node* current = list->root;

    while(current != NULL){
        printf("\nNode: %i, markedFlag: %i", current->data, current->markedFlag);
        current = current->next;
    }
}


int main () {

    clock_t start, end;
    time_t t;

    //Create the Stack and Heap Arrays, the stack holds Linked Lists
    //The heap holds Nodes
    struct List* stack[STACKHEAPLENGTH];
    struct Node* heap[STACKHEAPLENGTH];

    //Initialize both the stack and heap elements to NULL
    for (int i = 0; i < STACKHEAPLENGTH; i++){
        stack[i] = NULL;
    }

    for (int i = 0; i < STACKHEAPLENGTH; i++){
        heap[i] = NULL;
    }

    int heapIndex = 0;
    int numOfLists = 5;
    int numOfNodesPerList = 3;

    //Creates the linked lists and populates them with nodes
    //Uses a random number to either add to linked list and heap,
    //Add to just the linked list, or add just to the heap.
    for (int i = 0; i < numOfLists; i++){
        struct Node* root = createNode(i*10, 0);
        struct List* list = createList(root);
        list->id = i;
        stack[i] = list;
        for(int j = 1; j <= numOfNodesPerList; j++){
            char intI[10];
            char intJ[10];
            sprintf(intI, "%i", i);
            sprintf(intJ, "%i", j);
            strcat(intI, intJ);
            struct Node* node = createNode(atoi(intI), 0);

            srand(time(&t));
            int randomNum = rand() % 3;
            //Condition where node is added to list and the heap, won't add to heap if it is full
            if (randomNum == 0){
                addToList(node, stack[i]);
                if (heapIndex < STACKHEAPLENGTH){
                    heap[heapIndex] = node;
                    heapIndex++;
                } else {
                    printf("The heap is full.");
                    break;
                }
            //Condition where node is just added to the list
            } else if (randomNum == 1) {
                addToList(node, stack[i]);
            //Condition where node is just added to heap with no reference from stack
            } else {
                 if (heapIndex < STACKHEAPLENGTH){
                    heap[heapIndex] = node;
                    heapIndex++;
                } else {
                    printf("The heap is full.");
                    break;
                }
            }
        }
    }

    //Measuring the runtime of the mark and sweep functions using time.h clock() function
    start = clock();
    mark(stack);
    sweep(heap);
    end = clock();

    //Print the nodes that were left on the Heap
    for (int i = 0; i < STACKHEAPLENGTH; i++){
        if (heap[i] == NULL){
            continue;
        }
        printf("\nHeap Data: %i Heap AFlag: %i", heap[i]->data, heap[i]->markedFlag);
    }

    printf("\nTIME: %f", ((double)(end - start))/CLOCKS_PER_SEC);

    return 0;    
}