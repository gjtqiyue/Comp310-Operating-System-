// the pc field only get updated during each task switch
// the ram index is added for convience of tracking the file pointer in RAM
// the professor said we could do it even tho it is not mentioned in the assignment description
typedef struct PCB{
    FILE *pc;   //current position in the file
    int ramIndex;
    int pageTable[10];
    int PC_page;    //current page
    int PC_offset;  //current offset
    int pages_max;  //total number of pages
} PCB;

// slightly change the attributes to add ram index
PCB* makePCB(FILE *p);