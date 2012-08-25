
#ifndef ___typecast_h__included__
#define ___typecast_h__included__


#include "_types.h"
#include <string>
#include <ostream>

using namespace std;
using namespace fff::_Intern;

namespace fff {
namespace _Intern {

#define CASTER(T, X) \
    ::fff::_Intern::caster<T>(#X)

#define CAST(T, X) \
    (T)CASTER(T, X)


#define BIN(X) \
    CAST(::fff::UInt, b##X)

#define OCT(X) \
    CAST(::fff::UInt, o##X)

#define DEC(X) \
    CAST(::fff::UInt, d##X)

#define HEX(X) \
    CAST(::fff::UInt, x##X)


#define BINCASTER(X) \
    CASTER(::fff::UInt, b##X)

#define OCTCASTER(X) \
    CASTER(::fff::UInt, o##X)

#define DECCASTER(X) \
    CASTER(::fff::UInt, d##X)

#define HEXCASTER(X) \
    CASTER(::fff::UInt, x##X)


template<class T>
class caster
{
public:

    static const char HEX = 'x';
    static const char DEC = 'd';
    static const char OCT = 'o';
    static const char BIN = 'b';


    caster(const char *num)
        :
        m_num(0),
        m_invalid(false)
    {
        size_t n = strlen(num);
        if(n > 1)
        {
            switch(*num)
            {
            case BIN:
                m_invalid = !castBin(m_num, num+1, n-1);
                break;
            case DEC:
                m_invalid = !castDec(m_num, num+1, n-1);
                break;
            case OCT:
                m_invalid = !castOct(m_num, num+1, n-1);
                break;
            case HEX:
                m_invalid = !castHex(m_num, num+1, n-1);
                break;
            default:
                m_invalid = true;
            }
        }
        else
            m_invalid = true;
    }

    operator T()
    {
        return m_num;
    }

    template<class T>
    static bool castBin(T &d, const char *num, size_t n)
    {
        d=0;
        T i = 1;
        for(
            const char *c = num + n - 1;
            c >= num;
            --c)
        {
            if(*c=='O')
            {}
            else if(*c=='I')
                d += i;
            else
                return false;

            i*=2;
        }

        return true;
    }
    
    template<class T>
    static bool castDec(T &d, const char *num, size_t n)
    {
        d=0;

        T i = 1;
        for(
            const char *c = num + n - 1;
            c >= num;
            --c)
        {
            if(*c>='0' && *c<='9')
                d += ((T)(*c)-(T)'0')*i;
            else
                return false;

            i*=10;
        }

        return true;
    }

    template<class T>
    static bool castOct(T &d, const char *num, size_t n)
    {
        d=0;

        T i = 1;
        for(
            const char *c = num + n - 1;
            c >= num;
            --c)
        {
            if(*c>='0' && *c<='7')
                d += ((T)(*c)-(T)'0')*i;
            else
                return false;

            i*=8;
        }

        return true;
    }
    
    template<class T>
    static bool castHex(T &d, const char *num, size_t n)
    {
        d=0;

        T i = 1;
        for(
            const char *c = num + n - 1;
            c >= num;
            --c)
        {
            if(*c>='0' && *c<='9')
                d += ((T)(*c)-(T)'0')*i;
            else if(*c>='a' && *c<='f')
                d += (((T)(*c)-(T)'a')+10)*i;
            else if(*c>='A' && *c<='F')
                d += (((T)(*c)-(T)'A')+10)*i;
            else
                return false;

            i*=16;
        }

        return true;
    }

    bool operator!() const
    {
        return
            m_invalid;
    }
    

private:
    T m_num;
    bool m_invalid;
};


class stringified
{
public:
    template<class T>
    stringified(const T &x)
    {
        StringStream ss;
        ss<<x;
        m_string = ss.str();
    }

    stringified()
    {
    }

    operator String() const
    {
        return
            getString();
    }

    String getString() const
    {
        return
            m_string;
    }

private:
    String
        m_string;
};




}
}

ostream &operator<<(ostream &stream, const stringified &rhs);

#endif