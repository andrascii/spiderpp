#include "redirect_chain.h"

namespace spiderpp
{

void RedirectChain::addLoadResult(const LoadResult& hop)
{
	m_redirectChain.push_back(hop);
}

void RedirectChain::addLoadResult(LoadResult&& hop)
{
	m_redirectChain.emplace_back(std::move(hop));
}

bool RedirectChain::hasRedirectTo(const Url& url) const noexcept
{
	const auto predicate = [&url](const LoadResult& hop)
	{
		return url.compare(hop.url());
	};

	return std::find_if(m_redirectChain.begin(), m_redirectChain.end(), predicate) != m_redirectChain.end();
}

std::size_t RedirectChain::length() const noexcept
{
	return m_redirectChain.size();
}

LoadResult& RedirectChain::firstLoadResult() noexcept
{
	return m_redirectChain.front();
}

const LoadResult& RedirectChain::firstLoadResult() const noexcept
{
	return m_redirectChain.front();
}

LoadResult& RedirectChain::lastLoadResult() noexcept
{
	return m_redirectChain.back();
}

const LoadResult& RedirectChain::lastLoadResult() const noexcept
{
	return m_redirectChain.back();
}

LoadResult& RedirectChain::operator[](std::size_t idx) noexcept
{
	return m_redirectChain[idx];
}

const LoadResult& RedirectChain::operator[](std::size_t idx) const noexcept
{
	return m_redirectChain[idx];
}

bool RedirectChain::empty() const noexcept
{
	return m_redirectChain.empty();
}


RedirectChain::iterator RedirectChain::begin() noexcept
{
	return m_redirectChain.begin();
}

RedirectChain::const_iterator RedirectChain::begin() const noexcept
{
	return m_redirectChain.begin();
}

RedirectChain::const_iterator RedirectChain::cbegin() const noexcept
{
	return m_redirectChain.cbegin();
}

RedirectChain::iterator RedirectChain::end() noexcept
{
	return m_redirectChain.end();
}

RedirectChain::const_iterator RedirectChain::end() const noexcept
{
	return m_redirectChain.end();
}

RedirectChain::const_iterator RedirectChain::cend() const noexcept
{
	return m_redirectChain.cend();
}

}