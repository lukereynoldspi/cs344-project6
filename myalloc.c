// Luke Reynolds

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(void) {

    print_data();

}

int myalloc(int bytes){

	if (head == NULL) {
    head = sbrk(1024);
    head->next = NULL;
    head->size = 1024 - PADDED_SIZE(sizeof(struct block));
    head->in_use = 0;
    }

    else {
        int actual_size = PADDED_SIZE(bytes);

        struct block *cur = head->next;
        cur->next = NULL;
        cur->size = actual_size;
        cur->in_use = 1;
    
	    int padded_block_size = PADDED_SIZE(sizeof(struct block));

    
        return PTR_OFFSET(cur, padded_block_size);
    }
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
        // printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}