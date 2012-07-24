
#ifndef KEYWORD_DICT_H
#define KEYWORD_DICT_H

#include <stdint.h>

class KeywordNode;
class KeywordDict
{
public :
	KeywordDict();
	~KeywordDict();

public :

	bool init();
	void final();
	
	// 增加关键字
	int32_t add( char * words, int32_t len = 0 );
	
	// 检查字符串中是否有关键字
	bool check( char * words, int32_t len );

	// 过滤消息(默认每个关键字用*代替, 直接修改源字符串)
	//		words		- 需要过滤的消息
	//		len			- 过滤消息的长度
	int32_t filter( char * words, int32_t len );
	
	// 过滤消息
	//		src			- 需要过滤的字符串
	// 		srclen		- 需要过滤的字符串长度
	//		dst			- 过滤后的字符串
	//		dstlen		- 存放过滤后字符串的空间长度
	// 		replace		- 关键字用replace来替换
	//		replacelen	- 替换字符串的长度
	int32_t filter( char * src, int32_t srclen, char * dst, int32_t dstlen, char * replace, int32_t replacelen );

private :
	
	KeywordNode * find( KeywordNode * node, char c );
	int32_t matching( char * words, int32_t index, int32_t len, int32_t & count );
	void insert( KeywordNode * node, char * words, int32_t len, int32_t index );

private :

	KeywordNode * m_RootNode;
}; 


#endif
