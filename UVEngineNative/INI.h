#pragma once 
#include "SECTION.h"
#include <vector>
#include <string>
using namespace std; 
using namespace DCG__INI__FILE; 
#define MAXFILENAME  4096 
#define DEFAULT_LINE 50 //假设一个ini文件不超过50行（超过也可以运行，没有问题） 
 
typedef std::vector<long> LineEnd; // 换行符的位置 
 
class CINI :	public CINI_BASE 
{ 
public: 
	CINI();
	CINI( const char * FileName ); 
	~CINI(void); 
protected: 
	string _fileName; 
	string _iniContent; //INI文件内容 
	long GetFileLength( const char * FileName ); 
	// 把文件读入内存 
	int LoadFileIntoMen(const char * FileName); 
	LineEnd lineEnd;  //文章的换行符 
	// 给文章内容构建换行符, lineBreak 是换行符 
	int RebuildLineBreak(char *  strContent, const char * lineBreak, size_t filesize ); 
	// 读取某一行，lineNumber是行号,读取成功，返回那一行的起始字符的地址 
	const char * ReadLine( size_t lineNumber, char * buf );	 
 
	vector<std::string> headLine; //在一个ini文件头部，可能有一些文字不在任何section里面，这些文件统一放在headLine里面 
 
 
	CSECTION section; 
	//写操作的终极函数，最后一个参数是 string 类型的value。 
	void WriteValueCommon( const string & Section, const string & Item, const string & value ); 
	//读操作的终极函数，最后一个参数是 string & 类型的value，这个值是读返回值。bool 类型的函数返回标志表示读到还是没有读到。 
	bool ReadValueCommon( const string & Section, const string & Item, string & value ); 
	// 把ini信息写入文件 
	void writeList(void); 
 
	//是否修改过的标志 
	//当有写入操作，有删除操作时此值被赋值成 true 
	//每次保存，此值都被赋为 false 
	//保存文件时，如果此值为 false 就不保存 
	bool bModified_; 
 
public: 
	int    ReadInt( const string & Section, const string & Item, const int DefaultValue ); 
	long   ReadLong( const string & Section, const string & Item, const long & DefaultValue ); 
	double ReadFloat( const string & Section, const string & Item, const double & DefaultValue ); 
	string  ReadString( const string & Section, const string & Item, const string & DefaultValue ); 
	bool   ReadBool( const string & Section, const string & Item, const bool DefaultValue ); 
 
	void WriteInt( const string & Section, const string & Item, const int i ); 
	void WriteLong( const string & Section, const string & Item, const long & i ); 
	void WriteFloat( const string & Section, const string & Item, const double & i ); 
	void WriteString( const string & Section, const string & Item, const string & i ); 
	void WriteBool( const string & Section, const string & Item, const bool i ); 
 
	void WriteFileNow(); 
 
	// 获取一共有多少个 section 
	int GetSectionCount(void); 
	// 获取第index个section的名字。index从0开始计算。如果获取成功，返回true，同时 section_name 变量赋值为取出的名字 
	bool GetSectionName(int index, string & section_name); 
	// 获取第 index  个 section 里包含有多少item,取不到返回-1 
	int GetItemCount(int index_of_Section); 
	// 获取 section_name 里包含有多少item,取不到返回-1 
	int GetItemCount( const string & section_name ); 
 
	// 从第 index_of_Section 个 section 里众多的item里取出第 index_of_Item 个 Item 的名字 
	bool GetItemName(int index_of_Section, int index_of_Item,  string & item_value ); 
	// 获取 section_name 里第 index_of_Item 个 Item 的名字 
	bool GetItemName( const string & section_name, int index_of_Item,  string & item_value ); 
	// 获取 item 的值 
	bool GetItemValue(int index_of_Section, int index_of_Item, string & value); 
	bool GetItemValue(const string & section_name, int index_of_Item, string & value); 
	bool GetItemValue(int index_of_Section, const string & item_name, string & value); 
	bool GetItemValue(const string & section_name, const string & item_name, string & value); 
	// 删除第 index 个section 
	void DelSection(int index); 
	// 删除名为 section_name 的section 
	void DelSection( const string & section_name ); 
	// 删除全部section 
	void clearSection(void); 
	// 删除item 
	void DelItem(int index_of_section, int index_of_item); 
	void DelItem( const string & section_name, int index_of_item); 
	void DelItem(int index_of_section, const string & item_name ); 
	void DelItem( const string & section_name, const string & item_name ); 
 
	// 删除某个section里所有的item 
	//当参数 bIncludeOtherLine 为true时，此Section下所有的行（包括item的和其他文字的）都被删除。 
	//当参数 bIncludeOtherLine 为 false 时，只删除 item 的。其他文字保留 
	void clearItem(int index_of_section, bool bIncludeOtherLine = false ); 
 
	// 删除某个section里所有的item 
	//当参数 bIncludeOtherLine 为true时，此Section下所有的行（包括item的和其他文字的）都被删除。 
	//当参数 bIncludeOtherLine 为 false 时，只删除 item 的。其他文字保留 
	void clearItem( const string & section_name, bool bIncludeOtherLine = false ); 
}; 
