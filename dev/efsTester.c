/* Author : Jordan Bondo */

#include <stdio.h>
#include "efs.h"

/* Forward Declarations */
int save_file(char *filename);
int write(struct fileNode **fileListNode, int data[], int count, int writeMode, int offset);
/* End Forward Declarations */

//struct block disk[NUMBLOCKS];

int main(int argc, char *argv[])
{
  printf("\nfreeBlockList before initialization:");
  print_block_list(freeBlockList);
  printf("\nfile before initialization:");
  print_file_list(fileList);
  
  printf("\nInitializing Disk...\n");
  init_disk(disk);
  
  printf("\nfreeBlockList after initialization:\n");
  print_block_list(freeBlockList);
  printf("\nfile after initialization:\n");
  print_file_list(fileList);
  
  int status;
  
  printf("\nSaving file...\n");
  status = save_file("testFile.out");
  struct fileNode * file = get_file("testFile.out");
  
  if(status == 0 && file != NULL)
  {
    printf("File %s found\n", file->filename);
    printf("\nFilelist after adding \"testFile.out\":\n");
    print_file_list(fileList);
    printf("\nDisk after adding file:\n");
    print_disk(disk);
    printf("\nfreeBlockList after deleting nodes used for file:\n");
    print_block_list(freeBlockList);
    printf("\n%s blockList:\n", file->filename);
    print_block_list(file->blockList);
  }
  else
    printf("\nFile not saved successfully. Status returned %d. \n", status);
    
    
  printf("\nSaving file...\n");
  status = save_file("milk1.out");
  file = get_file("milk1.out");
  
  if(status == 0 && file != NULL)
  {
    printf("File %s found\n", file->filename);
    printf("\nFilelist after adding \"milk1.out\":\n");
    print_file_list(fileList);
    printf("\nDisk after adding file:\n");
    print_disk(disk);
    printf("\nfreeBlockList after deleting nodes used for file:\n");
    print_block_list(freeBlockList);
    printf("\n%s blockList:\n", file->filename);
    print_block_list(file->blockList);
  }
  else
    printf("\nFile not saved successfully. Status returned %d. \n", status);
  
  int offset = 13;
  printf("\n\nTesting overwrite on testFile.out...\nWriting 1-6 with an offset of %d\n", offset);
  struct fileNode * testFile = get_file("testFile.out");
  int d[] = {1, 2, 3, 4, 5, 6};
  write(&testFile, d, 6, OVERWRITE, offset);
  printf("\n%s blockList:\n", testFile->filename);
  print_block_list(testFile->blockList);
  
  
  printf("\n");
  return 0;
}


/* int save_file(char *filename)
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
  newFile = add_file(filename, numBlocks); // Add a new file to the list and get the pointer to that fileNode
  if(DEBUG) printf("newFile %p\n", newFile);
  newFile->blockList = get_free_block_node();
  write(&newFile, instructions, numInstructs, NEWFILE, 0);
  
  return 0;
}// end save_file()


/* int write(struct fileNode **fileListNode, int data[], int count, int writeMode)
 * Description:
 *    Write data to a file on the VFS. If writeMode is OVERWRITE, writing begins at
 *    the beginning of the file, but 
 * Input:
 *    struct fileNode **fileListNode : pointer to the file to be written to
 *    int data[] : Data to be written
 *    int count : How many elements to be written
 *    int writeMode : How to write to the file (Overwrite, append, etc).
 * Output:
 */
int write(struct fileNode **fileListNode, int data[], int count, int writeMode, int offset)
{
  int newFile;
  newFile = writeMode;
  struct blockNode *blockNode = (*fileListNode)->blockList;
  struct block *curBlock; // Declare a block pointer.
  if(DEBUG) printf("\nstart write : fileListNode->blockList->block %p\n", (*fileListNode)->blockList->block);
  if(DEBUG) printf("blockNode %p\n", blockNode);
  // Appending to end of file?
  int i; // First empty index in instructions array
  i = 0;
   // TODO: FIX APPEND.
  if(writeMode == APPEND)
  {
/*    // Forward the list to the last node*/
/*    while(blockNode->nextBlock != NULL)*/
/*      blockNode = blockNode->nextBlock;*/
/*    */
/*    curBlock = blockNode->block;*/
/*    // Does this block already have data in it?*/
/*    if(curBlock != NULL)*/
/*      // Find the first free instruction index*/
/*      for(i; curBlock->instructions[i] != -1 || i < BLOCKSIZE; i++);*/
/*    // Get the next free block on the disk*/
/*    else*/
/*      curBlock = get_free_block(); // Get the first available block on the disk*/
  }
  else // OVERWRITE or NEWFILE
  {
    if(offset != 0)
    {
      int k = 1;
      for(; k <= offset; k++)
        if(k % BLOCKSIZE == 0)
          blockNode = blockNode->nextBlock;
      i = offset % BLOCKSIZE;
    }
    curBlock = blockNode->block;
  }
      
  int j;
  for(j = 0; j < count; j++)
  {
    if((i == 0) && (newFile != NEWFILE))
    { 
      if(DEBUG) printf("curBlock %p\n", curBlock);
      if(DEBUG) printf("\nadding new blockNode...\n");
      if(blockNode->nextBlock == NULL)
        blockNode->nextBlock = get_free_block_node();
      blockNode = blockNode->nextBlock;
      curBlock = blockNode->block;
      if(DEBUG) printf("new blockNode %p\n", blockNode);
      if(DEBUG) printf("new block %p\n", curBlock);
    }
    if(DEBUG) printf("writing  %3d  to block  %2d\n", data[j], curBlock->blockNum);
    curBlock->instructions[i] = data[j];

    i++;
    // If i == BLOCKSIZE, we have written to the last availble instruction location and need to get a new blockNode next time.
    if(i == BLOCKSIZE)
      i = 0;
    // If this is a new file, set newFile to 0 so that when i == 0 we will get a new blockNode
    if(newFile == NEWFILE)
      newFile = 0;
  }
  if(DEBUG) printf("end write : fileListNode->blockList->block %p\n", (*fileListNode)->blockList->block);
  return 0;
} // end write()

