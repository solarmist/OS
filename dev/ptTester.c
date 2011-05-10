/* 
 * File: ptTester.c
 * 
 * Author Brian Zier
 * 
 * 
 */

#include "pt.h"

void accessPage(int pid, int vpn);
void init_main_mem();
void print_mem();

main(int argc, char *argv[])
{
  init_main_mem();
  init_pg_tbl();
  
  print_mem();
  
  //print_page_table();
  printf("\r\n--------------------------------------------------------\r\n");
  
  accessPage(1, 4);
  accessPage(2, 3);
  accessPage(1, 2);
  accessPage(1, 4);
  accessPage(0, 6);
  accessPage(3, 6);
}

void accessPage(int pid, int vpn)
{
  printf("\r\nLooking up virtual page number %d for process %d\r\n", vpn, pid);
  
  print_page_table();
  
  int physPgNm = lookup(pid, vpn, 0);
  
  printf("\r\nPhysical page number is: %d\r\n", physPgNm);
  
  print_mem();
  
  printf("\r\n--------------------------------------------------------\r\n");
}

void init_main_mem()
{
  int i,j;
  for(i = 0; i < MAXPRO; i++)
    for(j = 0; j < MAXMEM; j++)
      mem[i][j] = -1;
}

void print_mem()
{
  printf("\r\n------------------\r\n|%c[%d;%dm   MAIN MEMORY  %c[%dm|\r\n|----------------|\r\n", 27, 1, 42, 27, 0);
  
  int i,j;
  for(i = 0; i < MAXPRO; i++)
  {
//    printf("| Mem column: %2d |\r\n", i);
    for(j = 0; j < MAXMEM; j++)
    {
      if(j % PAGESIZE == 0)
      {
        printf("|                |\r\n", j / PAGESIZE);
        printf("|-- PhysPage %d --|\r\n", j / PAGESIZE);
        printf("|                |\r\n", j / PAGESIZE);
        printf("|PhysAddr | Value|\r\n");
        printf("|-------- | -----|\r\n");
      }
      printf("|%5d:   | %5d|\r\n", j, mem[i][j]);
    }
  }
  
  printf("------------------\r\n");
}

