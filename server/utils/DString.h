#ifndef DSTRING_H
#define DSTRING_H
#include <string>
#include <vector>
#include <cstdint>
#include <utility>
template<typename It>
uint32_t LCSubstr(It beg1,It end1,It beg2,It end2,bool ignoreCase = false)
{
    uint32_t len1 = end1 - beg1;
    uint32_t len2 = end2 - beg2;
    dvector<uint32_t> matchLenVec(len2,0);

    uint32_t maxMatchLen = 0;    //匹配的长度
    uint32_t pos = 0;        //在str2上的匹配最末位置
    for(uint32_t i = 0 ; i < len1 ; ++i,++beg1)
    {
        It last2 = end2 - 1;
        for(int32_t j = len2 - 1; j >= 0 ; --j,--last2)  //更新时从后往前遍历
        {
            if(*beg1 == *last2 || (ignoreCase && 32 == std::abs(*beg1 - *last2) ))
            {
                if(0 == i || 0 == j)
                {
                    matchLenVec[j] = 1;
                }
                else
                {
                    matchLenVec[j] = matchLenVec[j-1] + 1;
                }

                if(matchLenVec[j] > maxMatchLen )
                {
                    maxMatchLen  = matchLenVec[j];
                    pos = j-1;
                }
            }
            else
            {
                matchLenVec[j] = 0;
            }
        }
    }

    return maxMatchLen ;
}

template<typename T1,typename T2>
std::pair<T1,uint32_t> Match(T1 begStr,T1 endStr,T2 matchBeg,T2 matchEnd,bool ignoreCase = false)
{
    uint32_t maxMatchLen = 0;

    T1 ret = begStr;
    for(T1 it = begStr;it != endStr; ++it)
    {
        int matchLen = LCSubstr(matchBeg,matchEnd,it->begin(),it->end(),ignoreCase);
        if(matchLen > maxMatchLen)
        {
            maxMatchLen = matchLen;
            ret = it;
        }
    }

    return std::make_pair(ret,maxMatchLen);
}
#endif // DSTRING_H
