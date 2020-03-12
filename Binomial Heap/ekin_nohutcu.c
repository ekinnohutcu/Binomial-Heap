#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//EKIN NOHUTCU 150116067
//REFERENCES: https://gist.github.com/chinchila/81a4c9bfd852e775f2bdf68339d212a2

//****AFTER RUN THE PROGRAM, USER HAVE TO ENTER THE q VALUE FOR EACH q value.*****



int q ;
int emax = 0;

int currentTime;

struct node {

    char id[20];
    int e;
    int time;
    int lastTime;
    int sumTime;
    int index;
    int isActive;
    int enqueueCount;
    double ce;
    double f;
    double priority;
    struct node *next;


};

typedef struct node Node;

Node nodeList[1023];
int totalNodeCount;


typedef struct __node_t__ {
    char id[20];
    int e;
    int time;
    int index;
    double ce;
    double f;
    double priority;
    int degree;
    struct __node_t__ *child;
    struct __node_t__ *parent;
    struct __node_t__ *brother;
} node_t;

typedef struct __heap_t__ {
    node_t *head;
} heap_t;


typedef struct node node;
node *header = NULL;


void chanceQ() {
    int a = 1;

    for (a = 1; a < 10; a++) {
        q = a;
    }
}


node_t *node_init(double priority, int e, int index, double ce, double f, char id[], int time) {
    node_t *node = malloc(sizeof(node_t));
    node->priority = priority;
    node->e = e;
    node->index = index;
    node->ce = ce;
    node->f = f;
    strcpy(node->id, id);
    node->time = time;
    node->degree = 0;
    return node;
}

heap_t *heap_init() {
    heap_t *heap = malloc(sizeof(heap_t));
    heap->head = NULL;
    return heap;
}

node_t *heap_merge(heap_t *heap1, heap_t *heap2) {
    if (heap1->head == NULL) return heap2->head;
    if (heap2->head == NULL) return heap1->head;

    node_t *head;
    node_t *h1It = heap1->head;
    node_t *h2It = heap2->head;
    node_t *tail;

    if (heap1->head->degree <= heap2->head->degree) {
        head = heap1->head;
        h1It = h1It->brother;
    } else {
        head = heap2->head;
        h2It = h2It->brother;
    }
    tail = head;
    while (h1It != NULL && h2It != NULL) {
        if (h1It->degree <= h2It->degree) {
            tail->brother = h1It;
            h1It = h1It->brother;
        } else {
            tail->brother = h2It;
            h2It = h2It->brother;
        }

        tail = tail->brother;
    }

    tail->brother = (h1It != NULL) ? h1It : h2It;

    return head;
}

node_t *heap_union(heap_t *original, heap_t *uni) {
    node_t *new_head = heap_merge(original, uni);

    original->head = NULL;
    uni->head = NULL;

    if (new_head == NULL)
        return NULL;

    node_t *prev = NULL;
    node_t *aux = new_head;
    node_t *next = new_head->brother;

    while (next != NULL) {
        if (aux->degree != next->degree || (next->brother != NULL && next->brother->degree == aux->degree)) {
            prev = aux;
            aux = next;
        } else {
            if (aux->priority < next->priority) {
                aux->brother = next->brother;
                next->parent = aux;
                next->brother = aux->child;
                aux->child = next;
                aux->degree++;
            } else {
                if (prev == NULL)
                    new_head = next;
                else
                    prev->brother = next;

                aux->parent = next;
                aux->brother = next->child;
                next->child = aux;
                next->degree++;

                aux = next;
            }
        }

        next = aux->brother;
    }

    return new_head;
}

void heap_insert(heap_t *heap, double priority, int e, int index, double ce, double f, char id[], int time) {
    node_t *node = node_init(priority, e, index, ce, f, id, time);
    heap_t *temp = heap_init();
    temp->head = node;
    heap->head = heap_union(heap, temp);
    free(temp);
}


void heap_remove(heap_t *heap, node_t *head, node_t *bef) {
    if (head == heap->head)
        heap->head = head->brother;
    else
        bef->brother = head->brother;

    node_t *new_head = NULL;
    node_t *child = head->child;

    while (child != NULL) {
        node_t *next = child->brother;
        child->brother = new_head;
        child->parent = NULL;
        new_head = child;
        child = next;
    }

    heap_t *temp = heap_init();
    temp->head = new_head;
    heap->head = heap_union(heap, temp);
    free(temp);
}


node_t *heap_min(heap_t *heap) {
    if (heap->head == NULL) return NULL;

    node_t *min = heap->head;
    node_t *min_prev = NULL;
    node_t *next = min->brother;
    node_t *next_prev = min;

    while (next != NULL) {
        if (next->priority < min->priority) {
            min = next;
            min_prev = next_prev;
        }

        next_prev = next;
        next = next->brother;
    }

    heap_remove(heap, min, min_prev);
    return min;
}

void heap_free(heap_t *h) {
    while (heap_min(h));
    free(h);
}


void addlinkedlist(char id1[], int e, int time, int index) {

    node *new_node = (node *) malloc(sizeof(node));

    node *last = header;

    strcpy(new_node->id, id1);
    new_node->e = e;
    new_node->time = time;
    new_node->index = index;
    new_node->next = NULL;
    new_node->isActive = 0;
    new_node->enqueueCount = 0;
    new_node->sumTime = 0;
    new_node->lastTime = time;

    if (header == NULL) {
        header = new_node;
        return;
    }

    while (last->next != NULL)
        last = last->next;

    last->next = new_node;


}

void heapRemove(node *removedNode) {
    node *temp = header;
    node *prevTemp = NULL;

    while (temp != NULL) {
        if (temp == removedNode) {
            if (prevTemp == NULL) {
                header = temp->next;
            } else {
                prevTemp->next = temp->next;
            }
            free(removedNode);
            break;
        }

        prevTemp = temp;
        temp = temp->next;
    }
}

