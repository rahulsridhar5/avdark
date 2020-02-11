/**
 * Cache simulation using a functional system simulator.
 *
 * Course: Advanced Computer Architecture, Uppsala University
 * Course Part: Lab assignment 1
 *
 * Original authors: UART 1.0(?)
 * Modified by: Andreas Sandberg <andreas.sandberg@it.uu.se>
 * Revision (2015, 2016, 2017, 2018): German Ceballos, Johan Janzen, Chris Sakalis
 *
 */

#include "avdark-cache.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#ifdef SIMICS
/* Simics stuff  */
#include <simics/api.h>
#include <simics/alloc.h>
#include <simics/utils.h>

#define AVDC_MALLOC(nelems, type) MM_MALLOC(nelems, type)
#define AVDC_FREE(p) MM_FREE(p)

#else

#define AVDC_MALLOC(nelems, type) malloc(nelems * sizeof(type))
#define AVDC_FREE(p) free(p)

#endif

/**
 * Cache block information.
 *
 * TODO: Fill in the data structure
 * HINT: You will probably need to change this structure
 */
struct avdc_cache_line {
        avdc_tag_t tag;
        int        valid;
        int        history;
};
int counter=0;

struct avdc_cache_set{
        avdc_tag_t tag[2];
        int        valid[2];
        int        history[2];
};


/**
 * Extract the cache line tag from a physical address.
 *
 * You probably don't want to change this function, instead you may
 * want to change how the tag_shift field is calculated in
 * avdc_resize().
 */
static inline avdc_pa_t
tag_from_pa(avdark_cache_t *self, avdc_pa_t pa)
{
        return pa >> self->tag_shift;
}

/**
 * Calculate the cache line index from a physical address.
 *
 * Feel free to experiment and change this function
 */
static inline int
index_from_pa(avdark_cache_t *self, avdc_pa_t pa)
{
        return (pa >> self->block_size_log2) & (self->number_of_sets - 1);
}

/**
 * Computes the log2 of a 32 bit integer value. Used in dc_init
 *
 * Do NOT modify!
 */
static int
log2_int32(uint32_t value)
{
        int i;

        for (i = 0; i < 32; i++) {
                value >>= 1;
                if (value == 0)
                        break;
        }
        return i;
}

/**
 * Check if a number is a power of 2. Used for cache parameter sanity
 * checks.
 *
 * Do NOT modify!
 */
static int
is_power_of_two(uint64_t val)
{
        return ((((val)&(val-1)) == 0) && (val > 0));
}

void
avdc_dbg_log(avdark_cache_t *self, const char *msg, ...)
{
        va_list ap;
        
        if (self->dbg) {
                const char *name = self->dbg_name ? self->dbg_name : "AVDC";
                fprintf(stderr, "[%s] dbg: ", name);
                va_start(ap, msg);
                vfprintf(stderr, msg, ap);
                va_end(ap);
        }
}

void 
replaceoldest(avdc_cache_set_t * set, int index, avdc_tag_t tag){
        
}

void
avdc_access(avdark_cache_t *self, avdc_pa_t pa, avdc_access_type_t type)
{
        /* TODO: Update this function */
        avdc_tag_t tag = tag_from_pa(self, pa);
        int index = index_from_pa(self, pa);
        int hit;
        int hit_i;

        avdc_assoc_t assoc = self->assoc;

        //avdc_cache_set_t * tempset = self->set[index];
	if(assoc == 2){
        	for(int i=0; i<2; i++){
                	if(self->set[index].valid[i] && self->set[index].tag[i] == tag){
                	        hit_i=i;
                	        hit = self->set[index].valid[i] && self->set[index].tag[i] == tag;   
				if(hit){
					printf("hit ");
					break;  
				}	
               		}
        	}	
        
        	if (!hit){
			printf("miss ");
			//hit = 0;
			//if(counter==0){
				///printf("init ");
                	//self->set[index].valid[0] = 1; 
                		//self->set[index].tag[0] = tag;
                		//self->set[index].history[0] = 1;
				//self->set[index].history[0] = 0;
                		//counter++;
        		//}
                	 if(self->set[index].history[0] > self->set[index].history[1]){
				printf("1 ");
                		self->set[index].valid[1] = 1; 
                		self->set[index].tag[1] = tag;
                		self->set[index].history[1] = 1;
                		self->set[index].history[0] = 0;
        		}
        		else if(self->set[index].history[1] > self->set[index].history[0]){
				printf("2 ");
                		self->set[index].valid[0] = 1; 
                		self->set[index].tag[0] = tag;
                		self->set[index].history[0] = 1;
                		self->set[index].history[1] = 0;
        		}
        		else {
				printf("3 ");
                		self->set[index].valid[0] = 1; 
                		self->set[index].tag[0] = tag;
                		self->set[index].history[0] = 1;
                		self->set[index].history[1] = 0;
        		}
//if (self->set[index].history[1] = self->set[index].history[0])
        	}
        
        	else{
               		for(int i=0;i<2;i++){
                        	        if(i!=hit_i)
                        	                self->set[index].history[i] = 0;
                        	        if(i == hit_i)
                        	                self->set[index].history[i] = 1;
                	} 
        	}
	}
	else if (assoc==1){
		hit = self->lines[index].valid && self->lines[index].tag == tag;
        	if (!hit) {
                	self->lines[index].valid = 1;
                	self->lines[index].tag = tag;
	}
}
        

        switch (type) {
        case AVDC_READ: /* Read accesses */
                avdc_dbg_log(self, "read: pa: 0x%.16lx, tag: 0x%.16lx, index: %d, hit: %d\n",
                             (unsigned long)pa, (unsigned long)tag, index, hit);
                self->stat_data_read += 1;
                if (!hit)
                        self->stat_data_read_miss += 1;
                break;

        case AVDC_WRITE: /* Write accesses */
                avdc_dbg_log(self, "write: pa: 0x%.16lx, tag: 0x%.16lx, index: %d, hit: %d\n",
                             (unsigned long)pa, (unsigned long)tag, index, hit);
                self->stat_data_write += 1;
                if (!hit)
                        self->stat_data_write_miss += 1;
                break;
        }
}

