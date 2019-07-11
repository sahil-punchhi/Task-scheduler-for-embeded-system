// Name: Sahil Punchhi
// zID: z5204256
// COMP 9024 Assignment 3- Binomial Heap
// Date: 17th April 2019

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// This template is only a guide
// You need to include additional fields, data structures and functions

// data type for heap nodes
typedef struct HeapNode {
    // each node stores the priority (key), name, execution time,
    //  release time and deadline of one task
    int key; //key of this task
    int TaskName;  //task name
    int Etime; //execution time of this task
    int Rtime; // release time of this task
    int Dline; // deadline of this task
    int degree; //degree of the key
    struct HeapNode *child;
    struct HeapNode *parent;
    struct HeapNode *sibling;
} HeapNode;

//data type for a priority queue (heap)
typedef struct BinomialHeap{ //this is heap header
    int  size;      // count of items in the heap
    HeapNode *head;
} BinomialHeap;

// create a new heap node to store an item (task)
HeapNode *newHeapNode(int k, int n, int c, int r, int d)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
    HeapNode *new;
    new = malloc(sizeof(HeapNode));
    assert(new != NULL);
    new->key = k;
    new->TaskName = n;
    new->Etime = c;
    new->Rtime = r;
    new->Dline = d;
    new->degree = 0;
    new->child = NULL;
    new->parent = NULL;
    new->sibling = NULL;
    return new;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
    BinomialHeap *T;
    T = malloc(sizeof(BinomialHeap));
    assert (T != NULL);
    T->head = NULL;
    return T;
}

// time complexity for heapMerge is O(log n) where n is number of tasks in a heap
HeapNode *heapMerge( BinomialHeap *heap_1, BinomialHeap *heap_2 )
{
    if( heap_1->head == NULL )
        return heap_2->head;
    if( heap_2->head == NULL )
        return heap_1->head;
    HeapNode *node; HeapNode *node2;
    HeapNode *heap1 = heap_1->head; HeapNode *heap2 = heap_2->head;
    if( heap_1->head->degree <= heap_2->head->degree )
    { node = heap_1->head; heap1 = heap1->sibling; }
    else
    { node = heap_2->head; heap2 = heap2->sibling;  }
    node2 = node;
    while( heap1 != NULL && heap2 != NULL )
    {
        if( heap1->degree <= heap2->degree )
        {  node2->sibling = heap1; heap1 = heap1->sibling; }
        else
        { node2->sibling = heap2; heap2 = heap2->sibling; }
        
        node2 = node2->sibling;}
    node2->sibling = ( heap1 != NULL ) ? heap1 : heap2;
    return node;
}

// time complexity for heapUnion is O(log n) where n is number of tasks in a heap
HeapNode *heapUnion( BinomialHeap *existing, BinomialHeap *new_created )
{   HeapNode *head_ = heapMerge( existing, new_created );
    new_created->head = NULL; existing->head = NULL;
    HeapNode *prev_node = NULL;
    if( head_ == NULL )
        return NULL;
    HeapNode *transit_node = head_; HeapNode *next_node = head_->sibling;
    while( next_node != NULL )
    {
        if(( next_node->sibling != NULL && next_node->sibling->degree == transit_node->degree ) || transit_node->degree != next_node->degree )
        { prev_node = transit_node; transit_node = next_node;
        }
        else
        {
            if(next_node->key > transit_node->key)
            {
                transit_node->sibling = next_node->sibling; next_node->parent = transit_node;
                next_node->sibling = transit_node->child; transit_node->child = next_node;
                transit_node->degree++; }
            else
            {
                if( prev_node == NULL )
                    head_ = next_node;
                else
                    prev_node->sibling = next_node;
                
                transit_node->parent = next_node; transit_node->sibling = next_node->child;
                next_node->child = transit_node; next_node->degree++;
                transit_node = next_node;
            }}
        next_node = transit_node->sibling; }
    return head_;
}

// time complexity for Insert() is O(log n) where n is number of tasks
// time complexity for heapUnion is O(log n), merge is O(log n), hence overall time complexity for Insert function is O(log n)
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline
    // You don't need to check if this task already exists in T
    HeapNode *node = newHeapNode(k,n,c,r,d);
    BinomialHeap *heap_ = newHeap();
    heap_->head = node;
    T->head = heapUnion( T, heap_ ); free( heap_ );
}

// time complexity for RemoveMin() is O(log n) where n is the number of tasks
// returns minimum node and modifies the heap T
HeapNode *RemoveMin(BinomialHeap *T)
{
    if(T->head == NULL)
        return NULL;
    HeapNode *bef = NULL; HeapNode *min_node = T->head;
    HeapNode *next_ = min_node; HeapNode *next_node = min_node->sibling;
    while( next_node != NULL )
    {
        if( next_node->key < min_node->key )
        {  min_node = next_node; bef = next_; }
        next_ = next_node; next_node = next_node->sibling;
    }
    if( min_node == T->head )
        T->head = min_node->sibling;
    else
        bef->sibling = min_node->sibling;
    HeapNode *child = min_node->child; HeapNode *head_ = NULL;
    while( child != NULL )
    {
        HeapNode *next_node = child->sibling;
        child->sibling = head_; child->parent = NULL;
        head_ = child; child = next_node;
    }
    BinomialHeap *heap_ = newHeap();
    heap_->head = head_;
    T->head = heapUnion(T, heap_); free(heap_);
    return min_node;
}

