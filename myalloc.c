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

void split_space(struct block *current_node, int requested_size) {
    int padded_block_size = PADDED_SIZE(sizeof(struct block));
    int old_size = current_node->size;

    int remaining_space = old_size - requested_size - padded_block_size;
    int room_to_split = remaining_space >= 16;

    if (room_to_split) {

        struct block *new = PTR_OFFSET(current_node, requested_size + padded_block_size);

        new->size = current_node->size - (requested_size + padded_block_size);

        current_node->size = requested_size;

        new->in_use = 0;

        current_node->next = new;

        new->next = NULL;
    }
}

void myfree(void * p) {
    struct block *cur = PTR_OFFSET(p, -PADDED_SIZE(sizeof(struct block)));
    cur->in_use = 0;
    while (cur->next != NULL) {
        if (cur->in_use == 0 && cur->next->in_use == 0) {
            cur->size = cur->next->size + cur->size + PADDED_SIZE(sizeof(struct block));
            cur->next = cur->next->next;
        }
        else {
            cur = cur->next;
        }
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

    struct block *cur = head;
    while (cur != NULL) {
        if (cur->in_use == 0 && cur->size >= actual_size) {

            split_space(cur, actual_size);
            cur->in_use = 1;

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
void *p, *q, *r, *s;

p = myalloc(10); print_data();
q = myalloc(20); print_data();
r = myalloc(30); print_data();
s = myalloc(40); print_data();

myfree(q); print_data();
myfree(p); print_data();
myfree(s); print_data();
myfree(r); print_data();
}