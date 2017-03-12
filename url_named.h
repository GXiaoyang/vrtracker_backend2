#pragma once
#include <string>
#include <vector>

namespace base
{
	struct URL
	{
		URL() = default;

		URL(const URL &rhs)
			: m_name(rhs.m_name),
			m_full_path(rhs.m_full_path)
		{}

		URL(const char *name, const char *full_path)
			:
			m_name(name),
			m_full_path(full_path)
		{}

		const std::string &get_name() const { return m_name; };
		const std::string &get_full_path() const { return m_full_path; };

		URL make_child(const char *child_name) const
		{
			std::string temp(m_full_path.c_str());
			temp += "/";
			temp += child_name;
			return URL(child_name, temp.c_str());
		}

	private:
		std::string m_name;
		std::string m_full_path;
	};

	struct url_named
	{
		//url_named()
		//{}

		url_named(const URL &url)
			: m_url(url)
		{}
		std::string get_name() const { return m_url.get_name(); };
		std::string get_path() const { return m_url.get_full_path(); };
		const URL &get_url() const { return m_url; }
		void set_url(const URL&url) { m_url = url; }

	private:
		URL m_url;
	};

	template <typename T, class Allocator = std::allocator<char*>>
	struct named_vector : url_named, std::vector<T, Allocator>
	{
		template<typename... Args>
		named_vector(const URL &name, Args&&... args)
			:
			url_named(name),
			std::vector<T, Allocator>(std::forward<Args>(args)...)
		{}
		URL make_url_for_child(const char *child) { return get_url().make_child(child); }
	};

};