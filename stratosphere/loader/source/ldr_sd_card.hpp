#pragma once
#include <switch.h>
#include <stratosphere.hpp>

enum SdCardInsertionState {
    SdCardInsertionState_Removed,
    SdCardInsertionState_Inserted
};

class SdCardUtils {
    public:
        static Result IsSdCardAccessible(bool *out);
        static Result GetSdCardEvent(IWaitable **out);
};