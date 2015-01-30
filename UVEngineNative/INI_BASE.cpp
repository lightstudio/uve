#include "pch.h"
#include "INI_BASE.h"    
   
using namespace DCG__INI__FILE;   
   
   
CINI_BASE::CINI_BASE(void)   
{   
}   
   
CINI_BASE::~CINI_BASE(void)   
{   
}   
   
void CINI_BASE::trim(string & str)   
{   
    if( str.empty() ) return;   
    int i;   
    for( i = 0; i < (int)str.length(); i++ )   
    {   
        if( str[i] == ' ' || str[i] == '    ' )   
            continue;   
        else   
        {   
            break;   
        }   
    }   
   
    if( i > ((int)str.length()) - 1 )  //当整个字符串都是空格时，就会有这个情况    
    {   
        str = "";   
        return;   
    }   
   
   
    int j;   
    for( j = ((int)str.length()) - 1; j >= 0; j-- )   
    {   
        if( str[j] == ' ' ||  str[j] == '   ' )   
            continue;   
        else   
        {   
            break;   
        }   
    }   
   
    if( i > j )   
    {      
        str = "";   
        return;   
    }   
   
    if( i == 0  &&  j == ((int)str.length()) - 1 )  //这种情况是字符串两边都没有空格    
    {   
        return;   
    }   
   
    str = str.substr( i, j - i + 1 );   
    return;   
}   
   
// 判断某行是否是section格式，即 [ ... ] 格式    
bool CINI_BASE::isSection(const char * Line)   
{   
        size_t i;   
        size_t length = strlen( Line );   
        for( i = 0; i < length; i++ )   
        {   
            if( Line[i] == '[' )   
            {   
                break;   
            }   
            else if( Line[i] == ' ' || Line[i] == ' ' )   
            {   
                continue;   
            }   
            else   
            {   
                return false;   
            }   
        }   
        if( i>= length )   
        {   
            return false;   
        }   
   
        ++i;   
   
        for( ; i < length; i++ )   
        {   
            if( Line[i] == ']' )   
                break;   
        }   
        if( i >= length )   
        {   
            return false;   
        }   
   
        ++i;   
        for( ; i < length; i++ )   
        {   
            if( Line[i] == ' ' || Line[i] == '  ')   
            {   
                continue;   
            }   
            else if( Line[i] == ';' )  //分号，后面的都是注释，可以认为是正确的Section了。    
            {   
                return true;   
            }   
            else   
                return false;   
        }   
        return true;   
}   
   
// 判断某行是否是Item格式，即有等号，而且等号左边有非空字符    
bool CINI_BASE::isItem(const char * Line)   
{   
        const char * p;   
        p = strstr( Line, "=" );   
        if( p == NULL )   
        {   
            return false;   
        }   
        const char * p2;   
        for( p2 = Line; p2 < p; p2++ )   
        {   
            if( (*p2)  != ' ' &&  (*p2) != '    ' )   
            {   
                return true;   
            }   
        }   
        return false;   
}   
   
// 是否包含 等于 符号。Section 和 Item 写的时候需要判断，不能包含等于号    
bool CINI_BASE::isContainEqu(const char * Line)   
{   
        const char * p = NULL; 
        p = strstr( Line, "=" );   
        if( p == NULL )   
        {   
            return false;   
        }   
        return true;   
}   
   
// 是否包含 等于 符号。Section 和 Item 写的时候需要判断，不能包含等于号    
bool CINI_BASE::isContainEqu(const string & Line)   
{   
    //static const basic_string <CHAR>::size_type npos = -1;    
    basic_string <CHAR>::size_type i;   
    i = Line.find( "=" );   
    if( i == -1 )   
    {   
        return false;   
    }   
    return true;   
}   
   
// 是否包含方括号    
bool CINI_BASE::isContainBracket(const string & str)   
{   
        size_t i;   
        size_t len_of_str = str.length();   
        for( i = 0; i < len_of_str; i++ )   
        {   
            if( str[i] == '[' || str[i] == ']' )   
            {   
                return true;   
            }   
        }   
        return false;   
}   
   
