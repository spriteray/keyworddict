
#include <map>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

#include "keyworddict.h"

class KeywordNode
{
public :
    KeywordNode( uint8_t c )
        : m_Char( c ),
          m_IsEnding( false )
    {}

    ~KeywordNode()
    {
        KeywordNodes::iterator it = m_NextNodes.begin();
        for ( ; it != m_NextNodes.end(); ++it )
        {
            KeywordNode * node = it->second;
            if ( node )
            {
                delete node;
            }
        }
        m_NextNodes.clear();
    }

public :
    KeywordNode * find( uint8_t c ) const
    {
        KeywordNode * node = NULL;
        KeywordNodes::const_iterator pos;

        pos =  m_NextNodes.find( c );
        if ( pos != m_NextNodes.end() )
        {
            node = pos->second;
        }

        return node;
    }

    void add( uint8_t c, KeywordNode * node )
    {
        m_NextNodes.insert( std::make_pair( c, node ) );
    }

    uint8_t getChar() const { return m_Char; }

    void setEnding() { m_IsEnding = true; }
    bool isEnding() const { return m_IsEnding; }

private :
    typedef std::map<uint8_t, KeywordNode *> KeywordNodes;

    uint8_t             m_Char;
    bool                m_IsEnding;
    KeywordNodes        m_NextNodes;
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

KeywordDict::KeywordDict()
    : m_Count( 0 )
{
    m_RootNode = new KeywordNode( 0 );
    assert( m_RootNode != NULL );
}

KeywordDict::~KeywordDict()
{
    delete m_RootNode;

    m_Count = 0;
    m_RootNode = NULL;
}

// TODO: DFA Minimization
int32_t KeywordDict::add( const char * words, int32_t len )
{
    len = ( len == 0 ? (int32_t)std::strlen(words) : len );
    if ( len <= 0 )
    {
        return -1;
    }

    this->insert( m_RootNode, words, len, 0 );
    return 0;
}

bool KeywordDict::check( const char * words, int32_t len )
{
    int32_t i = 0;

    while ( i < len )
    {
        int32_t index = i;
        int32_t count = 0;

        i = this->matching( words, index, len, count );

        // 自增
        ++i;

        // 找到了关键字
        if ( count > 0 )
        {
            return true;
        }
    }

    return false;
}

int32_t KeywordDict::filter( char * words, int32_t len )
{
    int32_t i = 0;

    while ( i < len )
    {
        int32_t index = i;
        int32_t count = 0;

        i = this->matching( words, index, len, count );

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

        i = this->matching( src, index, srclen, count );

        if ( count > 0 )
        {
            // 找到了关键字, 替换
            std::memcpy( dst+len, replace, replacelen );
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

KeywordNode * KeywordDict::allocate( uint8_t c )
{
#if 0
    if ( m_UsedBlock == NULL
            || m_Allocated >= NODE_COUNT_PER_BLOCK - 1 )
    {
        m_UsedBlock = std::malloc( sizeof(KeywordNode) * NODE_COUNT_PER_BLOCK );
        assert( m_UsedBlock != NULL && "allocate m_UsedBlock failed" );

        m_Allocated = 0;
        m_BlockList.push_back( m_UsedBlock );
    }

    return new ( (KeywordNode *)m_UsedBlock+(m_Allocated++) ) KeywordNode( c );
#endif
    ++m_Count;
    return new KeywordNode( c );
}

void KeywordDict::insert( KeywordNode * node, const char * words, int32_t len, int32_t index )
{
    KeywordNode * n = NULL;

    n = node->find( words[index] );
    if ( n == NULL )
    {
        n = this->allocate( words[index] );
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
        this->insert( n, words, len, index );
    }

    return;
}

int32_t KeywordDict::matching( const char * words, int32_t index, int32_t len, int32_t & count )
{
    int32_t i = index;
    KeywordNode * node = m_RootNode;

    while ( i < len )
    {
        node = node->find( words[i] );
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
