#include "../include/vm.h"


Object *create_new_object(Vm *vm, ObjectType _type)
{
    Object *object = malloc(sizeof(Object)); 
    object->type = _type;
    object->marked = NON_MARKED;

    printf("Created new object!\n");
    if (vm->numObjects == vm->maxObjects) {

        gc(vm);
        printf("Reached maximum threshhold, garbage collection proceeded!\n");
    }

    vm->numObjects++;

    object->next = vm->firstObject;
    vm->firstObject = object;

    return object;
}




Vm *create_new_vm(void)
{
    Vm *vm = malloc(sizeof(Vm));
    vm->stackSize = 0;

    vm->numObjects = 0;
    vm->maxObjects = INITIAL_GC_THRESHOLD;
    vm->firstObject = NULL;
    return vm;
}



/**
 * @brief Private Function Section
 * 
 */


/**
 * @brief 
 * 
 * @param vm 
 * @param object 
 */
static void push(Vm *vm, Object *object)
{
    if (vm->stackSize > STACK_MAX) {
        fprintf(stderr,"Stackoverflow!\n");
        exit(EXIT_FAILURE);
    }

    vm->stack[vm->stackSize++] = object;
}

Object *pop(Vm *vm)
{
    if (vm->stackSize <= 0) {
        fprintf(stderr,"Stackunderflow!\n");
        exit(EXIT_FAILURE);
    }

    return vm->stack[--vm->stackSize];    
}

static void mark(Object* object) {

    /* If already marked, we're done. Check this first
        to avoid recursing on cycles in the object graph. */
    if (object->marked) return;

    object->marked = MARKED;

    if (object->type == OBJ_PAIR) {
        mark(object->head);
        mark(object->tail);
    }
}

/**
 * @brief Public Function Section
 * 
 */



/**
 * @brief 
 * 
 */
void push_int(Vm *vm, int intValue) {
    Object *object = create_new_object(vm, OBJ_INT);
    object->value = intValue;
    push(vm, object);
}

Object *push_pair(Vm *vm) {
    Object *object = create_new_object(vm, OBJ_PAIR);
    object->tail = pop(vm);
    object->head = pop(vm);

    push(vm, object);
    return object;
}



void mark_all(Vm *vm)
{
    for (int i = 0; i < vm->stackSize; i++) {
        mark(vm->stack[i]);
    }
}



void sweep(Vm *vm)
{
    Object **object = &vm->firstObject;
    while (*object) {
        if (!(*object)->marked) {
            /* This object wasn't reached, so remove it from the list
                and free it. */
            Object* unreached = *object;

            *object = unreached->next;
            printf("Collecting!\n");
            free(unreached);
        } 
        else 
        {
            /* This object was reached, so unmark it (for the next GC)
                and move on to the next. */
            (*object)->marked = NON_MARKED;
            object = &(*object)->next;
        }
    }
    printf("Sweeped!\n");
}


void gc(Vm *vm) 
{
    int numObjects = vm->numObjects;

    mark_all(vm);
    sweep(vm);

    vm->maxObjects = numObjects * 2;
}