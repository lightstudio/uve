#include "pch.h"
#include "INI.h"  
#include <sys/stat.h>
   
CINI::CINI()
{

}

CINI::CINI( const char * FileName )   
{   
    if( FileName == NULL )   
    {   
        throw( "config file can't be found." );   
    }   
   
    if( strlen( FileName ) > MAXFILENAME )   
    {   
        throw( "config file name is too long." );   
    }   
   
    lineEnd.resize( DEFAULT_LINE );   
    //headLine.reserve( DEFAULT_LINE );    
    _fileName = FileName;   
    LoadFileIntoMen( FileName );   
    bModified_ = false;   
}   
   
CINI::~CINI(void)   
{   
    writeList();   
}   
   
long CINI::GetFileLength( const char * FileName )   
{   
    FILE * f;
	fopen_s( &f,FileName, "r" );

    if( f == NULL )   
    { //读取文件失败    
        return -1;   
    }   
   
    long fl = -1; // Length of file    
    struct stat  buf;   
    int fileno;   
    fileno = _fileno( f );   
    if ( fstat( fileno, & buf ) != 0 )   
    {   
        fclose( f );   
        return -1;   
    }   
    fclose( f );   
    fl = (long)buf.st_size;   
   
    return fl;   
}   
   
   
// 把文件读入内存    
int CINI::LoadFileIntoMen(const char * FileName)   
{   
    long FileLength = GetFileLength( FileName );   
    if( FileLength <= 0 )   
    {   
        return 0; //如果文件长度等于0，或文件根本不存在，直接返回成功（文件不存在，后面会创建文件）    
    }   
   
    FILE * f;
	fopen_s( &f, FileName, "r" );   
    if( f == NULL )   
    { //读取文件失败    
        return -1;   
    }   
   
    char * _buf;   
    try   
    {   
        _buf = new char [ FileLength + 1 ];   
        if( _buf == NULL )   
        {   
            return -2; //内存不足    
        }   
    }catch(...){ return -2 ; } //内存不足    
    auto_ptr<CHAR> FileBuf( _buf );   
    _buf [ FileLength  ] = '\0';   
    size_t s_read = fread( _buf, sizeof(char), FileLength , f );   
    _buf[ s_read ] = '\0';   
   
    fclose ( f );   
    //重建文章的换行符    
    RebuildLineBreak( _buf, "\n", s_read );   
   
   
    //把文章读入内存 section     
    size_t i;   
    bool bHeadLine = true;  //一开始会去识别 headLine    
    string strTemp, strLine;   
    CItemLine * item = NULL;   
    for( i = 0; i < lineEnd.size(); i++ )   
    {   
        const char * line = ReadLine( i, _buf );   
        if( line == NULL )   
        {   
            continue;   
        }   
        strLine = line;   
        if( bHeadLine )   
        {   
            if( this->isSection( line ) )   
            {   
                item = this->section.add_line( strLine );   
                if( item == NULL ) //加入一行成功，说明这一行可能是普通的 head line    
                {   
                    this->headLine.push_back( strLine );   
                    continue;   
                }   
                else   
                { //加入 section 成功， 正式进入section 范围    
                    bHeadLine = false;   
                }   
            }   
            else   
            {   
                this->headLine.push_back( strLine );   
                continue;   
            }   
        }   
        else   
        {              
            if( this->isSection( line ) )   
            {   
                CItemLine * item2 = NULL;   
                item2 = this->section.add_line( line );   
                if( item2 == NULL )   
                {   
                    if( item == NULL )   
                    {   
                        continue;   
                    }   
                    item->add_line( strLine );   
                }   
                else   
                {   
                    item = item2;   
                }   
            }   
            else   
            {   
                item->add_line( strLine );   
            }   
        }   
    } //读入内存完毕    
    return 0;   
}   
   
