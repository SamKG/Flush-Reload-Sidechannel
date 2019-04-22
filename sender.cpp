
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

ADDR_PTR sendbit_arr[9];
void send_bit(char c,int i){
	CYCLES start_t = get_highres_time();
	while(get_highres_time() - start_t < TIME_SLOT){
		if (c & (1<<(i-1))){
			(( void (*)(void) )sendbit_arr[i])();
		}
	}
}
int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	sendbit_arr[0] = (ADDR_PTR) send_bit_0;
	sendbit_arr[1] = (ADDR_PTR) send_bit_1;
	sendbit_arr[2] = (ADDR_PTR) send_bit_2;
	sendbit_arr[3] = (ADDR_PTR) send_bit_3;
	sendbit_arr[4] = (ADDR_PTR) send_bit_4;
	sendbit_arr[5] = (ADDR_PTR) send_bit_5;
	sendbit_arr[6] = (ADDR_PTR) send_bit_6;
	sendbit_arr[7] = (ADDR_PTR) send_bit_7;
	sendbit_arr[8] = (ADDR_PTR) send_bit_8;
	printf("Please type a message.\n");
	
	bool sending = true;
	while (sending) {
		char text_buf[128];
		fgets(text_buf, sizeof(text_buf), stdin);
		for (int i = 0 ; i < 1 ; i++){
			if (text_buf[i] == '\0'){
				break;
			}
			int c = text_buf[i];	
			printf("%c\n",c);
			CYCLES start_t = get_highres_time();
			while(get_highres_time() - start_t < TIME_SLOT){
				send_bit_0();
			}
			send_bit(c,1);
        		while(get_highres_time() - start_t <= TIME_SLOT*20){
			}
		}		
		// Put your covert channel code here
	}

	printf("Sender finished.\n");

	return 0;
}


