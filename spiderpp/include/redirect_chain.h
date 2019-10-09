#pragma once

#include "url.h"
#include "load_result.h"

namespace spiderpp
{

class RedirectChain
{
public:
	using iterator = std::vector<LoadResult>::iterator;
	using const_iterator = std::vector<LoadResult>::const_iterator;

	void addLoadResult(const LoadResult& hop);
	void addLoadResult(LoadResult&& hop);

	bool hasRedirectTo(const Url& url) const noexcept;
	std::size_t length() const noexcept;

	LoadResult& firstLoadResult() noexcept;
	const LoadResult& firstLoadResult() const noexcept;

	LoadResult& lastLoadResult() noexcept;
	const LoadResult& lastLoadResult() const noexcept;

	LoadResult& operator[](std::size_t idx) noexcept;
	const LoadResult& operator[](std::size_t idx) const noexcept;

	bool empty() const noexcept;


	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;

	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;

private:
	std::vector<LoadResult> m_redirectChain;
};

}

Q_DECLARE_METATYPE(spiderpp::RedirectChain)
