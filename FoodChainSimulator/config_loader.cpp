#include "config_loader.h"
#include <map>
#include <algorithm>
#include "game.h"
#include "sheep.h"
#include "grass.h"
#include "wolf.h"
#include "random_gen.h"
#include "display.h"
namespace EcoSim
{
	std::regex ConfigLoader::Lexer::r_section("^\\[[._a-zA-Z][._a-zA-Z0-9]*\\]");
	std::regex ConfigLoader::Lexer::r_comment("^#.*?$");
	std::regex ConfigLoader::Lexer::r_equal("^=");
	std::regex ConfigLoader::Lexer::r_space("^[\\s]+");
	std::regex ConfigLoader::Lexer::r_string("^'.*'");
	std::regex ConfigLoader::Lexer::r_bool("^(true)|(false)");
	std::regex ConfigLoader::Lexer::r_field("^[\\._a-zA-Z][\\._a-zA-Z0-9]*");
	std::regex ConfigLoader::Lexer::r_column("^:");
	std::regex ConfigLoader::Lexer::r_number("^[+-]?[0-9]+(\\.[0-9]+)?");

	template <typename T>
	using MetaMap = std::map<std::string, T*>;

	MetaMap<int> intMap = {
		  { "Global.seed", &seed}
		, { "Global.threadMinimumLoad", &Game::activeGame->threadMinimumLoad}
		, { "Global.displayMode", &Display::displayMode}
		, { "Grass.targetOffspringCount",&Grass::targetOffspringCount }
		, { "Sheep.consumptionHealthBenifit",&Sheep::consumptionHealthBenifit }
		, { "Sheep.initialHealth",&Sheep::initialHealth }
		, { "Sheep.maximumHealth",&Sheep::maximumHealth }
		, { "Sheep.minimumReproduceHealth",&Sheep::minimumReproduceHealth }
		, { "Sheep.starvationHealthHarm",&Sheep::starvationHealthHarm }
		, { "Sheep.targetOffspringCount",&Sheep::targetOffspringCount }
		, { "Wolf.consumptionHealthBenifit",&Wolf::consumptionHealthBenifit }
		, { "Wolf.initialHealth",&Wolf::initialHealth }
		, { "Wolf.maximumHealth",&Wolf::maximumHealth }
		, { "Wolf.minimumReproduceHealth",&Wolf::minimumReproduceHealth }
		, { "Wolf.starvationHealthHarm",&Wolf::starvationHealthHarm }
		, { "Wolf.targetOffspringCount",&Wolf::targetOffspringCount }
	};

	MetaMap<float> floatMap = {};
	MetaMap<bool> boolMap = {};
	MetaMap<std::string> stringMap = {};
	 
	auto ConfigLoader::Lexer::Lex(std::istream& stream) -> std::vector<Token>
	{
		std::vector<Token> result;

		std::vector<Rule> rules{
			Rule(r_section,TokenType::Section)
			,Rule(r_comment, TokenType::Comment)
			,Rule(r_equal, TokenType::Equal)
			,Rule(r_string, TokenType::String)
			,Rule(r_bool, TokenType::Bool)
			,Rule(r_field,TokenType::Field)
			,Rule(r_column,TokenType::Column)
			,Rule(r_number,TokenType::Number)
			,Rule(r_space, TokenType::Space) };


		std::string line;
		int lineNo = 1;
		while (std::getline(stream, line))// 处理一行
		{
			auto target_begin = line.begin();
			std::match_results<std::string::iterator> m;

			while (target_begin != line.end())// 处理target_begin到line.end()的部分
			{
				bool found_matching_rule = false;
				for (auto& rule : rules)// 尝试将此部分和一条规则匹配。
				{
					std::regex_search(target_begin, line.end(), m, rule.regex);
					if (m.length() > 0)// 可以匹配
					{
						result.push_back(Token(rule.tokenType, std::string(m[0].first, m[0].second)));
						found_matching_rule = true;
						break;
					}
				}
				if (!found_matching_rule)// 未找到匹配
				{
					throw std::exception(("Lexical Error at Line %d." + std::to_string(lineNo)).c_str());
				}
				// 找到匹配 重设target_begin
				target_begin = m[0].second;
			}
			lineNo++;
		}

		return result;
	}

	enum class ValueType
	{
		Bool, Int, Float, String, None
	};


	static auto FindType(std::string name) -> ValueType
	{
		if (std::find_if(intMap.begin(), intMap.end(), [=](std::pair<std::string, int*> pair) {return pair.first == name; }) != intMap.end())
		{
			return ValueType::Int;
		}

		if (std::find_if(floatMap.begin(), floatMap.end(), [=](std::pair<std::string, float*> pair) {return pair.first == name; }) != floatMap.end())
		{
			return ValueType::Float;
		}

		if (std::find_if(stringMap.begin(), stringMap.end(), [=](std::pair<std::string, std::string*> pair) {return pair.first == name; }) != stringMap.end())
		{
			return ValueType::String;
		}

		if (std::find_if(boolMap.begin(), boolMap.end(), [=](std::pair<std::string, bool*> pair) {return pair.first == name; }) != boolMap.end())
		{
			return ValueType::Bool;
		}

		return ValueType::None;
	}



	auto ConfigLoader::Load(std::istream&& stream) -> void
	{
		Lexer lexer;
		auto tokens = lexer.Lex(stream);

		auto newEnd = std::remove_if(tokens.begin(), tokens.end(), [](Token token) {return token.type == TokenType::Comment || token.type == TokenType::Space; });
		tokens.erase(newEnd, tokens.end());

		std::string section;
		for (auto iterator = tokens.begin(); iterator != tokens.end(); ++iterator)
		{
			auto& token = *iterator;

			if (token.type == TokenType::Section)
			{
				section = std::string(token.text.begin() + 1, token.text.end() - 1);
			}
			else if (token.type == TokenType::Field)
			{
				if (iterator + 1 != tokens.end()
					&& iterator + 2 != tokens.end()
					&& (iterator + 1)->type == TokenType::Equal
					&& (iterator + 2)->IsValue())
				{
					auto full_qualified_field_name = section + "." + token.text;
					auto type = FindType(full_qualified_field_name);
					auto value_str = (iterator + 2)->text;
					if (type == ValueType::Int)
					{
						try {
							*intMap[full_qualified_field_name] = std::stoi(value_str);
						}
						catch (...)
						{
							throw std::exception(("Integer Value Expected for Field `"
								+ full_qualified_field_name +
								"`. (Given: " + value_str + ")").c_str());
						}
						iterator += 2;
					}
					// TODO: add float, bool, string, etc.
					else
					{
						throw std::exception(("Unknown Field `" + full_qualified_field_name + "`").c_str());
					}
				}
				else
				{
					throw std::exception("':' or Value Expected After Field Name.");
				}
			}
			else
			{
				throw std::exception(("Unexpected Token `" + token.text + "`.").c_str());
			}
		}
	}
}