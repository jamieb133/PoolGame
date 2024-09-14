#include "Application.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

static const char* LIB_NAME = "./libPoolGame_Game.dylib";

typedef void (*ApplicationInitFunc)(Application*, int, int, const char*);
typedef void (*ApplicationUpdateFunc)(Application*);
typedef void (*ApplicationDeinitFunc)(Application*);
typedef void (*ApplicationReloadFunc)(Application*);

static ApplicationInitFunc ApplicationInit;
static ApplicationUpdateFunc ApplicationUpdate;
static ApplicationDeinitFunc ApplicationDeinit;
static ApplicationReloadFunc ApplicationReload;

static time_t current_filemod_time_;
static void* handle_;

static void* LoadFunction(const char* name)
{
    void* fptr = dlsym(handle_, name);
    const char* err = dlerror();

    if (err)
    {
        fprintf(stderr, "Failed to locate symbol '%s': %s", name, err);
        dlclose(handle_);
        exit(1);
    }

    return fptr;
}

static void LoadLibrary()
{
    handle_ = dlopen(LIB_NAME, RTLD_LAZY);

    if (!handle_)
    {
        fprintf(stderr, "failed to load the game logic libary - %s", LIB_NAME);
        exit(1);
    }

    // Load the game logic functions.
    ApplicationInit = (ApplicationInitFunc) LoadFunction("Application_Init");
    ApplicationUpdate = (ApplicationUpdateFunc) LoadFunction("Application_Update");
    ApplicationDeinit = (ApplicationDeinitFunc) LoadFunction("Application_Deinit");
    ApplicationReload = (ApplicationReloadFunc) LoadFunction("Application_Reload");

    dlerror();// Reset DLL errors    
}

static time_t GetLastModifiedFiletime(const char* filename)
{
    struct stat attr;
    return (stat(filename, &attr) == 0) ? attr.st_mtime : 0;
}

static bool ShouldHotReload()
{
    // TODO: only poll this every half second.

    // Poll the modified file time.
    time_t const filemod_time = GetLastModifiedFiletime(LIB_NAME);
    if (filemod_time != current_filemod_time_)
    {
        current_filemod_time_ = filemod_time;
        return true;
    }

    return false;
}

int main()
{
    LoadLibrary();

    Application app;
    ApplicationInit(&app, 1200, 600, "Snookered!");

    current_filemod_time_ = GetLastModifiedFiletime(LIB_NAME);

    while (app.running) 
    {
        ApplicationUpdate(&app);

        if (ShouldHotReload())
        {
            printf("Game library (%s) updated, reloading", LIB_NAME);
            dlclose(handle_);
            LoadLibrary();
            ApplicationReload(&app);
        }
    }

    ApplicationDeinit(&app);
    dlclose(handle_);
}
