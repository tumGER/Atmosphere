#include <switch.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include "ldr_registration.hpp"
#include "ldr_content_management.hpp"

static FsFileSystem g_CodeFileSystem = {0};

static std::vector<u64> g_created_titles;

Result ContentManagement::MountCode(u64 tid, FsStorageId sid) {
    char path[FS_MAX_PATH] = {0};
    Result rc;
        
    if (R_FAILED(rc = ResolveContentPath(path, tid, sid))) {
        return rc;
    }
    
    /* Fix up path. */
    for (unsigned int i = 0; i < FS_MAX_PATH && path[i] != '\x00'; i++) {
        if (path[i] == '\\') {
            path[i] = '/';
        }
    }
    
    /* Always re-initialize fsp-ldr, in case it's closed */
    if (R_FAILED(rc = fsldrInitialize())) {
        return rc;
    }
    
    if (R_FAILED(rc = fsldrOpenCodeFileSystem(tid, path, &g_CodeFileSystem))) {
        fsldrExit();
        return rc;
    }
    
    fsdevMountDevice("code", g_CodeFileSystem);
    
    return rc;
}

Result ContentManagement::UnmountCode() {
    fsdevUnmountDevice("code");
    return 0;
}

Result ContentManagement::MountCodeForTidSid(Registration::TidSid *tid_sid) {
    return MountCode(tid_sid->title_id, tid_sid->storage_id);
}

Result ContentManagement::ResolveContentPath(char *out_path, u64 tid, FsStorageId sid) {
    Result rc;
    LrRegisteredLocationResolver reg;
    LrLocationResolver lr;
    char path[FS_MAX_PATH] = {0};
    
    /* Try to get the path from the registered resolver. */
    if (R_FAILED(rc = lrOpenRegisteredLocationResolver(&reg))) {
        return rc;
    }
    
    if (R_SUCCEEDED(rc = lrRegLrResolveProgramPath(&reg, tid, path))) {
        strncpy(out_path, path, sizeof(path));
    } else if (rc != 0x408) {
        return rc;
    }
    
    serviceClose(&reg.s);
    if (R_SUCCEEDED(rc)) {
        return rc;
    }
    
    /* If getting the path from the registered resolver fails, fall back to the normal resolver. */
    if (R_FAILED(rc = lrOpenLocationResolver(sid, &lr))) {
        return rc;
    }
    
    if (R_SUCCEEDED(rc = lrLrResolveProgramPath(&lr, tid, path))) {
        strncpy(out_path, path, sizeof(path));
    }
    
    serviceClose(&lr.s);
    
    return rc;
}

Result ContentManagement::ResolveContentPathForTidSid(char *out_path, Registration::TidSid *tid_sid) {
    return ResolveContentPath(out_path, tid_sid->title_id, tid_sid->storage_id);
}

Result ContentManagement::RedirectContentPath(const char *path, u64 tid, FsStorageId sid) {
    Result rc;
    LrLocationResolver lr;
    
    if (R_FAILED(rc = lrOpenLocationResolver(sid, &lr))) {
        return rc;
    }
    
    rc = lrLrRedirectProgramPath(&lr, tid, path);
    
    serviceClose(&lr.s);
    
    return rc;
}

Result ContentManagement::RedirectContentPathForTidSid(const char *path, Registration::TidSid *tid_sid) {
    return RedirectContentPath(path, tid_sid->title_id, tid_sid->storage_id);
}

bool ContentManagement::HasCreatedTitle(u64 tid) {
    return std::find(g_created_titles.begin(), g_created_titles.end(), tid) != g_created_titles.end();
}

void ContentManagement::SetCreatedTitle(u64 tid) {
    if (!HasCreatedTitle(tid)) {
        g_created_titles.push_back(tid);
    }
}
