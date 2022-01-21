#ifndef VM_H
#define VM_H

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


#define STACK_MAX 256
#define INITIAL_GC_THRESHOLD 5

#define NON_MARKED 0
#define MARKED 1

typedef enum ObjectType {
    OBJ_INT,
    OBJ_PAIR
} ObjectType;

struct Object{
    ObjectType type;
    unsigned char marked;

    //for keep track all objects
    struct Object* next;

    union {
        int value;


        struct 
        {
            struct Object *head;
            struct Object *tail;
        };
    };
};

typedef struct Object Object;





struct Vm {
    Object *stack[STACK_MAX];
    int stackSize;


    int numObjects;

    /* The number of objects required to trigger a GC. */
    int maxObjects;

    //Vm has the first node of Object list to keep track them
    Object *firstObject;
};

typedef struct Vm Vm;


Vm *create_new_vm(void);
Object *create_new_object(Vm *vm, ObjectType type);

void push_int(Vm *vm, int intValue);
Object *pop(Vm *vm);
Object *push_pair(Vm *vm);

void mark_all(Vm *vm);

void sweep(Vm *vm);
void gc(Vm *vm);
#endif