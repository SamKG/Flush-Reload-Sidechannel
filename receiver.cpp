#include"util.hpp"
int main(int argc, char **argv)
{
	// Put your covert channel setup code here
	int fd = open("sender",O_RDONLY);
	char* mapped = (char*) mmap(NULL, 0x2000, PROT_READ, MAP_PRIVATE, fd, 0);
	// I think using uint64_t prevents prefetching
	int num_ptrs = 0;
	uint64_t probe_ptrs[100];
	int mult = 10;
	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + 0x1146 + mult*CACHE_LINE_SIZE);
	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + 0x16cd + mult*CACHE_LINE_SIZE);
//	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + 0x1c54 + mult*CACHE_LINE_SIZE);
//	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + 0x15cb + mult*CACHE_LINE_SIZE);
//	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + 0x1752 + mult*CACHE_LINE_SIZE);
//	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + 0x18d9 + mult*CACHE_LINE_SIZE);
//	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + 0x1a60 + mult*CACHE_LINE_SIZE);
//	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + 0x1be7 + mult*CACHE_LINE_SIZE);
//	probe_ptrs[num_ptrs++] = (uint64_t)(mapped + 0x1d6e + mult*CACHE_LINE_SIZE);
	printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");
	int hitcnt = 0;
	bool listening = true;
	while (listening) {
		//spy flushes
		for (int i = 0 ; i < num_ptrs ; i++){
			flush_one_block(probe_ptrs[i]);
		}
		//spy waits TIME_SLOT time
		CYCLES start_t = get_highres_time();
		while(get_highres_time() - start_t <= TIME_SLOT){
		}
		//spy reloads
		int hits[100];
		for (int i = 0 ; i < num_ptrs ; i++){
			CYCLES time = measure_one_block_access_time(probe_ptrs[i]);
			if ( time < TIME_CUTOFF){
				hits[i] = 1;
				printf("HIT %d\t%d\t%d\n",i,hitcnt++,time);
			}
			else {
				hits[i] = 0;
			}
		}
		
		if (hits[0]){
			// 0 bit means we have a message!
			int bitstr = 0;
			for (int i = 1 ; i < num_ptrs ; i++){
				if (hits[i]){
					printf("HIT BIT %d\n",i);
					bitstr |= 1<<(i-1);	
				}
			}
			printf("%c",(char)bitstr);
		}

	}

	printf("Receiver finished.\n");

	return 0;
}


