#include"util.hpp"
int main(int argc, char **argv)
{
	// Put your covert channel setup code here
	int fd = open("sender",O_RDONLY);
	char* mapped = (char*) mmap(NULL, 0x20000, PROT_READ, MAP_PRIVATE, fd, 0);
	// I think using uint64_t prevents prefetching
	int num_ptrs = 0;
	ADDR_PTR probe_ptrs[100];
	int mult = 15;
	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + ADDR_0 + mult*CACHE_LINE_SIZE);
	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + ADDR_1 + mult*CACHE_LINE_SIZE);
	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + ADDR_2 + mult*CACHE_LINE_SIZE);
	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + ADDR_3 + mult*CACHE_LINE_SIZE);
	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + ADDR_4 + mult*CACHE_LINE_SIZE);
	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + ADDR_5 + mult*CACHE_LINE_SIZE);
	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + ADDR_6 + mult*CACHE_LINE_SIZE);
	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + ADDR_7 + mult*CACHE_LINE_SIZE);
	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + ADDR_8 + mult*CACHE_LINE_SIZE);
	printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");
	int hitcnt = 0;
	bool listening = true;
	int hits[100];
	while (listening) {
		//spy flushes
		for (int i = 0 ; i < 100 ; i++){
			hits[i] = 0;
		}
		//spy waits TIME_SLOT time
		for (int i = 0 ; i < SLOT_WINDOW_COUNT;  i++){
			for (int i = 0 ; i < num_ptrs ; i++){
				flush_one_block(probe_ptrs[i]);
			}
			CYCLES start_t = get_highres_time();
			while(get_highres_time() - start_t <= TIME_SLOT){
			}
			//spy reloads
			for (int i = 0 ; i < num_ptrs ; i++){
				CYCLES time = probe_one_block(probe_ptrs[i]);
				if ( time < TIME_CUTOFF){
					hits[i]++;
					//printf("HIT %d\t%d\t%d\n",i,hitcnt++,time);
				}
			}
		}
		
		if (hits[0] >= SLOT_HIT_CUTOFF){
			// 0 bit means we have a message!
			int bitstr = 0;
			for (int i = 1 ; i < num_ptrs ; i++){
				if (hits[i] >= SLOT_HIT_CUTOFF){
					//printf("HIT BIT %d\n",i);
					bitstr |= 1<<(i-1);	
				}
			}
			printf("%c",(char)bitstr);
			fflush(stdout);
		}

	}

	printf("Receiver finished.\n");

	return 0;
}


