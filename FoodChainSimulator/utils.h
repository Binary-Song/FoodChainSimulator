#ifndef _UTILS_H_
#define _UTILS_H_
#include <vector>
#include <cassert>
namespace EcoSim {

	struct ColorCode
	{
		static const int Red = 0x0004;
		static const int Blue = 0x0001;
		static const int Green = 0x0002;
		static const int Intensity = 0x0008;
	};

	/// <summary>
	/// 将前n项随机插入进全部项中。
	/// </summary>
	/// <typeparam name="BidiIter">双向迭代器</typeparam>
	/// <param name="begin">头</param>
	/// <param name="end">尾后</param>
	/// <param name="n">洗牌的项数</param>
	/// <returns></returns>
	template<class BidiIter >
	BidiIter Shuffle(BidiIter begin, BidiIter end, size_t n) {
		size_t left = std::distance(begin, end);
		while (n--) {
			BidiIter r = begin;
			std::advance(r, rand() % left);
			std::swap(*begin, *r);
			++begin;
			--left;
		}
		return begin;
	}

	/// <summary>
	/// 随机选择n项。
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="target">从此列表中选择。</param>
	/// <param name="n">选择的项数，当n大于target.size()时，会全部选择。</param>
	/// <returns></returns>
	template<class T>
	std::vector<T> RandomSelect(std::vector<T> target, size_t n)
	{
		n = std::min(target.size(), n);
		auto end = Shuffle(target.begin(), target.end(), n);
		return std::vector<T>(target.begin(), end);
	}

	/// <summary>
	/// 随机选择1项。
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="target">从此列表中选择。</param> 
	/// <returns></returns>
	template<class T>
	T RandomSelect(std::vector<T> target)
	{
		assert(target.size() > 0);
		return RandomSelect(target, 1)[0];
	}

	/// <summary>
	/// std::shared_ptr专用的dynamic_cast
	/// </summary>
	/// <typeparam name="Derived"></typeparam>
	/// <typeparam name="Base"></typeparam>
	/// <param name="bp"></param>
	/// <returns></returns>
	template<class Derived, class Base>
	std::shared_ptr<Derived> sp_dynamic_cast(std::shared_ptr<Base> bp)
	{ 
		return std::dynamic_pointer_cast<Derived>(bp);
	}
}
#endif