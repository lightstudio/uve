#pragma once 
#include <string>
#include <list>
using namespace std;

 
namespace DCG__INI__FILE 
{ 
 
	struct LINE_IN_ITEM_RANGE //这是一个ITEM的结构 
	{ 
	public: 
		std::string item;  //名称 
		std::string trimedItem; //已经trim过的名称，是 trim(item) 的结果 
		std::string value; //值 
		bool isItem;  //是否是ITEM。在ITEM范围内，也有可能是一些无关的文字，这些文字被保留到 value 里，但是这些文字的 isItem 为 false 。 
	}; 
	typedef std::list<LINE_IN_ITEM_RANGE> LineInSection; //每一个 Section 内部的行 
 
 
	static const basic_string<CHAR>::size_type npos = -1; 
 
 
 
	class CINI_BASE 
	{ 
	public: 
		CINI_BASE(void); 
		~CINI_BASE(void); 
		void trim(string & str); 
		// 判断某行是否是section格式，即 [ ... ] 格式 
		bool isSection(const char * Line); 
		// 判断某行是否是Item格式，即有等号，而且等号左边有非空字符 
		bool isItem(const char * Line); 
		// 是否包含 等于 符号。Section 和 Item 写的时候需要判断，不能包含等于号 
		bool isContainEqu(const char * Line); 
		// 是否包含 等于 符号。Section 和 Item 写的时候需要判断，不能包含等于号 
		bool isContainEqu(const string & Line); 
		// 是否包含方括号 
		bool isContainBracket(const string & str); 
		// 判断是否是数字 
		bool isDigital(const string & str); 
		// 获取下一个空格的字符的指针 
		char * getNextBlank(char * Line); 
		// 获取下一个空格的字符的指针 
		size_t getNextBlank(const string & Line); 
		// 获取下一个非空字符的指针 
		char * getNextNotBlank(char * Line); 
		// 获取下一个非空字符的指针 
		size_t getNextNotBlank(const string & Line); 
		// 判断是否是bool字符串, 
		// TRUE = 0 
		// FALSE = 1 
		// error = -1 
		int isBool( const string & str ); 
 
		bool CompareStringsIgnoreCase( const string & str1, const string & str2 ); 
	}; 
}; 
