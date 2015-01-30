#include "pch.h"
#include "ItemLine.h"    
   
using namespace DCG__INI__FILE;   
   
CItemLine::CItemLine(void)   
{   
    ITEM_COUNT = 0;   
    this->line_of_item.clear();   
}   
   
CItemLine::~CItemLine(void)   
{   
}   
   
// 根据索引返回第几个Item的 item 名字(不返回 value )。index 从0开始计数，找不到返回变量 NOT_FOUND_ITEM    
bool DCG__INI__FILE::CItemLine::getValue(int index, string & RetValue)   
{   
    if( index < 0 || index >= ITEM_COUNT )   
    {   
        return false;   
    }   
    int i = 0;   
    LineInSection::iterator it;   
    for( it = line_of_item.begin(); it != line_of_item.end(); it++ )   
    {   
        if( (*it).isItem )   
        {   
            if( i == index )   
            {   
                RetValue = (*it).value;   
                return true;   
            }   
            ++i;   
        }   
    }   
    return false;   
}   
   
bool DCG__INI__FILE::CItemLine::getValue(const string & ItemName, string & RetValue)   
{   
    string str;        
    str = ItemName;   
    trim( str );   
    LineInSection::iterator it;   
    for( it = line_of_item.begin(); it != line_of_item.end(); it++ )   
    {   
        if( (*it).isItem )   
        {   
            if( (*it).trimedItem == str )   
            {   
                RetValue = (*it).value;   
                return true;   
            }//找到了对应的内容    
        }   
    }   
    return false;   
}   
   
void DCG__INI__FILE::CItemLine::add_line(const string &  Line)   
{   
    bool b;   
    b = this->isContainEqu( Line );   
    LINE_IN_ITEM_RANGE line_range;   
   
    if( !b ) //不是Item;    
    {   
        line_range.isItem = false;   
        line_range.value = Line;   
        line_of_item.push_back( line_range );   
        return;   
    }   
    else   
    {   
        string item;   
        string value;   
        if( ! this->get_item_and_value( Line, item, value ) )   
        {   
            return;   
        }   
        //先寻找此 Item 是否已经存在，如果已经存在，只替换其值    
        string str;        
        str = item;   
        trim( str );   
        LineInSection::iterator it;   
        for( it = line_of_item.begin(); it != line_of_item.end(); it++ )   
        {   
            if( (*it).isItem )   
            {   
                if( (*it).trimedItem == str )   
                {   
                    (*it).value = value;   
                    return;   
                }//找到了对应的内容    
            }   
        }   
   
        //下面是找不到的情况： 添加新的Item    
        LINE_IN_ITEM_RANGE line_range;   
        line_range.isItem = true;   
        line_range.item = item;   
        line_range.value = value;   
        trim( item );   
        line_range.trimedItem = item;      
        line_of_item.push_back( line_range );   
        ++ ITEM_COUNT;   
    }   
}   
   
void DCG__INI__FILE::CItemLine::add_line(const string & ItemName, const string & value )   
{   
        //先寻找此 Item 是否已经存在，如果已经存在，只替换其值    
        string str;        
        str = ItemName;   
        trim( str );           
        LineInSection::iterator it;   
        for( it = line_of_item.begin(); it != line_of_item.end(); it++ )   
        {   
            if( (*it).isItem )   
            {   
                if( (*it).trimedItem == str )   
                {   
                    (*it).value = value;   
                    return;   
                }//找到了对应的内容    
            }   
        }   
   
        //下面是找不到的情况： 添加新的Item    
        LINE_IN_ITEM_RANGE line_range;   
        line_range.isItem = true;   
        line_range.item = ItemName;   
        line_range.value = value;   
        line_range.trimedItem = str;       
        line_of_item.push_back( line_range );   
        ++ ITEM_COUNT;   
}   
   
bool DCG__INI__FILE::CItemLine::get_item_and_value(const string & Line , string & item , string & value)   
{   
    item = "";   
    value = "";   
       
    basic_string <CHAR>::size_type i;   
    i = Line.find( "=" );   
    if( i == -1 )   
    {   
        return false;   
    }   
    item.append( Line, 0, i );   
    value.append( Line, i + 1, Line.length() - i - 1 );   
    return true;   
}   
   
