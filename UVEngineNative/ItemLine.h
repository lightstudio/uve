#pragma once 
#include "INI_BASE.h" 
 
namespace DCG__INI__FILE 
{ 
	class CItemLine : public CINI_BASE 
	{ 
	public: 
		CItemLine(void); 
		~CItemLine(void); 
	public: 
 	// 根据索引返回第几个Item的 item 名字(不返回 value )。index 从0开始计数，找不到返回变量 NOT_FOUND_ITEM 
		bool getValue(int index, string & RetValue); 
		bool getValue(const string & ItemName, string & RetValue); 
		void add_line(const string & Line); 
		void add_line(const string & ItemName, const string & value ); 
		bool get_item_and_value(const string & Line , string & item , string & value); 
		bool del_Item(const string & ItemName); 
		bool del_Item(int index); 
 
		bool getItemName( int index, string & item ); 
		bool getItemName( const string & ItemName, string & item ); 
 
		//获取某一行的内容，包括一些其他的文字 
		string getLine( int index ); 
		//获取所有行的行数 
		int getAllLineCount(); 
		//获取item的行数 
		int getItemCount(void); 
		// 清空所有的 item ，当参数 bIncludeOtherLine 为true时，此Section下所有的行（包括item的和其他文字的）都被删除。 
		//当参数 bIncludeOtherLine 为 false 时，只删除 item 的。其他文字保留 
		void clearAllItem(bool bIncludeOtherLine = false ); 
 
	protected: 
		LineInSection line_of_item; 
		int ITEM_COUNT; 
	}; 
};