// 判断是否是数字    
bool CINI_BASE::isDigital(const string & str)   
{   
        bool dot = false;      
        bool begin = true;   
   
        string strTemp = str;   
        trim( strTemp );   
        if( (int)strTemp.length() == 0 )   
        {   
            return false;   
        }   
        for( int i = 0; i < (int)strTemp.length(); i++ )   
        {   
            char c = strTemp[i];   
            if( c == '-' && begin )   
            {   
                begin = false;   
                continue;   
            }   
            if( c == '+' && begin )   
            {   
                begin = false;   
                continue;   
            }   
            begin = false;   
   
            if( c >= '0' && c <= '9' )   
            {   
                continue;   
            }   
            else if( c == '.' )   
            {   
                if( dot )   
                    return false;   
                else   
                    dot = true;   
            }   
            else    
                return false;   
        }   
        return true;   
}   
   
// 获取下一个空格的字符的指针    
char * CINI_BASE::getNextBlank(char * Line)   
{   
        char * p = Line;   
        size_t i = 0;   
        while ( p != '\0' )   
        {   
            if( (*p) == ' ' || (*p) == '    ' || (*p) == '　' )   
            {   
                return p;   
            }   
            if( i >= 512 )   
            {   
                break;   
            }   
            ++i;   
            ++p;   
        }   
        return NULL;   
}   
   
// 获取下一个空格的字符的指针    
size_t CINI_BASE::getNextBlank(const string & Line)   
{   
        size_t l = Line.length();   
        size_t i ;   
        for( i = 0; i < l; i ++ )   
        {   
            if( Line[i]  == ' ' ||  Line[i] == '    ' ||  Line[i] == '　' )   
            {   
                return i;   
            }              
        }   
        return NULL;   
}   
   
// 获取下一个非空字符的指针    
char * CINI_BASE::getNextNotBlank(char * Line)   
{   
        char * p = Line;   
        size_t i = 0;   
        while ( p != '\0' )   
        {   
            if( (*p) != ' ' && (*p) != '    ' && (*p) != '　' )   
            {   
                return p;   
            }   
            if( i >= 512 )   
            {   
                break;   
            }   
            ++i;   
            ++p;   
        }   
        return NULL;   
}   
   
// 获取下一个非空字符的指针    
size_t CINI_BASE::getNextNotBlank(const string & Line)   
{   
        size_t l = Line.length();   
        size_t i ;   
        for( i = 0; i < l; i ++ )   
        {   
            if( Line[i]  != ' ' &&  Line[i] != '    ' &&  Line[i] != '　' )   
            {   
                return i;   
            }              
        }   
        return NULL;   
}   
   
   
// 判断是否是bool字符串,    
// TRUE = 0    
// FALSE = 1    
// error = -1    
int CINI_BASE::isBool( const string & str )   
{   
    if( (int)str.length() != 4 && (int)str.length() != 5 )   
    {   
        return -1;   
    }   
    string sTrue = "true";   
    string sFalse = "false" ;   
    string strTemp = str;   
    trim( strTemp );   
    if( CompareStringsIgnoreCase( strTemp, sTrue ) )   
    {   
        return 0;   
    }   
    else if( CompareStringsIgnoreCase( strTemp, sFalse ) )   
    {   
        return 1;   
    }   
    return -1;   
}   
   
//不区分大小写匹配，吻合返回true    
bool CINI_BASE::CompareStringsIgnoreCase( const string & str1, const string & str2 )   
{   
    if( str1.length() != str2.length() )   
    {   
        return false;   
    }   
   
    int i;   
    for( i = 0; i < (int)str1.length(); i++ )   
    {   
        char c, d;   
        c = str1[i];   
        d = str2[i];   
        if( c == d )   
        {   
            continue;   
        }   
        if( c >= 'A' && c <= 'Z' && d >= 'a' && d <= 'z' )   
        {   
            c += 32;   
            if( c == d )   
                continue;   
        }   
        else if ( d >= 'A' && d <= 'Z' && c >= 'a' && c <= 'z' )   
        {   
            d += 32;   
            if( c == d )   
                continue;   
        }   
        return false;   
    }   
    return true;   
} 