// 给文章内容构建换行符, lineBreak 是换行符    
int CINI::RebuildLineBreak(char * strContent, const char * lineBreak, size_t filesize )   
{   
    char * p = NULL;   
    lineEnd.clear();   
    size_t lenOfLineBreak = strlen( lineBreak );   
   
    lineEnd.push_back( 0 );   
   
    p = strContent;   
    while( p != NULL )   
    {   
        p = strstr( p, lineBreak );   
        if( p == NULL )   
        {   
            break;   
        }   
        size_t i;   
        for( i = 0; i < lenOfLineBreak; i ++ )   
        {   
            *p = '\0';   
            ++p;   
        }          
        size_t  offset = (size_t)( p - strContent );   
        if( offset >= filesize )   
        {   
            break;   
        }   
        lineEnd.push_back( (long) offset );        
    }   
    return 0;   
}   
   
// 读取某一行，lineNumber是行号,读取成功，返回那一行的起始字符的地址    
const char * CINI::ReadLine( size_t lineNumber, char * buf )   
{   
    if( lineNumber > lineEnd.size()  || lineNumber < 0 )   
    {   
        return NULL;   
    }   
   
    if( lineNumber == 0 )   
    {   
        return buf;   
    }   
   
    size_t LineStartPos = (size_t)( lineEnd[ lineNumber ] );   
    char *p = buf + LineStartPos;   
    return p;   
}   
   
//写操作的终极函数，最后一个参数是 string 类型的value。    
void CINI::WriteValueCommon( const string & Section, const string & Item, const string & value )   
{   
    CItemLine * item = NULL;   
    item = section.GetItemPtr( Section );   
    if( item == NULL )   
    { //没有这个section         
        item = section.add_line( Section, false );   
    }   
    item->add_line( Item, value );    
    bModified_ = true;   
}   
//读操作的终极函数，最后一个参数是 string & 类型的value，这个值是读返回值。bool 类型的函数返回标志表示读到还是没有读到。    
bool CINI::ReadValueCommon( const string & Section, const string & Item, string & value )   
{   
    CItemLine * item = section.GetItemPtr( Section );   
    if( item == NULL )   
    {   
        return false;   
    }   
   
    if( ! item->getValue( Item, value ) )   
    {   
        return false;   
    }   
    trim( value );   
    return true;   
}   
   
int CINI::ReadInt( const string & Section, const string & Item, const int DefaultValue )   
{   
    int i;   
    string str;   
    if( ReadValueCommon( Section, Item, str ) )   
    {   
        trim( str );   
        if( isDigital( str ) )   
        {   
            i = atoi( str.c_str() );   
        }   
        else   
        {   
            i = DefaultValue;   
        }   
    }   
    else   
    {   
        i = DefaultValue;   
    }   
    return i;   
}   
   
long CINI::ReadLong( const string & Section, const string & Item, const long & DefaultValue )   
{   
    long i;   
    string str;   
    if( ReadValueCommon( Section, Item, str ) )   
    {   
        trim( str );   
        if( isDigital( str ) )   
        {   
            i = atol( str.c_str() );   
        }   
        else   
        {   
            i = DefaultValue;   
        }   
    }   
    else   
    {   
        i = DefaultValue;   
    }   
    return i;   
}   
   
double CINI::ReadFloat( const string & Section, const string & Item, const double & DefaultValue )   
{   
    double i;   
    string str;   
    if( ReadValueCommon( Section, Item, str ) )   
    {   
        trim( str );   
        if( isDigital( str ) )   
        {   
            i = atof( str.c_str() );   
        }   
        else   
        {   
            i = DefaultValue;   
        }   
    }   
    else   
    {   
        i = DefaultValue;   
    }   
    return i;   
}   
   
string  CINI::ReadString( const string & Section, const string & Item, const string & DefaultValue )   
{   
    string i;   
    string str;   
    if( ReadValueCommon( Section, Item, str ) )   
    {   
        trim( str );   
        if( (int)str.length() == 0 )   
        {   
            i = DefaultValue;   
        }   
        else   
        {   
            i = str;   
        }   
    }   
    else   
    {   
        i = DefaultValue;   
    }   
    return i;   
}   
   
   
bool CINI::ReadBool( const string & Section, const string & Item, const bool DefaultValue )   
{   
    bool i;   
    string str;   
    if( ReadValueCommon( Section, Item, str ) )   
    {   
        trim( str );   
        int boolValue;   
        boolValue = isBool( str );   
        if( boolValue == 0 )   
        {   
            i = true;   
        }   
        else if ( boolValue == 1 )   
        {   
            i = false;   
        }   
        else   
        {   
            i = DefaultValue;   
        }   
    }   
    else   
    {   
        i = DefaultValue;   
    }   
    return i;   
}   
   
   
void CINI::WriteInt( const string & Section, const string & Item, const int i )   
{   
    string strTemp;   
    char cTemp[ 256 ];   
    cTemp[255] = '\0';   
    sprintf_s( cTemp, "%d", i );   
    strTemp = cTemp;   
    WriteValueCommon( Section, Item, strTemp );   
    return;   
}   
   
