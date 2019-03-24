// the pc field only get updated during each task switch
// the ram index is added for convience of tracking the file pointer in RAM
// the professor said we could do it even tho it is not mentioned in the assignment description
typedef struct PCB{
    FILE *pc;
    int ramIndex;
} PCB;

// slightly change the attributes to add ram index
PCB* makePCB(FILE *p, int i);