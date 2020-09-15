#ifndef _UTILS_H_
#define _UTILS_H_
#include <vector>
#include <cassert>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <algorithm>
#include <random>
#include <memory>
#include "random_gen.h"
namespace EcoSim {

	std::mt19937& Randomizer();

	struct ColorCode
	{
		static const int Red = 0x0004;
		static const int Blue = 0x0001;
		static const int Green = 0x0002;
		static const int Intensity = 0x0008;
	};

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
		n = target.size() > n ? n : target.size();
		std::shuffle(target.begin(), target.end(), Randomizer());
		return std::vector<T>(target.begin(), target.begin() + n);
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



	std::wstring GetExeDir();
}
#endif