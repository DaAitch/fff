
#ifndef __events_h__included__
#define __events_h__included__

#include "../_intern.h"

namespace fff {
namespace Computing {

    class Events
    {
    public:
        std::vector<::cl::Event> m_events;

        Events(UInt eventCount)
            :
            m_events(eventCount)
        {
        }

        void wait()
        {
            ::cl::WaitForEvents(m_events);
        }
    };
}
}


#endif