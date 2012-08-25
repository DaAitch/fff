
#ifdef _TEST

#include "../../include/_intern.h"
#include "../../include/Testing/testintern.h"
#include "../../include/Testing/Base/_testingbase.h"

using namespace fff::_Intern;
using namespace fff::Testing::Intern;
using namespace fff::Testing::Base;

namespace fff {
namespace Testing {

fff_TEST_BEGIN_IMPL(fff, intern)

    fff_TEST_CALL(intern, caster);

fff_TEST_END_IMPL()

namespace Intern {

fff_TEST_BEGIN_IMPL(intern, caster)
    // binary
    // valid
    fff_TEST_EQ(BIN(O),0);
    fff_TEST_EQ(BIN(I),1);
    fff_TEST_EQ(BIN(III),7);
    fff_TEST_EQ(BIN(IOOOO), 16);
    fff_TEST_EQ(BIN(IOIOI), 21);
    fff_TEST_EQ(BIN(IIIIIOOO), 248);
    fff_TEST_EQ(BIN(IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII),
        0xFFFFFFFF);

    // invalid
    fff_TEST_NOK(BINCASTER(Ix));
    fff_TEST_NOK(BINCASTER(IaOb4));
    fff_TEST_NOK(BINCASTER(I3I2I));
    fff_TEST_NOK(BINCASTER(IIIIIyyy));

    // octal
    // valid
    fff_TEST_EQ(OCT(0),0);
    fff_TEST_EQ(OCT(1),1);
    fff_TEST_EQ(OCT(7),7);
    fff_TEST_EQ(OCT(10),8);
    fff_TEST_EQ(OCT(157),111);
    fff_TEST_EQ(OCT(01234567),342391);

    // invalid
    fff_TEST_NOK(OCTCASTER(o1));
    fff_TEST_NOK(OCTCASTER(1x));
    fff_TEST_NOK(OCTCASTER(8));
    fff_TEST_NOK(OCTCASTER(22.3333));

    // decimal
    // valid
    fff_TEST_EQ(DEC(0),0);
    fff_TEST_EQ(DEC(1),1);
    fff_TEST_EQ(DEC(9),9);
    fff_TEST_EQ(DEC(10),10);
    fff_TEST_EQ(DEC(999),999);
    fff_TEST_EQ(DEC(0123456789),123456789);

    // invalid
    fff_TEST_NOK(DECCASTER(d1));
    fff_TEST_NOK(DECCASTER(1x));
    fff_TEST_NOK(DECCASTER(x));
    fff_TEST_NOK(DECCASTER(22.3333));
    fff_TEST_NOK(DECCASTER(a));
    
    // hexadecimal
    // valid
    fff_TEST_EQ(HEX(0),0x0);
    fff_TEST_EQ(HEX(1),0x1);
    fff_TEST_EQ(HEX(9),0x9);
    fff_TEST_EQ(HEX(10),0x10);
    fff_TEST_EQ(HEX(a),0xa);
    fff_TEST_EQ(HEX(999),0x999);
    fff_TEST_EQ(HEX(012345678),
        0x012345678);
    fff_TEST_EQ(HEX(abcdef),
        0xabcdef);

    // invalid
    fff_TEST_NOK(HEXCASTER(d1z));
    fff_TEST_NOK(HEXCASTER(1x));
    fff_TEST_NOK(HEXCASTER(x));
    fff_TEST_NOK(HEXCASTER(22.3333));
    fff_TEST_NOK(HEXCASTER(3248ecg));

fff_TEST_END_IMPL()

}
}
}

#endif