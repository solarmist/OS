#include <stdio.h>
#include "efs.h"

#define APPEND 0 // begin writeing at the end of the file
#define OVERWRITE 1 // begin writing at the beginning of the file

int save_file(char *filename);
int write(struct fileNode **fileListNode, int data[], int count, int writeMode);

struct block disk[NUMBLOCKS];

int main(int argc, char *argv[])
{
  init_disk(disk);
  
  printf("\nfreeBlockList after initialization:\n");
  print_block_list(freeBlockList);
  printf("\nDisk after initialization:\n");
  print_disk(disk);
  
  int status = save_file("milk1.out");
  
  if(status == 0)
  {
    printf("\nFilelist after adding \"milk1.out\":\n");
    print_file_list(fileList);
    printf("\nfreeBlockList after deleting nodes used for file:\n");
    print_block_list(freeBlockList);
    printf("\nDisk after adding file:\n");
    print_disk(disk);
  }
  else
    printf("\nFile not saved successfully. Status returned %d\n", status);
  
  printf("\n");
  return 0;
}


/* 
 * int save_file(char *filename)
 * Description:
 *    Attempt to open a file on the physical disk. If it exists, read through it
 *    and count the number of lines (instructions) and number of blocks necessary.
 *    If disk space is available, rewind the file pointer and
 *    read all the instructions into an array. Add a new file to the fileList
 *    and call write() in OVERWRITE mode to write all the instructions to the disk.
 * Input:
 *    char *filename : Name of the file on the physical disk. This will also be used
 *    as the filename in the virtual disk.
 * Output:
 *    -1 : File could not be opened. Either it doesn't exist on the physical disk, 
 *          or somethign else happened.
 *    -2 : Insufficient disk space
 *     0 : File was successfully created and written to the virtual disk
 */
int save_file(char *filename)
{
  FILE *fd;
  fd = fopen(filename, "r");
  if(fd == NULL)
    return -1;

  int numBlocks = 1; // Start at one block because this is the first block.
  int numInstructs = 0; // Counter for the number of lines in the file
  int instruction = 0; // What the line says

  // Read the file through to the end
  int i;
  for(i = 0; fscanf(fd, "%d\n", &instruction) != EOF; i++)
  {
    // printf("Inst# %3d: %d\n", numInstructs, instruction);
    numInstructs++;
    
    // Check if we need to increment the number of blocks that will be used
    if(numInstructs % BLOCKSIZE == 0)
      numBlocks++;
  }
  // Enough available disk space?
  if(get_block_count(freeBlockList) < numBlocks)
  {
    fclose(fd);
    return -2;
  }
  rewind(fd);
  int instructions[numInstructs];
  for(i = 0; fscanf(fd, "%d\n", &instruction) != EOF; i++)
    instructions[i] = instruction;
  fclose(fd);

  struct fileNode *newFile; // Declare a fileNode pointer
  newFile = add_file(filename); // Add a new file to the list and get the pointer to that fileNode
  write(&newFile, instructions, numInstructs, OVERWRITE);
  
  return 0;
}


int write(struct fileNode **fileListNode, int data[], int count, int writeMode)
{
  struct blockNode *blockNode = (*fileListNode)->blockList;
  struct block *curBlock; // Declare a block pointer.

  if(blockNode == NULL)
  {
    blockNode = malloc(sizeof(struct blockNode));
    add_block_node(&blockNode, curBlock);
  }
  // printf("%p\n", blockNode);

  // Appending to end of file?
  int i; // First empty index in instructions array
  i = 0;
  if(writeMode == APPEND)
  {
    // Forward the list to the last node
    while(blockNode->nextBlock != NULL)
      blockNode = blockNode->nextBlock;
    
    curBlock = blockNode->block;
    // Does this block already have data in it?
    if(curBlock != NULL)
      // Find the first free instruction index
      for(i; curBlock->instructions[i] != -1 || i < BLOCKSIZE; i++);
    // Get the next free block on the disk
    else
      curBlock = get_free_block(); // Get the first available block on the disk
  }
  else // Overwrite
    curBlock = blockNode->block;
  
  int j;

  for(j = 0; j < count; j++)
  {
    if(i == 0)
    {
      curBlock = malloc(sizeof(struct block));
      curBlock = get_free_block();
    }
    curBlock->instructions[i] = data[j];
    i++;
    if(i == BLOCKSIZE)
    {
      blockNode = add_block_node(&blockNode, curBlock);
      blockNode->block = curBlock;
      i = 0;
    }
  }
  return 0;
}

