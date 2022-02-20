// Luke Reynolds

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))

#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

struct block *head = NULL;

struct block {
    struct block *next;
    int size;     // Bytes
    int in_use;   // Boolean
};

void * myalloc(int bytes){

	if (head == NULL) {
    head = sbrk(1024);
    head->next = NULL;
    head->size = 1024 - PADDED_SIZE(sizeof(struct block)); // 1008 bytes
    head->in_use = 0;
    }
    int actual_size = PADDED_SIZE(bytes); // mult of 16
    int padded_block_size = PADDED_SIZE(sizeof(struct block)); // 16 bytes

    //struct block *cur = head->next;
    //head->in_use = 1;
    while (cur) {
        if (cur->in_use == 0 && (actual_size + padded_block_size <= cur->size)) {

            cur->size = actual_size;
            cur->in_use = 1;

            struct block *nxt = PTR_OFFSET(cur, padded_block_size + actual_size); // struct block to separate memory pools
            nxt->next = NULL;
            nxt->in_use = 0;
            nxt->size = cur->size - (actual_size + padded_block_size);

            cur->next = nxt;

            return PTR_OFFSET(cur, padded_block_size);
            }
            
        else {
            cur = cur->next;
        }
    }
        
    return NULL;
}

void print_data(void)
{
    struct block *b = head;

    if (b == NULL) {
        printf("[empty]\n");
        return;
    }

    while (b != NULL) {
        // Uncomment the following line if you want to see the pointer values
        //printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}

int main(void) {

    void *p;

    print_data();
    p = myalloc(16);
    print_data();
    p = myalloc(16);
    print_data();
    p = myalloc(16);
    print_data();
    p = myalloc(2000);
    print_data();
    printf("%p\n", p);

}