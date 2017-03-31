#include "slab_allocator.h"
#include "BaseStream.h"
#include "MemoryStream.h"
#include "FileStream.h"
#include "log.h"

void test_stream(BaseStream &stream)
{
	// write at position 1023
	stream.set_pos(1023);
	char c = 'a';
	stream.write_to_stream(&c, 1);
	stream.set_pos(1023);
	char a;
	stream.read_from_stream(&a, 1);
	assert(a == 'a');

	// write at position 0
	stream.set_pos(0);
	c = 'x';
	stream.write_to_stream(&c, 1);
	stream.set_pos(0);
	stream.read_from_stream(&a, 1);
	assert(a == 'x');
}


void test_base_stream()
{
	char buf[1024];

	MemoryStream m(buf, sizeof(buf), false);
	test_stream(m);

	FileStream f;
	assert(f.open_file_for_write_plus("c:\\temp\\test.bin"));
	test_stream(f);
}

