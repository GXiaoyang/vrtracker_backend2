// 

//
#pragma once
#include "BaseStream.h"
#include "log.h"
#include <cstdint>

struct FileStream : public BaseStream
{
	FileStream()
		: m_file(0)
	{}

	~FileStream()
	{
		if (m_file)
		{
			fclose(m_file);
		}
	}

	FileStream(FileStream &) = delete;

	bool open_file_for_write_plus(const char *filename)
	{
		if (m_file)
			fclose(m_file);
		m_file = fopen(filename, "wb+");
		if (!m_file)
		{
			log_printf("open of %s failed %d\n", filename, errno);
			return false;
		}
		else
		{
			return true;
		}
	}

	bool open_file_for_read(const char *filename)
	{
		if (m_file)
			fclose(m_file);
		m_file = fopen(filename, "rb");
		if (!m_file)
		{
			log_printf("open of %s for read failed %d\n", filename, errno);
			return false;
		}
		else
		{
			return true;
		}
	}

	uint64_t get_pos() const override
	{
		uint64_t pos = _ftelli64(m_file);
		return pos;
	}

	void set_pos(uint64_t pos) override
	{
		int rc = _fseeki64(m_file, pos, SEEK_SET);
		if (rc != 0)
		{
			log_printf("seek failed %d\n", errno);
		}
	}

	void reset_buf_pos() override
	{
		_fseeki64(m_file, 0, 0);
	}

	// write value to buf and advance pointer
	void write_to_stream(const void *src, size_t s) override
	{
		if (s > 0)
		{
			size_t ret = fwrite(src, s, 1, m_file);
			if (ret != 1)
			{
				log_printf("fwrite failed %d\n", errno);
			}
		}
	}

	// write internal value out to stream and advance pointer
	void read_from_stream(void *dest, size_t s) override
	{
		if (s > 0)
		{
			fread(dest, s, 1, m_file);
		}
	}
	FILE *m_file;
};

