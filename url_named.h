#pragma once
#include <string>
#include <vector>
#include "base_serialization.h"

namespace base
{
	struct URL
	{
		const static URL& EMPTY_URL;

		URL() = default;

		URL(const URL &rhs)
			:	m_name(rhs.m_name),
				m_full_path(rhs.m_full_path)
		{}

		URL(const std::string &name, const std::string &full_path)
			:
			m_name(name),
			m_full_path(full_path)
		{}

		bool operator ==(const URL &rhs) const
		{
			return (m_name == rhs.m_name) && (m_full_path == rhs.m_full_path);
		}
		bool operator !=(const URL &rhs) const
		{
			return (m_name != rhs.m_name) || (m_full_path != rhs.m_full_path);
		}

		const std::string &get_name() const { return m_name; };
		const std::string &get_full_path() const { return m_full_path; };

		URL make_child(const std::string &child_name) const
		{
			return URL(m_full_path + "/" + child_name, child_name);
		}

		void encode(EncodeStream &stream) const
		{
			stream.contiguous_container_out_to_stream(m_name);
			stream.contiguous_container_out_to_stream(m_full_path);
		}
		void decode(EncodeStream &stream)
		{
			stream.contiguous_container_from_stream(m_name);
			stream.contiguous_container_from_stream(m_full_path);
		}

	private:
		std::string m_name;
		std::string m_full_path;
	};

	struct url_named
	{
		url_named()
		{}

		url_named(const URL &url)
			: m_url(url)
		{}
		const std::string &get_name() const { return m_url.get_name(); };
		const std::string &get_path() const { return m_url.get_full_path(); };
		const URL &get_url() const { return m_url; }

		void set_url(const URL&url) { m_url = url; }
		void encode(EncodeStream &e) const
		{
			m_url.encode(e);
		}

		void decode(EncodeStream &e)
		{
			m_url.decode(e);
		}
		bool operator==(const url_named &rhs) const { return m_url == rhs.m_url; }
		bool operator!=(const url_named &rhs) const { return m_url != rhs.m_url; }
	private:
		URL m_url;
	};

	

};