void CINI::WriteLong( const string & Section, const string & Item, const long & i )   
{   
    string strTemp;   
    char cTemp[ 256 ];   
    cTemp[255] = '\0';   
    sprintf_s( cTemp, "%ld", i );   
    strTemp = cTemp;   
    WriteValueCommon( Section, Item, strTemp );   
    return;   
}   
   
void CINI::WriteFloat( const string & Section, const string & Item, const double & i )   
{   
    string strTemp;   
    char cTemp[ 256 ];   
    cTemp[255] = '\0';   
    sprintf_s( cTemp, "%lf", i );   
    strTemp = cTemp;   
    WriteValueCommon( Section, Item, strTemp );   
    return;   
}   
   
   
void CINI::WriteString( const string & Section, const string & Item, const string & i )   
{   
    WriteValueCommon( Section, Item, i );   
    return;   
}   
   
   
void CINI::WriteBool( const string & Section, const string & Item, const bool i )   
{   
    string strTemp;   
    if( i )   
    {   
        strTemp = "True";   
    }   
    else   
    {   
        strTemp = "False";   
    }   
    WriteValueCommon( Section, Item, strTemp );   
    return;   
}   
// 把ini信息写入文件    
void CINI::writeList(void)   
{   
    if( ! bModified_ )   
    { //ini文件没有任何写入操作，不写入文件    
        return;   
    }   
    FILE *p;   
    fopen_s( &p, this->_fileName.c_str(), "w" );   
    if( !p )   
    {   
        p = NULL;   
        throw( "couldn't write file" );   
    }   
    int i;   
    string sTemp;   
   
    int lenOfheadLine = (int)headLine.size();   
    int lenOfSection  = section.getSectionCount();   
   
    //先写 headline    
    if( lenOfSection == 0 )   
    {   
        for( i = 0; i < lenOfheadLine; i ++ )   
        {   
            sTemp = headLine[i];   
            if( i == (lenOfheadLine - 1) )   
            {                  
                fprintf( p, "%s", sTemp.c_str() );   
            }   
            else   
            {   
                fprintf( p, "%s\n", sTemp.c_str() );   
            }   
        }   
    }   
    else   
    {   
        for( i = 0; i < lenOfheadLine; i ++ )   
        {   
            sTemp = headLine[i];   
            fprintf( p, "%s\n", sTemp.c_str() );   
        }   
    }   
   
   
    //再写 section    
    for( i = 0; i < lenOfSection; i++ )   
    {   
        if( ! section.getSectionName( i, sTemp ) )   
        {   
            continue;   
        }   
        CItemLine * item = section.GetItemPtr( i );   
        if( item == NULL )   
        {   
            continue;   
        }   
        if( i == (lenOfSection-1)  &&  item == NULL )   
        {   
            fprintf( p, "[%s]", sTemp.c_str() );   
        }   
        else   
        {   
            fprintf( p, "[%s]\n", sTemp.c_str() );   
            int j;   
            for( j = 0; j < item->getAllLineCount(); j ++ )   
            {   
                sTemp = item->getLine( j );   
                if( i == (lenOfSection-1)  &&  j == (item->getAllLineCount() - 1) )   
                {   
                    fprintf( p, "%s", sTemp.c_str() );   
                }   
                else   
                {   
                    fprintf( p, "%s\n", sTemp.c_str() );   
                }   
            }   
        }   
    }   
    fclose( p );   
    bModified_ = false;   
}   
   
void CINI::WriteFileNow()   
{   
    writeList();   
}   
   
// 获取一共有多少个 section    
int CINI::GetSectionCount(void)   
{   
    return this->section.getSectionCount();     
}   
   
