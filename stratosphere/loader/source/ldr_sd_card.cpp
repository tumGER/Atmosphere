#include <switch.h>
#include <stratosphere.hpp>
#include "ldr_sd_card.hpp"

static IWaitable *g_sd_card_event = NULL;
static FsEventNotifier g_fs_sd_notifier = {0};
static SdCardInsertionState g_sd_card_state = SdCardInsertionState_Removed;

static Result SdCardEventCallback(Handle *handles, size_t num_handles, u64 timeout) {
    /* Clear the event signal. */
    svcClearEvent(handles[0]);
    
    u32 x;
    svcReadWriteRegister(&x, 0x7000E400, 0xFFFFFFFF, 0x10);
    
    bool is_sdcard_accessible;
    switch (g_sd_card_state) {
        case SdCardInsertionState_Inserted:
            /* If the card was inserted, it is now removed. */
            g_sd_card_state = SdCardInsertionState_Removed;
            fsdevUnmountDevice("sdmc");
            break;
        case SdCardInsertionState_Removed:
            if (R_SUCCEEDED(SdCardUtils::IsSdCardAccessible(&is_sdcard_accessible)) && is_sdcard_accessible) {
                /* The SD card was inserted! */
                fsdevMountSdmc();                
                g_sd_card_state = SdCardInsertionState_Inserted;
            } else {
                /* This case happens sometimes -- the SD card isn't inserted, though. */
                g_sd_card_state = SdCardInsertionState_Removed;
            }
            break;
    }
    return 0;
}

Result SdCardUtils::IsSdCardAccessible(bool *out) {
    FsDeviceOperator dev_op = {0};
    Result rc;
    
    if (R_FAILED((rc = fsOpenDeviceOperator(&dev_op)))) {
        return rc;
    }
    
    rc = fsDeviceOperatorIsSdCardInserted(&dev_op, out);
    
    fsDeviceOperatorClose(&dev_op);
    return rc;
}

Result SdCardUtils::GetSdCardEvent(IWaitable **out) {
    Result rc = 0;
    Handle sd_event_hnd = 0;
    if (g_sd_card_event != NULL) {
        *out = g_sd_card_event;
        return 0x0;
    }
    
    if (R_SUCCEEDED((rc = fsOpenSdCardDetectionEventNotifier(&g_fs_sd_notifier)))) {
        if (R_SUCCEEDED((rc = fsEventNotifierGetEventHandle(&g_fs_sd_notifier, &sd_event_hnd)))) {
            g_sd_card_event = new WaitOnlyEvent(sd_event_hnd, &SdCardEventCallback);
        } else {
            fsEventNotifierClose(&g_fs_sd_notifier);
        }
    }
    
    if (R_SUCCEEDED(rc)) {
        *out = g_sd_card_event;
    }
    return rc;
}