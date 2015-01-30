#include "pch.h"
#include "SECTION.h"     
   
using namespace DCG__INI__FILE;   
   
   
CSECTION::CSECTION(void)   
{   
    sections_.clear();     
}   
   
CSECTION::~CSECTION(void)   
{   
}   
   
bool DCG__INI__FILE::CSECTION::get_SectionName(const  string & Line, string & section )   
{   
    section = "";   
    basic_string <CHAR>::size_type i1, i2;   
    i1 = Line.find( "[" );   
    if( i1 == -1 )   
    {   
        return false;   
    }   
    i2 = Line.find( "]" );   
    if( i2 == -1 )   
    {   
        return false;   
    }   
    if( i1 >= i2 )   
    {   
        return false;   
    }   
    section.append( Line, i1 + 1, i2 - i1 - 1 );   
    return true;   
}   
   
//增加一行，这一行的内容由 Line 传入    
//参数 bIsSectionFormat 为 false 时，传入的 Line 必须是 [ Section ] 的格式才会被处理，如果传入的格式不对，会返回NULL    
//参数 bIsSectionFormat 为 true  时，传入的 Line 必须不包括 [ ] ，然后会被当成一个 Section 来处理。如果包括了 [ ]，会返回 NULL    
CItemLine * DCG__INI__FILE::CSECTION::add_line(const string & Line, bool bIsSectionFormat)   
{   
    if( ! bIsSectionFormat )   
    {   
        if( this->isContainBracket( Line ) )   
        {   
            return NULL;   
        }   
    }   
    else   
    {   
        if( ! this->isSection( Line.c_str() ) )   
        {   
            return NULL;   
        }   
    }   
    string section;   
    if( bIsSectionFormat )   
    {   
        if( ! this->get_SectionName( Line, section ) )   
        {   
            return NULL;   
        }   
    }   
    else   
    {   
        section = Line;   
    }   
   
       
    ONE_SECTION a_section;   
    a_section.section = section;   
    trim( section );   
    a_section.trimedSection = section;   
    sections_.push_back( a_section );   
    SECTION_LIST::const_reference cr = sections_.back();   
	SECTION_LIST::reference f = const_cast<SECTION_LIST::reference> (cr);   
    return &(f.items);   
}   
   
//根据索引名删除 section，如果索引名找不到，返回false，否则，一定会删除成功，返回true    
bool DCG__INI__FILE::CSECTION::del_section(const string & ItemName)   
{   
    string str = ItemName;   
    trim( str );   
    SECTION_LIST::iterator it;   
    for( it = sections_.begin(); it != sections_.end(); it++ )   
    {   
        if( (*it).trimedSection == str )   
        {   
            sections_.erase( it );   
            return true;   
        }   
    }   
    return false;   
}   
   
   
//根据索引号删除 section，如果索引号范围有错误，返回false，否则，一定会删除成功，返回true    
bool DCG__INI__FILE::CSECTION::del_section(int index)   
{   
    if( index < 0 || index >= (int)(sections_.size()) )   
    {   
        return false;   
    }   
    int i = 0;   
    SECTION_LIST::iterator it;   
    for( it = sections_.begin(); it != sections_.end(); it++, i++ )   
    {   
        if( i == index )               
        {   
            sections_.erase( it );   
            return true;   
        }              
    }   
    return false;   
}   
   
   
int DCG__INI__FILE::CSECTION::getSectionCount(void)   
{   
    return (int)(sections_.size());   
}   
   
void DCG__INI__FILE::CSECTION::clearAllSection(void)   
{   
    sections_.clear();   
}   
   
// 获取某个Section下的所有的item。传入Section名字，找不到item，返回NULL    
CItemLine * DCG__INI__FILE::CSECTION::GetItemPtr(const string & SectionName)   
{   
    string str = SectionName;   
    trim( str );   
    SECTION_LIST::iterator it;   
    for( it = sections_.begin(); it != sections_.end(); it++ )   
    {   
        if(  (*it).trimedSection == str  )   
        {   
            return &((*it).items);   
        }   
    }   
    return NULL;   
}   
   
// 获取某个Section下的所有的item。传入Section的索引编号（0开始计算），找不到item，返回NULL    
CItemLine * DCG__INI__FILE::CSECTION::GetItemPtr(int index)   
{   
    if( index < 0 || index >= (int)(sections_.size()) )   
    {   
        return NULL;   
    }   
    int i = 0;   
    SECTION_LIST::iterator it;   
    for( it = sections_.begin(); it != sections_.end(); it++, i++ )   
    {   
        if(  i == index  )   
        {   
            return &((*it).items);   
        }   
    }   
    return NULL;   
}   
   
//获取某一行的内容，包括一些其他的文字,获取成功，返回true，失败返回false    
bool DCG__INI__FILE::CSECTION::getSectionName( int index, string & GOT_SectionName )   
{   
    GOT_SectionName = "";   
    if( index < 0 || index >= (int)sections_.size() )   
    {   
        return false;   
    }   
    int i = 0;   
    SECTION_LIST::iterator it;   
    for( it = sections_.begin(); it != sections_.end(); it ++,  i++ )   
    {   
        if( i == index )   
        {   
            GOT_SectionName = (*it).section;   
            return true;   
        }   
    }   
    return false;   
}