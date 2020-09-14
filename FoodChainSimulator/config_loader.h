#pragma once
#include <iostream>
#include <string>
#include <regex> 
#include "utils.h"
namespace EcoSim
{
	class ConfigLoader
	{
	private:

		enum class TokenType
		{
			Section, Comment, Equal, String, Space, Field, Column, Number, Bool
		};

		struct Token
		{
			TokenType type;
			std::string text;
			Token(TokenType type, std::string text)
				:type(type), text(text) {}
			auto IsValue() -> bool
			{
				return type == TokenType::Bool || type == TokenType::String || type == TokenType::Number;
			}
		};

		class Lexer
		{ 
			struct Rule
			{
				std::regex& regex;
				TokenType tokenType; 
				Rule(std::regex& regex, TokenType tokenType) :regex(regex), tokenType(tokenType) {}
			};

			static std::regex r_section;
			static std::regex r_field;
			static std::regex r_comment;
			static std::regex r_equal;
			static std::regex r_string;
			static std::regex r_space;
			static std::regex r_column;
			static std::regex r_number;
			static std::regex r_bool;
		public: 

			auto Lex(std::istream& stream)->std::vector<Token>;
		}; 
	
	public:
		auto Load(std::istream& stream) -> void { Load(std::move(stream)); }
		auto Load(std::istream&& stream) -> void;
	};
}