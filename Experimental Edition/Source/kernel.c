#include "mouse.c"
#include "console.c"
#include "interrupts.c"
#include "multiboot.h"
#include "vfs.c"
#include "cmos.c"
#include "pci.c"
#include "ahci.c"
#include "timer.c"
#include "mem.c"
#include "paging.c"
#include "ata.c"
#include "acpi.c"
#include "vmem.c"
#include "keyboard.c"

u32int initial_esp;
u8int calle=0;

void timertest()
{
   /* RectD(0,0,500,500,700,700,700);
    Mouse_Plot(mousex,mousey);
    DBuff();*/
}

void dbug()
{
	int a[4];
	printf("\n\tEnter the size of: ");
	for(int i=0;i<4;i++)
	{
		printf("\n\t\tvar %x: ",i+1);
		a[i]=getint();
	}
	uint32_t *temp1=(uint32_t*)malloc(a[0]),*temp2=temp1;
	//malloc(4096);
	uint32_t *test1=(uint32_t*)malloc(a[1]),*test2=test1;
	uint32_t *test3=(uint32_t*)malloc(a[2]),*test32=test3;
	uint32_t *test4=(uint32_t*)malloc(a[3]),*test42=test4;
	uint32_t *test5=(uint32_t*)malloc(1),*test52=test5;
	printf("\n\tLocation of var 1: %x, var 2: %x var 3: %x var 4: %x var 5: %x \n",temp1,test1,test3,test4,test5);
	printf("\tPutting Magic Numbers into first two addresses\n");
	for(int i=0;i<32;i++)
	{
		*temp1=4284;
		++temp1;
	}
	temp1=temp2;
	for(int i=0;i<100;i++)
	{
		*test1=100;
		++test1;
	}
	printf("\tDone, Now Reading what we just wrote, 4284 on first few memory, 100 on others\n");
	test1=test2;
	for(int i=0;i<32;i++)
	{
		printf(" %x ",*temp1);
		++temp1;
	}
	for(int i=0;i<100;i+=5)
	{
		printf(" %x ",*test1);
		++test1;
	}
	temp1=temp2;
	test1=test2;
	printf("If you just saw few 4284's and 100's and nothing else, no extra space; everything worked fine!\n");
	printf("Now Freeing the memory!\n");
	free(temp2);
	free(test2);
	for(int i=0;i<1000;i+=50)
	{
		printf(" %x ",*temp1);
		++temp1;
	}
	for(int i=0;i<100;i+=5)
	{
		printf(" %x ",*test1);
		++test1;
	}
	printf(" If you didnt saw any numbers above, It worked!!!\n");
	uint32_t *tmp1=(uint32_t*)malloc(a[0]),*tmp2=tmp1;
	//malloc(4096);
	uint32_t *tst1=(uint32_t*)malloc(a[1]),*tst2=tst1;
	printf("\tLocation of var 1: %x, var 2: %x \n",tmp1,tst1);
}

void kernel_early(struct multiboot *mboot_ptr,u32int initial_stack)
{
    initial_esp = initial_stack;
		console_init();
		mouseinit();
		printf("\nMouse Drivers initialized\n");
		keyboard_init();
		printf("\nKeyboard Drivers Initialized\n");
    init_descriptor_tables();
    printf("DESCRIPTOR TABLES INITIALIZED \n");
    printint(mboot_ptr->mmap_addr);
    printf("\n");
    printint(mboot_ptr->mmap_length);
    printf("\nAvailable memory: ");
    printint(mboot_ptr->mem_upper);
    printf("\nMemory Map:");
    maxmem=mboot_ptr->mem_upper;
    uint32_t *mmap=(uint32_t*)mboot_ptr->mmap_addr;
    mmap_info=(MemRegion_t*)mmap;//+mboot_ptr->size;

    for(int i=0;i<15;i++)
    {
        if(mmap_info->startLo==0) break;
        printf("region %i address: %x size: %x Bytes Type: %i (%s)\n",i,mmap_info->startHi,mmap_info->sizeHi,
               mmap_info->type,strMemoryTypes[mmap_info->type-1]);
        mmap_info++;
    }
		printf("\nInitializing Memory Manager!\n");
    mmap_info=(MemRegion_t*)mmap;//+mboot_ptr->size;*/
    Mapper();
		printf("\nEnabling ACPI!\n");
    initAcpi();
    if(!acpiEnable())
        printf("\nACPI Initialized\n");
    else printf("\nACPI CANT BE INITIALIZED\n");

		printf("\n\nEnumerating all devices on PCI BUS:\n");
    checkAllBuses();
		printf("\nEnabling Hard Disk");
		checkAHCI();
		if(!ahci_found) init_ata();
	  printf("\nEnabling Paging\n");
    initialise_paging();
    enable_paging();
  	printf("\n Paging Has been Enabled Successfully!");
		printf("\n Available Memory: %x KB\n",maxmem);
    asm volatile("sti");
    init_timer(50); //PIT WORKING
    printf("\nPIT TIMER Initialized\n");
    //setVesa(0x117);
   // init_timer_RTC()
	 //printf("\n Sizeof : uint32_t* %x, page_t %x ",sizeof(uint32_t*),sizeof(page_table_t));
   printf("\nLOADING MAIN KERNEL...\n");//
	 /*
	 uint32_t abc[4]={0,0,0,0};
	 for(int i=0;i<32*4;)
	 {
		 abc[i/32]|=(1<<(i%32));
		 if(i<32) i+=2;
		 else if(i<64) i+=3;
		else i+=6;
	 }
	 uint32_t tmp1=clearBits(abc,32);
	 printf("\n Test %x %x %x %x %x",abc[0],abc[1],abc[2],abc[3],tmp1);
	 tmp1=clearBits(abc,100);
	 printf(" %x",tmp1);
	 tmp1=clearBits(abc,200);
	 printf(" %x",tmp1);*/
	 mdbug=dbug;
	 vesa=setVesa;
	 printf("\n\n\tType shutdown to do ACPI shutdown (wont work on certain systems)");
	 printf("\n\tType mdbug to test the Memory Manager");
}

void kernel_start()
{
}

void kernel_main()
{
		printf("\n");
		char *inst=" ";
		uint8_t flg=0;
    while(!flg)
    {
				printf("\ncmd>");
				getline(inst);
				flg=console_manager(inst);//*/
    }
		while(1)
		{
			Mouse_Plot(mousex,mousey);
			DBuff();//*/
		}
}
