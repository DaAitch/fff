
#ifndef __runtime_h__included__
#define __runtime_h__included__

#include "_base.h"
#include "typecast.h"
#include "exceptions.h"

using namespace fff::_Intern;

namespace fff {
namespace _Intern {
namespace Exceptions {
namespace Runtime {

/*
// Maybe to be implemented in fff_EXPECT,
// not to compute LHS and RHS twice,
// for the comparison and the
// stringified argument
// workaround of "Cx11" auto keyword
template<class T>
class __storecopy
{
public:
    __storecopy(const T &any)
        :
        m_any(any) { }
    T &operator() { return m_any; }

private:
    T m_any;
};

template<class T>
class __storeref
{
public:
    __storeref(const T &any)
        :
        m_any(any) { }
    T &operator() { return m_any; }

private:
    T &m_any;
};
*/

#if defined(_fff_TEST) && 0

#define fff_EXPECT(LHS, REL, RHS) \
    do{ if(!((LHS) REL (RHS))) { \
        fff_ERR(InvalidRelation, \
            "Relation", #LHS, (LHS), #REL, #RHS, (RHS) \
        );}}while(0)

#define fff_EXPECT_FALSE(X) \
    do{ if(!!(X)) { \
        fff_ERR(WrongBool, \
            #X, (X), "False" \
        );}}while(0)

#define fff_EXPECT_TRUE(X) \
    do{ if(!(X)) { \
        fff_ERR(WrongBool, \
            #X, (X), "True" \
        );}}while(0)

#define fff_EXPECT_PARAM(LHS, REL, RHS) \
    do{ if(!((LHS) REL (RHS))) { \
        fff_ERR(InvalidRelation, \
            "Parameter", #LHS, (LHS), #REL, #RHS, (RHS) \
        );}}while(0)

#define fff_EXPECT_PARAM_NOTNULL(X) \
    fff_EXPECT_PARAM((X), !=, 0)

#define fff_EXPECT_VALID_OBJ(OBJ) \
    do{ if(!(OBJ)) { \
        fff_ERR(InvalidObject, #OBJ);}}while(0)

#define fff_EXPECT_INVALID_OBJ(OBJ) \
    do{ if(!!(OBJ)) { \
        fff_ERR(ValidObject, #OBJ);}}while(0)

#define fff_EXPECT_VALID_OBJ_RET(OBJ) \
    do{ if(!(OBJ)) { \
        fff_ERR(InvalidObject, #OBJ);} \
        return (OBJ); }while(0)

#define fff_EXPECT_VALID_THIS() \
    fff_EXPECT_VALID_OBJ(*this)

#define fff_EXPECT_INVALID_THIS() \
    fff_EXPECT_INVALID_OBJ(*this)

#define fff_EXPECT_VALID_PTR(PTR) \
    do{ if((PTR)==NULL) { \
        fff_ERR(InvalidPointer);}}while(0)

#define fff_EXPECT_NULL_PTR(PTR) \
    do{ if((PTR)!=NULL) { \
        fff_ERR(InvalidPointer);}}while(0)

#define fff_UNEXPECTED_PATH() \
    do{ fff_ERR(UnexpectedPath); }while(0)

#else

#define fff_EXPECT(LHS, REL, RHS) do{}while(0)
#define fff_EXPECT_FALSE(X) do{}while(0)
#define fff_EXPECT_TRUE(X) do{}while(0)
#define fff_EXPECT_PARAM(LHS, REL, RHS) do{}while(0)
#define fff_EXPECT_PARAM_NOTNULL(X) do{}while(0)
#define fff_EXPECT_VALID_OBJ(OBJ) do{}while(0)
#define fff_EXPECT_INVALID_OBJ(OBJ) do{}while(0)
#define fff_EXPECT_VALID_OBJ_RET(OBJ) return (OBJ)
#define fff_EXPECT_VALID_THIS() do{}while(0)
#define fff_EXPECT_INVALID_THIS() do{}while(0)
#define fff_EXPECT_VALID_PTR(PTR) do{}while(0)
#define fff_EXPECT_NULL_PTR(PTR) do{}while(0)
#define fff_UNEXPECTED_PATH() do{}while(0)
#endif

fff_EXCEPTION_WITH_REPORT_BEGIN(WrongBool)
(
    const stringified &expr,
    const stringified &value,
    const stringified &expected
)
{
    getReport() << "Expected '" <<
        expr << "' (=\"" << value << 
        "\") to be \"" << expected << "\"";
}
fff_EXCEPTION_WITH_REPORT_END()

fff_EXCEPTION_WITH_REPORT_BEGIN(InvalidRelation)
(
    const stringified &category,
    const stringified &expr,
    const stringified &value,
    const stringified &relation,
    const stringified &expectedExpr,
    const stringified &expectedValue
)
{
    getReport() << category <<
        " Error: " <<
        "Expected '" <<
        expr <<
        "' (=\"" <<
        value <<
        "\") to be " <<
        relation <<
        " '" <<
        expectedExpr <<
        "' (=\"" <<
        expectedValue <<
        "\")";
}
fff_EXCEPTION_WITH_REPORT_END()





fff_EXCEPTION_WITH_REPORT_BEGIN(ProgramBuild)
(
    const stringified &buildlog
)
{
    getReport() << buildlog;
}
fff_EXCEPTION_WITH_REPORT_END()




fff_EXCEPTION_WITH_REPORT_BEGIN(InvalidObject)
(
    const stringified &obj
)
{
    getReport() << "Invalid object: \"" << obj << "\"";
}
fff_EXCEPTION_WITH_REPORT_END()

fff_EXCEPTION_WITH_REPORT_BEGIN(InvalidPointer)
(
)
{
    getReport() << "Invalid pointer";
}
fff_EXCEPTION_WITH_REPORT_END()

fff_EXCEPTION_WITH_REPORT_BEGIN(UnexpectedPath)
(
)
{
    getReport() << "Unexpected execution path";
}
fff_EXCEPTION_WITH_REPORT_END()



}
}
}
}

#endif