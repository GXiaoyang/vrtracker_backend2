#pragma once
#include "platform.h"
#include <algorithm>

//
// macro to ensure that the cursor is up to date with it's state
// also defines a 'local_name' which holds a pointer to the result
//
#define CURSOR_ITER_NAME(local_name) local_name ## _iter	// temporary variable
#define CURSOR_SYNC_STATE(local_name, variable_name) \
SynchronizeChildVectors();\
auto & CURSOR_ITER_NAME(local_name) = iter_ref.variable_name;\
update_iter(CURSOR_ITER_NAME(local_name),\
	state_ref.variable_name,\
	m_context->GetCurrentFrame());\
auto *local_name = &CURSOR_ITER_NAME(local_name)->get_value();


// align the iterator node to the closest, rounding down , value in the history node for cursor_frame
template <typename T, typename U>
static void update_iter(T& cached_iterator, U &history_node, time_index_t cursor_frame)
{
	if (history_node.empty())
	{
		ABORT("error node is empty. this should happen to the cursor interfaces");
	}
	
	if (cached_iterator.initialized)
	{
		cached_iterator = history_node.last_item_less_than_or_equal_to_time(cursor_frame, cached_iterator);
	}
	else
	{
		cached_iterator = history_node.last_item_less_than_or_equal_to_time(cursor_frame);
	}
}

// strncmp for vector of char vs null terminated string
template <typename VectorT>
int util_char_vector_cmp(const char *pch, VectorT &v)
{
	if (v.size() == 0)
	{
		if (*pch == '\0')
			return 0;
		else
			return 1;
	}
	else
	{
		return strncmp(pch, &v[0], v.size());
	}
}


//
// common routine to return a vector into a size and count buffer
//  !NOTE this is the general case. there is a specialization for char* further down
//
template <typename VectorT, typename T>
inline bool util_vector_to_return_buf_rc(
	const VectorT *p,
	T *pRet,
	uint32_t unBufferCount,
	uint32_t *rc,
	typename std::enable_if<!std::is_same<T, char>::value, void>::type *a = nullptr)
{
	bool big_enough = false;
	uint32_t required_count = (uint32_t)p->size();

	if (pRet && unBufferCount > 0 && required_count > 0)
	{
		uint32_t bytes_to_write = std::min(unBufferCount, required_count) * sizeof(T);
		memcpy(pRet, p->data(), bytes_to_write);
		if (unBufferCount >= required_count)
		{
			big_enough = true;
		}
	}

	if (rc)
	{
		*rc = required_count;
	}
	return big_enough;
}

// copy up to buffer count bytes into rc
inline bool util_char_to_return_buf_rc(const char *val, uint32_t required_size, char *pRet, uint32_t unBufferCount, uint32_t *rc)
{
	bool big_enough = true;

	if (pRet && unBufferCount > 0)
	{
		if (required_size == 0)
		{
			pRet[0] = 0;
		}
		else
		{
			uint32_t bytes_to_write = std::min(unBufferCount, required_size);
			memcpy(pRet, val, bytes_to_write);
			if (bytes_to_write < required_size)
			{
				big_enough = false;
				pRet[unBufferCount - 1] = 0;
			}
		}
	}
	else
	{
		big_enough = false;
	}

	if (rc)
	{
		*rc = required_size;
	}
	return big_enough;
}

template <typename VectorT, typename T>
inline bool util_vector_to_return_buf_rc(
		const VectorT *p,
		T *pRet,
		uint32_t unBufferCount,
		uint32_t *rc,
		typename std::enable_if<std::is_same<T, char>::value, void>::type *a = nullptr)
	
	{
		assert(p->size() == 0 || p->at(p->size() - 1) == 0); // (proof that p always has the trailing null (to make sure size is consistent for all strings))

		return util_char_to_return_buf_rc(p->data(), (uint32_t)p->size(), pRet, unBufferCount, rc);
	}
