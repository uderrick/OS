#include <stdio.h>
#include <stdlib.h>


#define BitsForPageNo 6
#define BitsForPageOffset 10

#define ProcessSize 24800

void init_page_table( int table[] , int no_page );
char* to_binary ( unsigned short addr );

unsigned short logical2physical( unsigned short logical_addr , int page_table[]);


int main()
{
	int no_page = 0;
	int *page_table = NULL;

	unsigned short logical_addr[] = {0x21dd,0x0829,0x08f0,0x0ad1,0x071e,0x0431,0x0fb8,0x0b18,0x38ab,0x07a0};

	int i = 0;

	/* maximum # of pages : 64 => Because a word size is 16bits and offset is 10bits. 
	 so, 16-10 = 6 , 2^6 = 64 entries.
	 page size : 1KB(2^10) => Because a page offset is 10bits.*/
	no_page = 25;
	//25 => 24800/1024 = 24.xxx for containing remaining part.
	page_table = (int*)malloc(no_page * sizeof(int));

	if(page_table == NULL ){
		printf("Failed to allocate memory!\n");
		exit(-1);
	}

	init_page_table(page_table, no_page);

	for(i=0 ; i<10 ; i++ ){
		unsigned short physical_addr = logical2physical( logical_addr[i] , page_table);
		printf("0x%04x (%s)",logical_addr[i], to_binary( logical_addr[i]));
		printf("--> 0x%04x (%s)\n", physical_addr , to_binary(physical_addr));
	}
	free( page_table );

	return 0;
	}
void init_page_table( int table[] , int no_page )
{
	for( int i = 0 ; i< no_page ; i++ )
	{
		table[i]  = 60 - i*2;
	}
}

char* to_binary ( unsigned short addr )
{
	static char bits[17];
	int i = 0;
	for(i=0;i<16;i++){
		bits[15-i] = '0'+(addr&1);
		addr >>= 1;
	}
	bits[16] = 0;

	return bits;
}
unsigned short logical2physical( unsigned short logical_addr , int page_table[]){

	unsigned short  page_number = logical_addr >>10;
	unsigned short  lo = logical_addr & 0x3ff;
	unsigned short  frame_number = page_table[ page_number ];
		
	unsigned short  physical_addr = 0;
		frame_number = frame_number << 10;
		physical_addr = frame_number | lo;

	return physical_addr;
}
	
