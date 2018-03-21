
#ifndef __SRC_DOMAIN_KEYWORDDICT_H__
#define __SRC_DOMAIN_KEYWORDDICT_H__

#include <stdint.h>

class KeywordNode;

// 字典
class KeywordDict
{
public :
    KeywordDict();
    ~KeywordDict();

public :
    // 获取节点个数
    uint32_t count() const { return m_Count; }

    // 增加关键字
    // TODO: DFA Minimization
    int32_t add( const char * words, int32_t len = 0 );

    // 检查字符串中是否有关键字
    bool check( const char * words, int32_t len );

    // 过滤消息(默认每个关键字用*代替, 直接修改源字符串)
    //          words       - 需要过滤的消息
    //          len         - 过滤消息的长度
    int32_t filter( char * words, int32_t len );

    // 过滤消息
    //          src         - 需要过滤的字符串
    //          srclen      - 需要过滤的字符串长度
    //          dst         - 过滤后的字符串
    //          dstlen      - 存放过滤后字符串的空间长度
    //          replace     - 关键字用replace来替换
    //          replacelen  - 替换字符串的长度
    int32_t filter( char * src, int32_t srclen,
            char * dst, int32_t dstlen,
            char * replace, int32_t replacelen );

private :
    // 分配内存块
    KeywordNode * allocate( uint8_t c );

    // 插入
    void insert( KeywordNode * node,
            const char * words, int32_t len, int32_t index );

    // 匹配
    int32_t matching( const char * words,
            int32_t index, int32_t len, int32_t & count );

private :
    uint32_t            m_Count;
    KeywordNode *       m_RootNode;
};

#endif