void
avdc_flush_cache(avdark_cache_t *self)
{
	if(self->assoc > 1){
        	  /* TODO: Update this function */
        	for (int i = 0; i < self->number_of_sets; i++) {
                	for (int j=0; j < 2; j++){
                		self->set[i].valid[j] = 0;
                		self->set[i].tag[j] = 0;
                		self->set[i].history[j] = 0;
       			}
        	}	
	}
	else if (self->assoc==1){
		for (int i = 0; i < self->number_of_sets; i++) {
                	self->lines[i].valid = 0;
                	self->lines[i].tag = 0;
        	}
	}

      
}


int
avdc_resize(avdark_cache_t *self,
            avdc_size_t size, avdc_block_size_t block_size, avdc_assoc_t assoc)
{
		/* TODO: Update this function */
        /* HINT: This function precomputes some common values and
         * allocates the self->lines array. You will need to update
         * this to reflect any changes to how this array is supposed
         * to be allocated.
         */

        /* Verify that the parameters are sane */
        if (!is_power_of_two(size) ||
            !is_power_of_two(block_size) ||
            !is_power_of_two(assoc)) {
                fprintf(stderr, "size, block-size and assoc all have to be powers of two and > zero\n");
                return 0;
        }

        /* Update the stored parameters */
        self->size = size;
        self->block_size = block_size;
        self->assoc = assoc;

        /* Cache some common values */
        self->number_of_sets = (self->size / self->block_size) / self->assoc;
        self->block_size_log2 = log2_int32(self->block_size);
 	self->tag_shift = self->block_size_log2 + log2_int32(self->number_of_sets);       


	if(assoc == 2){   
		/* (Re-)Allocate space for the tags array */
        		if (self->set)
        	        	AVDC_FREE(self->set);
			
			

		self->set = AVDC_MALLOC(self->number_of_sets, avdc_cache_set_t);
        }
	else if (assoc==1){
		/* (Re-)Allocate space for the tags array */
        	if (self->lines)
                	AVDC_FREE(self->lines);
		self->lines = AVDC_MALLOC(self->number_of_sets, avdc_cache_line_t);
	}

        /* HINT: If you change this, you may have to update
         * avdc_delete() to reflect changes to how thie self->lines
         * array is allocated. */
       

        /* Flush the cache, this initializes the tag array to a known state */
        avdc_flush_cache(self);

        return 1;
}

void
avdc_print_info(avdark_cache_t *self)
{
        fprintf(stderr, "Cache Info\n");
        fprintf(stderr, "size: %d, assoc: %d, line-size: %d\n",
                self->size, self->assoc, self->block_size);
}

void
avdc_print_internals(avdark_cache_t *self)
{
        int i;

        fprintf(stderr, "Cache Internals\n");
        fprintf(stderr, "size: %d, assoc: %d, line-size: %d\n",
                self->size, self->assoc, self->block_size);

        for (i = 0; i < self->number_of_sets; i++)
		for(int j=0; j<self->assoc; j++){
                	fprintf(stderr, "tag: <0x%.16lx> valid: %d\n",
                        	(long unsigned int)self->set[i].tag[j],
                        	self->set[i].valid[j]);
		}
}

void
avdc_reset_statistics(avdark_cache_t *self)
{
        self->stat_data_read = 0;
        self->stat_data_read_miss = 0;
        self->stat_data_write = 0;
        self->stat_data_write_miss = 0;
}

avdark_cache_t *
avdc_new(avdc_size_t size, avdc_block_size_t block_size,
         avdc_assoc_t assoc)
{
        avdark_cache_t *self;

        self = AVDC_MALLOC(1, avdark_cache_t);

        memset(self, 0, sizeof(*self));
        self->dbg = 0;

        if (!avdc_resize(self, size, block_size, assoc)) {
                AVDC_FREE(self);
                return NULL;
        }

        return self;
}

void
avdc_delete(avdark_cache_t *self)
{
	if(self->assoc > 1){
        		if (self->set)
                		AVDC_FREE(self->set);

	}
	else if (self->assoc==1){
		 if (self->lines)
                	AVDC_FREE(self->lines);
	}
        AVDC_FREE(self);
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 8
 * indent-tabs-mode: nil
 * c-file-style: "linux"
 * compile-command: "make -k -C ../../"
 * End:
 */
