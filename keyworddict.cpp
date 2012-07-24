
#include <map>

#include <string.h>
#include <stdlib.h>

#include "keyworddict.h"

class KeywordNode
{
public :

	KeywordNode()
	{
		m_IsEnding = false;
		m_KeywordNodes.clear();
	}

	~KeywordNode()
	{
		m_IsEnding = false;

		std::map<char, KeywordNode*>::iterator it;
		for ( it = m_KeywordNodes.begin(); it != m_KeywordNodes.end(); ++it )
		{
			KeywordNode * node = it->second;
			if ( node )
			{
				delete node;
			}
		}

		m_KeywordNodes.clear();
	}

public :

	KeywordNode * find( char c )
	{
		KeywordNode * rc = NULL;
		std::map<char, KeywordNode*>::iterator pos;

		pos =  m_KeywordNodes.find( c );
		if ( pos != m_KeywordNodes.end() )
		{
			rc = pos->second;
		}

		return rc;
	}

	void add( char c, KeywordNode * node )
	{
		m_KeywordNodes.insert( std::make_pair(c, node) );
	}

	bool isEnding() const
	{
		return m_IsEnding;
	}

	void setEnding()
	{
		m_IsEnding = true;
	}

private :

	bool							m_IsEnding;
	std::map<char, KeywordNode *>	m_KeywordNodes;
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

KeywordDict::KeywordDict()
{
	m_RootNode = NULL;
}

KeywordDict::~KeywordDict()
{
	m_RootNode = NULL;
}

bool KeywordDict::init()
{
	m_RootNode = new KeywordNode;
	if ( m_RootNode )
	{
		return true;
	}

	return false;
}

void KeywordDict::final()
{
	if ( m_RootNode )
	{
		delete m_RootNode;
	}
}

void KeywordDict::insert( KeywordNode * node, char * words, int32_t len, int32_t index )
{
	KeywordNode * n = NULL;

	n = find( node, words[index] );
	if ( n == NULL )
	{
		n = new KeywordNode;
		if ( n )
		{
			node->add( words[index], n );	
		}
	}

	++index;
	if ( index == len )
	{
		n->setEnding();
	}
	else if ( index < len )
	{
		insert( n, words, len, index );
	}

	return;
}

KeywordNode * KeywordDict::find( KeywordNode * node, char c )
{
	return node->find( c );
}

int32_t KeywordDict::matching( char * words, int32_t index, int32_t len, int32_t & count )
{
	int32_t i = index;
	KeywordNode * node = m_RootNode;
	
	while ( i < len )
	{
		node = find( node, words[i] );
		if ( node == NULL )
		{
			count = 0;
			i = index;
			break;
		}
		else 
		{
			++count;
		
			if ( node->isEnding() )
			{
				// 关键字匹配完毕, 找到了需要过滤的字串
				break;
			}
			else if ( i == len-1 )
			{
				// 关键字还在匹配中, 但待过滤的字符串已经结束了
				count = 0;
				i = index;
				break;
			}
		}
		
		++i;
	}
	
	return i;
}

int32_t KeywordDict::add( char * words, int32_t len )
{
	len = ( len == 0 ? (int32_t)::strlen(words) : len );
	if ( len <= 0 )
	{
		return -1;
	}

	insert( m_RootNode, words, len, 0 );
	return 0;
}

bool KeywordDict::check( char * words, int32_t len )
{
	int32_t i = 0;
		
	while ( i < len )
	{
		int32_t index = i;
		int32_t count = 0;

		i = matching( words, index, len, count );

		// 找到了关键字
		if ( count > 0 )
		{
			break;
		}		
		
		++i;
	}
	
	return ( i == len );		
}

int32_t KeywordDict::filter( char * words, int32_t len )
{
	int32_t i = 0;
	
	while ( i < len )
	{
		int32_t index = i;
		int32_t count = 0;

		i = matching( words, index, len, count );

		// 找到了关键字
		for ( ; count > 0; --count )
		{
			words[index+count-1] = '*';
		}		
		
		++i;
	}
	
	words[len] = 0;
	
	return len;	
}

int32_t KeywordDict::filter( char * src, int32_t srclen, char * dst, int32_t dstlen, char * replace, int32_t replacelen )
{
	int32_t i = 0;
	int32_t len = 0;
	
	while ( i < srclen )
	{
		int32_t index = i;
		int32_t count = 0;

		i = matching( src, index, srclen, count );

		if ( count > 0 )
		{
			// 找到了关键字, 替换
			memcpy( dst+len, replace, replacelen );
			len += replacelen;
		}
		else
		{
			// 没有找到直接赋值
			dst[len] = src[index];
			len += 1;
		}
		
		if ( len >= dstlen )
		{
			len = 0;
			break;
		}
		
		++i;
	}
	
	dst[len] = 0;
	
	return len;	
}
