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

void * split_space(struct block *current_node, int requested_size) {
    int required_space = PADDED_SIZE(sizeof(requested_size)) + PADDED_SIZE(sizeof(struct block)) + 16;
    if (current_node->size >= required_space) {
        struct block *new = current_node->next;
        new->size = current_node->size - (PADDED_SIZE(requested_size) + (PADDED_SIZE(sizeof(struct block))));
        new->in_use = 0;
        new->next = NULL;

        current_node->size = PADDED_SIZE(requested_size);

    }
}

void * find_space(int bytes) {
    struct block *cur = head;
    bytes = bytes + GET_PAD(bytes);

    while(cur) {
        if (cur->size >= bytes) {
            split_space(cur, bytes);
            cur->in_use = 1;
            return cur;
        }
        cur = cur->next;

    }
    return NULL;


}

void myfree(void * p) {
    struct block *cur = head;
    while(cur) {
    }  
}
void * myalloc(int bytes){

	if (head == NULL) {
        head = sbrk(1024);
        head->next = NULL;
        head->size = 1024 - PADDED_SIZE(sizeof(struct block)); // 1008 bytes
        head->in_use = 0;
    }
    int actual_size = PADDED_SIZE(bytes); // mult of 16
    int padded_block_size = PADDED_SIZE(sizeof(struct block)); // 16 bytes

    bytes = bytes + actual_size;

    struct block *cur = head;
    while (cur) {
        if (cur->in_use == 0 && (actual_size + padded_block_size <= cur->size)) {

            cur->size = actual_size;
            cur->in_use = 1;

            struct block *nxt = PTR_OFFSET(cur, padded_block_size + actual_size); // struct block to separate memory pools
            nxt->next = NULL;
            nxt->in_use = 0;
            nxt->size = cur->size - (actual_size + padded_block_size);

            cur->next = nxt;

            find_space(bytes);

            return PTR_OFFSET(cur, padded_block_size);
            }
            
        cur = cur->next;
        
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

    p = myalloc(5);
    print_data();

    p = myalloc(2000);
    print_data();
}