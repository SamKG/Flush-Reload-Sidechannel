
#include "util.hpp"

/* Measure the time it takes to access a block with virtual address addr. */
CYCLES measure_one_block_access_time(ADDR_PTR addr)
{
	CYCLES cycles;

	asm volatile("mov %1, %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"mov %%eax, %%edi\n\t"
	"mov (%%r8), %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"sub %%edi, %%eax\n\t"
	: "=a"(cycles) /*output*/
	: "r"(addr)
	: "r8", "edi");	

	return cycles;
}
CYCLES get_highres_time(){
	CYCLES time;
	asm volatile(
	"lfence\n"
	"rdtsc\n"
	: "=a"(time));
	return time;
	
}

// probe straight from paper
CYCLES probe_one_block(ADDR_PTR adrs) {
  CYCLES time;

  asm __volatile__ (
    "  mfence             \n"
    "  lfence             \n"
    "  rdtsc              \n"
    "  lfence             \n"
    "  movl %%eax, %%esi  \n"
    "  movl (%1), %%eax   \n"
    "  lfence             \n"
    "  rdtsc              \n"
    "  subl %%esi, %%eax  \n"
    "  clflush 0(%1)      \n"
    : "=a" (time)
    : "c" (adrs)
    :  "%esi", "%edx");
  return time;
}

void wait_for_time(CYCLES time){
	CYCLES start_t = get_highres_time();
	while(get_highres_time() - start_t < time){
	}
}
void flush_one_block(ADDR_PTR addr){
	asm volatile(
	"clflush 0(%0)"
	: 
	: "r"(addr));
}
