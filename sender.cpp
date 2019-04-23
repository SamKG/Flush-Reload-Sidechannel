
#include"util.hpp"

void send_bit_0(){
	NOP_PADDING
}
void send_bit_1(){
	NOP_PADDING
}
void send_bit_2(){
	NOP_PADDING
}
void send_bit_3(){
	NOP_PADDING
}
void send_bit_4(){
	NOP_PADDING
}
void send_bit_5(){
	NOP_PADDING
}
void send_bit_6(){
	NOP_PADDING
}
void send_bit_7(){
	NOP_PADDING
}
void send_bit_8(){
	NOP_PADDING
}

char* mapped;
ADDR_PTR sendbit_arr[9];
void send_bit(char c,int i){
	if (i == 0 || c & (1<<(i-1))){
		char tmp = mapped[sendbit_arr[i]];
	}
}
int main(int argc, char **argv)
{
	// Put your covert channel setup code here
	int fd = open("sender",O_RDONLY);
	mapped = (char*) mmap(NULL, 0x20000, PROT_READ, MAP_PRIVATE, fd, 0);
	int mult = 15;
	sendbit_arr[0] = ADDR_0 + mult*CACHE_LINE_SIZE;
	sendbit_arr[1] = ADDR_1 + mult*CACHE_LINE_SIZE;
	sendbit_arr[2] = ADDR_2 + mult*CACHE_LINE_SIZE;
	sendbit_arr[3] = ADDR_3 + mult*CACHE_LINE_SIZE;
	sendbit_arr[4] = ADDR_4 + mult*CACHE_LINE_SIZE;
	sendbit_arr[5] = ADDR_5 + mult*CACHE_LINE_SIZE;
	sendbit_arr[6] = ADDR_6 + mult*CACHE_LINE_SIZE;
	sendbit_arr[7] = ADDR_7 + mult*CACHE_LINE_SIZE;
	sendbit_arr[8] = ADDR_8 + mult*CACHE_LINE_SIZE;
	printf("Please type a message.\n");
	
	bool sending = true;
	while (sending) {
		char text_buf[128];
		fgets(text_buf, sizeof(text_buf), stdin);
		clock_t begin = clock();
		for (int i = 0 ; i < 128 ; i++){
			if (text_buf[i] == '\0'){
				break;
			}
			int c = text_buf[i];	
			CYCLES start_t = get_highres_time();
			while(get_highres_time() - start_t < TIME_SLOT){//pulse for 1000 cycles
				send_bit(c,0);
				send_bit(c,1);
				send_bit(c,2);
				send_bit(c,3);
				send_bit(c,4);
				send_bit(c,5);
				send_bit(c,6);
				send_bit(c,7);
				send_bit(c,8);
			}
			start_t = get_highres_time();
        		while(get_highres_time() - start_t <= TIME_SLOT*10){
			}
		}		
		clock_t end = clock();
		printf("TOTAL TIME SPENT: %f\n",((double)end - begin)/CLOCKS_PER_SEC);
		// Put your covert channel code here
	}

	printf("Sender finished.\n");

	return 0;
}