bool DCG__INI__FILE::CItemLine::del_Item(const string & ItemName)   
{   
    size_t l = ItemName.length();   
    if( l == 0 )   
    {   
        return false;   
    }   
    string item = ItemName;   
    trim( item );   
   
    LineInSection::iterator it;   
    for( it == line_of_item.begin(); it != line_of_item.end(); it++ )   
    {   
        if( (*it).isItem )   
        {   
            if(  (*it).trimedItem == item )   
            {   
                line_of_item.erase( it );   
                --ITEM_COUNT;   
                return true;   
            }   
        }   
    }   
    return false;   
}   
   
bool DCG__INI__FILE::CItemLine::del_Item(int index)   
{   
    if( index < 0 || index >= ITEM_COUNT )   
    {   
        return NULL;   
    }   
    int i = 0;   
    LineInSection::iterator it;   
    for( it == line_of_item.begin(); it != line_of_item.end(); it++ )   
    {   
        if( (*it).isItem )   
        {   
            if( i == index )   
            {   
                line_of_item.erase( it );   
                --ITEM_COUNT;   
                return true;   
            }   
            ++i;   
        }   
    }   
    return false;   
}   
   
int DCG__INI__FILE::CItemLine::getItemCount(void)   
{   
    return ITEM_COUNT;   
}   
   
   
   
// 清空所有的 item ，当参数 bIncludeOtherLine 为true时，此Section下所有的行（包括item的和其他文字的）都被删除。    
//当参数 bIncludeOtherLine 为 false 时，只删除 item 的。其他文字保留    
void DCG__INI__FILE::CItemLine::clearAllItem(bool bIncludeOtherLine)   
{   
    ITEM_COUNT = 0;   
    if( bIncludeOtherLine )   
    {              
        line_of_item.clear();   
    }   
    else   
    {   
        LineInSection::iterator it, it_back;   
        for( it = line_of_item.begin(); it != line_of_item.end(); it++ )   
        {   
            if( (*it).isItem )   
            {   
                it_back = it;   
                --it_back;   
                line_of_item.erase( it );   
                it = it_back;   
            }   
        }   
    }   
}   
   
//获取某一行的内容，包括一些其他的文字    
string DCG__INI__FILE::CItemLine::getLine( int index )   
{   
    if( index < 0 )   
    {   
        return "";   
    }   
    int i = 0;   
    LineInSection::iterator it;   
    for( it = line_of_item.begin(); it != line_of_item.end(); it++ )   
    {   
        if( i == index )   
        {   
            if( (*it).isItem )   
            {   
                string strTemp;   
                strTemp = (*it).item + "=" + (*it).value;   
                return strTemp;   
            }   
            else   
            {   
                return (*it).value;   
            }   
        }   
        ++i;   
    }   
    return NULL;   
}   
//获取所有行的行数    
int DCG__INI__FILE::CItemLine::getAllLineCount()   
{   
    return (int)(line_of_item.size() );   
}   
   
   
bool DCG__INI__FILE::CItemLine::getItemName( int index, string & item )   
{   
    if( index < 0 || index >= ITEM_COUNT )   
    {   
        return false;   
    }   
    int i = 0;   
    LineInSection::iterator it;   
    for( it = line_of_item.begin(); it != line_of_item.end(); it++ )   
    {   
        if( (*it).isItem )   
        {   
            if( i == index )   
            {   
                item = (*it).trimedItem;   
                return true;   
            }   
            ++i;   
        }   
    }   
    return false;   
}   
   
bool DCG__INI__FILE::CItemLine::getItemName( const string & ItemName, string & item )   
{   
    string str;        
    str = ItemName;   
    trim( str );   
    LineInSection::iterator it;   
    for( it = line_of_item.begin(); it != line_of_item.end(); it++ )   
    {   
        if( (*it).isItem )   
        {   
            if( (*it).trimedItem == str )   
            {   
                item = (*it).trimedItem;   
                return true;   
            }//找到了对应的内容    
        }   
    }   
    return false;   
}  