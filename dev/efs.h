#include <stdlib.h>
#include "efsLL.h"

#define DISKSIZE    BLOCKSIZE * 14        // Total size of the disk
#define NUMBLOCKS   DISKSIZE / BLOCKSIZE  // Total number of blocks on the disk

struct fileNode *fileList = NULL;
struct blockNode *freeBlockList = NULL;


/* 
 * struct blockNode * add_file(char *filename)
 * Description:
 *    add a new file with the specified filename to the fileList
 * Input:
 *    char *filename : the name of the file to add
 * Output:
 *    a pointer to the first node in the blockList
 */
struct fileNode * add_file(char *filename, int numBlocks)
{
  // Add a new file to the fileList and get a pointer to the first block.  
  struct fileNode * newFileNode = malloc(sizeof(struct fileNode *));
  newFileNode = add_file_node(&fileList, filename, numBlocks);
  return newFileNode;
}


struct fileNode * get_file(char *filename)
{
  struct fileNode * file;
  file = find_file(&fileList, filename);
  return file;
}


/* 
 * struct block * get_free_block_node()
 * Description:
 *    Get the first free blockNode from the freeBlockList, then remove it from the freeBlockList
 * Input:
 *    none
 * Output:
 *    The first available free blockNode
 */
struct blockNode * get_free_block_node()
{
  struct blockNode * freeNode;
  int blockIndex = 0; // Just want the first free node in the freeBlockList

  freeNode = get_block_node(&freeBlockList, blockIndex);
  delete_block_node(&freeBlockList, freeNode->block);
  return freeNode;
}


/* 
 * void init_disk(block disk[])
 * Description:
 *    Initialize the passed in disk. Assumes that the disk was created using the constants defined.
 * Input:
 *    block disk[] : an array of blocks. Assumes that the array is of size NUMBLOCKS.
 * Output:
 *    All entries in block disk[] will be initialized to -1.
 */
void init_disk(struct block disk[])
{
  // Allocate memory for the head node in freeBlockList
  freeBlockList = malloc(sizeof(struct blockNode));
  
  // Initialize all blocks in the disk and add all blocks to freeBlockList
  int i, j;
  for(i = 0; i < NUMBLOCKS; i++)
  {
    for(j = 0; j < BLOCKSIZE; j++)
      disk[i].instructions[j] = -1;
    disk[i].blockNum = i;
    add_block_node(&freeBlockList, &disk[i]);
  }
  
  // Allocate memory for the head node in fileList
  fileList = malloc(sizeof(struct fileNode));
}


/* 
 * void print_block_list(struct blockNode *head)
 * Description:
 *    Print out all the data in all the blocks in this blockList
 * Input:
 *    struct blockNode *head : Pointer to the first node in this blockList
 * Output:
 *    Screen output of the data in the blockList
 */
void print_block_list(struct blockNode *blockList)
{
  if(blockList == NULL)
    print_error(LIST_EMPTY);
  else
    while(blockList != NULL)
    {
      printf("Block #%d:", blockList->block->blockNum);
      printf("blockList: %p\n", blockList);
      printf("blockList->block: %p", blockList->block);
      int i;
      for(i = 0; i < BLOCKSIZE; i++)
        ;
      //  printf(" %d", blockList->block->instructions[i]);
      printf("\n");
      blockList = blockList->nextBlock;
    }
}


/* 
 * void print_disk(struct block disk[])
 * Description:
 *    print out the contents of the disk
 * Input:
 *    struct block disk[] : The disk whos content's you wish to print
 * Output:
 *    screen output of all data stored in all blocks on the disk
 */
void print_disk(struct block disk[])
{
  int i, j;
  for(i = 0; i < NUMBLOCKS; i++)
  {
    printf("Block #%d", i);
    for(j = 0; j < BLOCKSIZE; j++)
      printf(" %d", disk[i].instructions[j]);
    printf("\n");
  }
}


/* 
 * void print_file_list(struct fileNode *head)
 * Description:
 *    Print out all the file currently stored on the disk
 * Input:
 *    struct fileNode *head : Pointer to the first node in the fileList
 * Output:
 *    Screen output of the files in the fileList
 */
void print_file_list(struct fileNode *head)
{
  if(head == NULL)
    print_error(LIST_EMPTY);
  else
  {
    printf("Filename      Blocks\n");
    while(head != NULL)
    {
      printf("%s%11d\n", head->filename, head->numBlocks);
      head = head->nextFile;
    }
  }
}