// time complexity for Min() is O(log n) where n is the number of tasks
// this does not modify the heap and returns value of minimum key from the heap
// minimum key would only be from all the roots of the heap because of heap order property
int Min(BinomialHeap *T)
{
    if( T->head == NULL ) return 9999;
    HeapNode *bef = NULL; HeapNode *min_node = T->head;
    HeapNode *next_ = min_node; HeapNode *next_node = min_node->sibling;
    while(next_node != NULL)
    {
        if(next_node->key < min_node->key)
        {min_node = next_node; bef = next_;}
        next_ = next_node; next_node = next_node->sibling;
    }
    return min_node->key;
}


// time complexity for MyTaskScheduler is O(n * log n) where n is the number of tasks
// time complexity of removeMin, insert, min each is O(log n) and there are n tasks iterating through while loop
// hence overall time complexity is O(n * log n)
// while loop (just before second priority queue) iterates through n times because if a task goes into priority queue 2 or 3, it is removed from priority queue 1 (full_tree)
int TaskScheduler(char *f1, char *f2, int m )
{
    int p = 0;
    int array[9999];
    char t;
    int digit;
    FILE *fil = fopen(f1, "r");
    FILE *fil2 = fopen(f2, "w");
    if (fil==NULL){
        printf("file1 does not exist\n");
        return 1;}
    while (!feof(fil)) {
        if (fscanf(fil, "%d", &digit) == 1)
            array[p++] = digit;
        else{
            t = fgetc(fil);
            if(t != '\0' && t!= EOF){
                printf("input error when reading the attribute of the task %d\n", array[p-(p%4)]);
                return 1;}
            
        }
    }
    fclose(fil);
    
    for(int n=0; n < p-3; n += 4){
        if(array[n]<0 || array[n+1]<= 0 || array[n+2]< 0 || array[n+3]<= 0){
            printf("input error when reading the attribute of the task %d\n", array[n]);
            return 1;
        }
    }
    
    //Binomial heap full_tree with release time as keys
    //This is FIRST priority queue
    BinomialHeap *full_tree = newHeap();
    for (int k=0; k < p - 3; k += 4) {
        Insert(full_tree, array[k+2], array[k],array[k + 1],array[k + 2],array[k + 3]);
        full_tree->size++;
    }
    
    //Third priority queue with cores as items and keys as scheduling points
    //Initially keys are minimum release times
    BinomialHeap *core_tree = newHeap();
    int r = Min(full_tree);
    for(int j=1; j <= m; j++){
        Insert(core_tree, r, j, 0,0,0);
        core_tree->size++;
    }
    
    HeapNode *node;
    BinomialHeap *deadlines_tree = newHeap();
    while(full_tree->size > 0){
        int s1 = Min(full_tree);
        int s2 = Min(core_tree);
        int s = (s1 >= s2) ? s1 : s2;
        while(Min(full_tree)<=s){
            node = RemoveMin(full_tree);
            full_tree->size--;
            // Second priority queue with key as deadline of the task
            Insert(deadlines_tree,node->Dline,node->TaskName,node->Etime,node->Rtime,node->Dline);
            deadlines_tree->size++;
        }
        
        while(deadlines_tree->size >0){
            HeapNode *node_core;
            node_core = RemoveMin(core_tree); //key is scheduling point
            core_tree->size--;
            int core = node_core->TaskName;
            
            HeapNode *deadline_node;
            deadline_node = RemoveMin(deadlines_tree);
            deadlines_tree->size--;
            int f = ( deadline_node->Rtime >= node_core->key ) ? deadline_node->Rtime : node_core->key;
            Insert(core_tree, f + deadline_node->Etime,core,0,0,0);
            core_tree->size++;
            fprintf(fil2, "%d Core%d %d\n",deadline_node->TaskName, core, f);
            if(deadline_node->key < f + deadline_node->Etime){
                fprintf(fil2, "where task%d misses its deadline.\n", deadline_node->TaskName);
                return 0;}
            
            // to include all tasks before scheduling point
            while(Min(full_tree)<= Min(core_tree)){
                node = RemoveMin(full_tree);
                full_tree->size--;
                Insert(deadlines_tree,node->Dline,node->TaskName,node->Etime,node->Rtime,node->Dline);
                deadlines_tree->size++;
            }
        }
    }
    fclose(fil2);
    return 1;
}

//  /Users/sahilpunchhi/Desktop/COMP_9024_DSA/Assignments/Assignment_3/
int main() //sample main for testing
{ int i;
        i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4 );
        if (i==0) printf("No feasible schedule!\n");
        /* There is a feasible schedule on 4 cores */
        i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
        if (i==0) printf("No feasible schedule!\n");
        /* There is no feasible schedule on 3 cores */
        i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
        if (i==0) printf("No feasible schedule!\n");
        /* There is a feasible schedule on 5 cores */
        i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
        if (i==0) printf("No feasible schedule!\n");
        /* There is no feasible schedule on 4 cores */
        i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
        if (i==0) printf("No feasible schedule!\n");
        /* There is no feasible schedule on 2 cores */
        i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
        if (i==0) printf("No feasible schedule!\n");
        /* There is a feasible schedule on 2 cores */
    return 0;
}

