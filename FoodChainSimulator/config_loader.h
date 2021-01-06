#pragma once
#include <iostream>
#include <string>
namespace EcoSim
{
	/// <summary>
	/// 加载配置文件。
	/// </summary>
	class ConfigLoader
	{
	private: 
		enum class ValueType
		{
			Bool, Int, Float, String, None
		};
		auto FindType(std::string name) -> ValueType;

	public:
		/// <summary>
		/// 给定输入流，从中读取配置文件。
		/// </summary> 
		auto Load(std::istream& stream) -> void { Load(std::move(stream)); }

		/// <summary>
		/// 给定输入流，从中读取配置文件。
		/// </summary> 
		auto Load(std::istream&& stream) -> void;

		/// <summary>
		/// 字符串化的配置信息
		/// </summary>
		/// <returns></returns>
		auto StrInfo() -> std::string;

		ConfigLoader();
	};
}