// 获取第index个section的名字。index从0开始计算。如果获取成功，返回true，同时 section_name 变量赋值为取出的名字    
bool CINI::GetSectionName(int index, string & section_name)   
{   
    return this->section.getSectionName( index, section_name );   
}   
   
// 获取第 index  个 section 里包含有多少item,取不到返回-1    
int CINI::GetItemCount(int index_of_Section)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_Section );   
    if( item == NULL )   
    {   
        return -1;   
    }   
    return item->getItemCount();    
}   
   
// 获取 section_name 里包含有多少item,取不到返回-1    
int CINI::GetItemCount( const string & section_name )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return -1;   
    }   
    return item->getItemCount();   
}   
   
// 从第 index_of_Section 个 section 里众多的item里取出第 index_of_Item 个 Item 的名字    
bool CINI::GetItemName(int index_of_Section, int index_of_Item,  string & item_value )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_Section );   
    if( item == NULL )   
    {   
        return false;   
    }   
    return item->getItemName( index_of_Item, item_value );      
}   
// 获取 section_name 里第 index_of_Item 个 Item 的名字    
bool CINI::GetItemName( const string & section_name, int index_of_Item,  string & item_value )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return false;   
    }   
    return item->getItemName( index_of_Item, item_value );      
}   
   
// 获取 item 的值    
bool CINI::GetItemValue(int index_of_Section, int index_of_Item, string & value)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_Section );   
    if( item == NULL )   
    {   
        return false;   
    }   
    return item->getValue( index_of_Item, value );   
}   
   
bool CINI::GetItemValue(const string & section_name, int index_of_Item, string & value)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return false;   
    }   
    return item->getValue( index_of_Item, value );   
}   
bool CINI::GetItemValue(int index_of_Section, const string & item_name, string & value)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_Section );   
    if( item == NULL )   
    {   
        return false;   
    }   
    return item->getValue( item_name, value );   
}   
bool CINI::GetItemValue(const string & section_name, const string & item_name, string & value)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return false;   
    }   
    return item->getValue( item_name, value );   
}   
// 删除第 index 个section    
void CINI::DelSection(int index)   
{   
    if( this->section.del_section( index ) )   
    {   
        bModified_ = true;   
    }   
}   
// 删除名为 section_name 的section    
void CINI::DelSection( const string & section_name )   
{   
    if( this->section.del_section( section_name ) )   
    {   
        bModified_ = true;   
    }   
}   
   
// 删除全部section    
void CINI::clearSection(void)   
{   
    if( this->section.getSectionCount() == 0 )   
    {   
        return ;   
    }   
    this->section.clearAllSection();   
    bModified_ = true;   
}   
   
// 删除item    
void CINI::DelItem(int index_of_section, int index_of_item)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_section );   
    if( item == NULL )   
    {   
        return;   
    }   
    if( item->del_Item( index_of_item ) )   
        bModified_ = true;   
}   
   
void CINI::DelItem( const string & section_name, int index_of_item)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return;   
    }   
    if( item->del_Item( index_of_item ) )   
        bModified_ = true;   
}   
void CINI::DelItem(int index_of_section, const string & item_name )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_section );   
    if( item == NULL )   
    {   
        return;   
    }   
    if( item->del_Item( item_name ) )   
        bModified_ = true;   
}   
void CINI::DelItem( const string & section_name, const string & item_name )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return;   
    }   
    if( item->del_Item( item_name ) )   
        bModified_ = true;   
}   
   
// 删除某个section里所有的item    
void CINI::clearItem(int index_of_section, bool bIncludeOtherLine )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_section );   
    if( item == NULL )   
    {   
        return;   
    }   
    item->clearAllItem( bIncludeOtherLine );   
    bModified_ = true;   
}   
   
// 删除某个section里所有的item    
//当参数 bIncludeOtherLine 为true时，此Section下所有的行（包括item的和其他文字的）都被删除。    
//当参数 bIncludeOtherLine 为 false 时，只删除 item 的。其他文字保留    
void CINI::clearItem( const string & section_name, bool bIncludeOtherLine )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return;   
    }   
    item->clearAllItem( bIncludeOtherLine );   
    bModified_ = true;   
}  