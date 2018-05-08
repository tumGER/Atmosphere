#pragma once
#include <switch.h>

#include "iwaitable.hpp"
#include "ievent.hpp"

class WaitOnlyEvent : public IEvent {
    public:
        WaitOnlyEvent(Handle wait_h, EventCallback callback) : IEvent(wait_h, callback) {
            
        }
        
        virtual Result signal_event() {
            fatalSimple(0xCAFE << 4 | 0xDD);
        }  
};