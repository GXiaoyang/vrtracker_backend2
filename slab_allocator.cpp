#include "slab_allocator.h"

int slab_allocator_base::slab_allocators_constructed;
int slab_allocator_base::slab_allocators_destroyed;
int slab_allocator_base::slab_allocators_leaks;
slab* slab_allocator_base::m_temp_slab;

int slab::slab_num_slabs;
int slab::slab_total_slab_page_allocs;
int slab::slab_total_slab_page_frees;

