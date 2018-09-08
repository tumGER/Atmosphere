/*
 * Copyright (c) 2018 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#pragma once
#include <switch.h>
#include <stratosphere/iserviceobject.hpp>

#include "pm_registration.hpp"

#define BOOT2_TITLE_ID (0x0100000000000008ULL)

enum ShellCmd {
    Shell_Cmd_LaunchProcess = 0,
    Shell_Cmd_TerminateProcessId = 1,
    Shell_Cmd_TerminateTitleId = 2,
    Shell_Cmd_GetProcessWaitEvent = 3,
    Shell_Cmd_GetProcessEventType = 4,
    Shell_Cmd_FinalizeExitedProcess = 5,
    Shell_Cmd_ClearProcessNotificationFlag = 6,
    Shell_Cmd_NotifyBootFinished = 7,
    Shell_Cmd_GetApplicationProcessId = 8,
    Shell_Cmd_BoostSystemMemoryResourceLimit = 9
};

enum ShellCmd_5X {
    Shell_Cmd_5X_LaunchProcess = 0,
    Shell_Cmd_5X_TerminateProcessId = 1,
    Shell_Cmd_5X_TerminateTitleId = 2,
    Shell_Cmd_5X_GetProcessWaitEvent = 3,
    Shell_Cmd_5X_GetProcessEventType = 4,
    Shell_Cmd_5X_NotifyBootFinished = 5,
    Shell_Cmd_5X_GetApplicationProcessId = 6,
    Shell_Cmd_5X_BoostSystemMemoryResourceLimit = 7
};

class ShellService final : public IServiceObject {
    public:
        Result dispatch(IpcParsedCommand &r, IpcCommand &out_c, u64 cmd_id, u8 *pointer_buffer, size_t pointer_buffer_size) override;
        Result handle_deferred() override;
        
        ShellService *clone() override {
            return new ShellService(*this);
        }
        
        
    private:
        /* Actual commands. */
        std::tuple<Result, u64> launch_process(u64 launch_flags, Registration::TidSid tid_sid);
        std::tuple<Result> terminate_process_id(u64 pid);
        std::tuple<Result> terminate_title_id(u64 tid);
        std::tuple<Result, CopiedHandle> get_process_wait_event();
        std::tuple<Result, u64, u64> get_process_event_type();
        std::tuple<Result> finalize_exited_process(u64 pid);
        std::tuple<Result> clear_process_notification_flag(u64 pid);
        std::tuple<Result> notify_boot_finished();
        std::tuple<Result, u64> get_application_process_id();
        std::tuple<Result> boost_system_memory_resource_limit(u64 sysmem_size);
};
