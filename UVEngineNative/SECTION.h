#pragma once 
#include "ItemLine.h" 
 
namespace DCG__INI__FILE 
{ 
	//单个 section 的结构 
	struct ONE_SECTION 
	{ 
		string section; 
		string trimedSection;	 
		CItemLine items; 
	}; 
 
	typedef list<ONE_SECTION> SECTION_LIST; 
 
 
	class CSECTION :	public CINI_BASE 
	{ 
	public: 
		CSECTION(void); 
		~CSECTION(void); 
 
	public: 
		bool get_SectionName (const string & Line, string & section ); 
		//增加一行，这一行的内容由 Line 传入 
		//参数 bIsSectionFormat 为 false 时，传入的 Line 必须是 [ Section ] 的格式才会被处理，如果传入的格式不对，会返回NULL 
		//参数 bIsSectionFormat 为 true  时，传入的 Line 必须不包括 [ ] ，然后会被当成一个 Section 来处理。如果包括了 [ ]，会返回 NULL 
		CItemLine * add_line(const string & Line, bool bIsSectionFormat = true ); 
 
		//根据索引名删除 section，如果索引名找不到，返回false，否则，一定会删除成功，返回true 
		bool del_section(const string & ItemName); 
		//根据索引号删除 section，如果索引号范围有错误，返回false，否则，一定会删除成功，返回true 
		bool del_section(int index); 
 
		void clearAllSection(void); 
		// 获取某个Section下的所有的item。传入Section名字，找不到item，返回NULL 
		CItemLine * GetItemPtr(const string & SectionName); 
		// 获取某个Section下的所有的item。传入Section的索引编号（0开始计算），找不到item，返回NULL 
		CItemLine * GetItemPtr(int index); 
		//获取某一行的内容，包括一些其他的文字,获取成功，返回true，失败返回false 
		bool getSectionName( int index, string & GOT_SectionName ); 
		//获取所有行的行数 
		int getSectionCount(void); 
 
	protected: 
		SECTION_LIST sections_;	 
	}; 
}; 