int largestElement() {
    int max = 0;
    node *temp = header;

    while (temp != NULL) {
        if (max < temp->e)
            max = temp->e;
        temp = temp->next;
    }
    return max;
}


void printlist() {

    struct node *temp;
    temp = header;

    while (temp != NULL) {
        printf(" id: %s ", temp->id);
        printf(" e: %d ", temp->e);
        printf(" time: %d ", temp->time);
        printf(" index: %d", temp->index);
        printf(" ce: %f", temp->ce);
        printf(" f: %f\n", temp->f);

        temp = temp->next;
    }
}

void findCeAndF(int e, node_t *newNode) {


    // printf("--%s - %d - %d - %d\n", newnode->id, newnode->e, newnode->time, emax);

    newNode->ce = 1 / exp(-pow(((2.0 * e) / (3 * emax)), 3));
    newNode->f = newNode->ce * e;


    if (newNode->priority == 0.0) {
        newNode->priority = (double) newNode->e;

    } else {
        newNode->priority = newNode->f;

    }


}


void processHeap(node_t *temp) {

    // printf("--%s - %d - %d - %d\n", newnode->id, newnode->e, newnode->time, emax);

    temp->ce = 1 / exp(-pow(((2.0 * temp->e) / (3 * emax)), 3));
    temp->f = temp->ce * temp->e;


    if (temp->priority == 0.0) {
        temp->priority = (double) temp->e;

    } else {
        temp->priority = temp->f;
    }

    temp->e = temp->e - q;


}


float GetPriority(Node *node) {
    if (node->enqueueCount == 0) {
        return node->e;
    } else {
        return (1 / exp(-pow(((2.0 * node->e) / (3 * emax)), 3))) * node->e;

    }
}


Node *GetMinNode(Node *header) {
    Node *tempHeader = header;

    Node *minNode = NULL;


    //  printf("%d", tempHeader->time);

    while (tempHeader != NULL) {
        if (tempHeader->isActive == 0) {
            tempHeader = tempHeader->next;
            continue;
        }

        if (minNode == NULL) {
            if (tempHeader->e > 0)
                minNode = tempHeader;
        } else {
            if (tempHeader->e <= 0) {
                tempHeader = tempHeader->next;
                continue;
            }


            float min = GetPriority(minNode);
            float temp = GetPriority(tempHeader);


            if (min == temp) {
                if (tempHeader->time < minNode->time)
                    minNode = tempHeader;
            } else if (temp < min) {
                minNode = tempHeader;
            }
        }

        tempHeader = tempHeader->next;
    }

    return minNode;


}


int main() {


    heap_t *heap = heap_init();
    char p[20];
    int e;
    int time;
    int index = 1;

    FILE *fptr;

    fptr = fopen("input.txt", "r");


    while (!feof(fptr)) {

        fscanf(fptr, "%s", p);
        fscanf(fptr, "%d", &e);
        fscanf(fptr, "%d", &time);

        addlinkedlist(p, e, time, index);
        index++;
    }
    fclose(fptr);

    printf("ENTER THE q VALUE: ");
    scanf("%d", &q);


    emax = largestElement();
    // assignFirstValues();
    // printlist();

    int currentTime = header->time;

    node *list = header;


    node_t *heapTemp;

    totalNodeCount = 0;
    currentTime = 0;

    node *currentNode = list;
    node *headerNode = list;

    while (currentNode != NULL || GetMinNode(headerNode) != NULL) {
        do {
            if (currentNode == NULL)
                break;

            if (currentNode->time <= currentTime) {
                currentNode->isActive = 1;
                currentNode = currentNode->next;


                if (currentNode == NULL)
                    break;
            }


        } while (currentNode->time <= currentTime);


        float timeStep = q;
        if (GetMinNode(headerNode) != NULL) {
            Node *minNode = GetMinNode(headerNode);
            if (minNode->e > q) {
                timeStep = q;
            } else {
                timeStep = minNode->e;
            }


            minNode->e -= timeStep;
            minNode->enqueueCount++;
            minNode->sumTime += currentTime - minNode->lastTime;
            minNode->lastTime = currentTime + timeStep;

           // printf("%s  %d --- ", minNode->id, minNode->e);
        }

        currentTime += timeStep;

    }

    float sum = 0;
    float counter = 0;
    while (list != NULL) {
        counter++;
        sum += list->sumTime;
        printf("%d \n", list->sumTime);

        list = list->next;
    }

    printf("\n");
    printf("AWT---> %f", sum / counter);



    /*  while (list != NULL) {


          if (currentTime == list->time) {
              heap_insert(heap, list->priority, list->e, list->index, list->ce,  list->f, list->id, list->time);
              heapTemp = heap_min(heap);
              processHeap(heapTemp);

          }

          if(heap != NULL){

              heap_insert(heap, heapTemp->priority, heapTemp->e, heapTemp->index, heapTemp->ce, heapTemp->f, heapTemp->id, heapTemp->time);

              printf("%f\n", heapTemp->priority, heapTemp->index);
              processHeap(heapTemp);

              if(heapTemp->e > 0 && list->next->time != currentTime){
                  heap_insert(heap, heapTemp->priority, heapTemp->e, heapTemp->index, heapTemp->ce, heapTemp->f, heapTemp->id, heapTemp->time);
                  //printf("%f\n", heapTemp->priority, heapTemp->index);
                  processHeap(heapTemp);

              }else if(heapTemp->e <= 0){
                      list = list->next;

              }


          }

          heapTemp = heap_min(heap);
          currentTime++;
          printf("current time: %d ",currentTime);

          if(list == NULL){
              break;
          }

      }*/


}


