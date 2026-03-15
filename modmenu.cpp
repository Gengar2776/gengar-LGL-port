
#define CPPHTTPLIB_THREAD_POOL_COUNT 4
#include "httplib.h"


#include "modmenu.h"
#include "ui_page.h"


#include <string>
#include <thread>
#include <atomic>
#include <sstream>
#include <cstdio>
#include <functional>
#include <unordered_map>
#include <memory>

#ifdef __ANDROID__
#  include <android/log.h>
#  define LOG_TAG "ModMenu"
#  define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#  define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#else
#  define LOGI(...) do { printf("[ModMenu] "); printf(__VA_ARGS__); printf("\n"); fflush(stdout); } while(0)
#  define LOGE(...) do { fprintf(stderr,"[ModMenu][ERR] "); fprintf(stderr,__VA_ARGS__); fprintf(stderr,"\n"); } while(0)
#endif


ModStates g_mods;


namespace
{
    std::unique_ptr<httplib::Server> g_server;
    std::thread                      g_serverThread;
    std::atomic<bool>                g_running { false };
    ToggleCallback                   g_callback;

    using ModMap = std::unordered_map<std::string, std::atomic<bool>*>;

    ModMap buildModMap()
    {
        return {
            { "fly",            &g_mods.fly            },
            { "esp",            &g_mods.esp            },
            { "infiniteHealth", &g_mods.infiniteHealth },
            { "speedHack",      &g_mods.speedHack      },
            { "noClip",         &g_mods.noClip         },
            { "godMode",        &g_mods.godMode        },
            { "unlimitedAmmo",  &g_mods.unlimitedAmmo  },
        };
    }

    ModMap g_modMap = buildModMap();

 
    bool applyToggle(const std::string& mod, bool state)
    {
        auto it = g_modMap.find(mod);
        if (it == g_modMap.end()) return false;
        it->second->store(state, std::memory_order_relaxed);

    
        LOGI("%s %s", mod.c_str(), state ? "Enabled" : "Disabled");

   
        if (g_callback) g_callback(mod, state);
        return true;
    }

    
    std::string buildStatusJson()
    {
        std::ostringstream ss;
        ss << "{";
        bool first = true;
        for (auto& [name, ptr] : g_modMap)
        {
            if (!first) ss << ",";
            first = false;
            ss << "\"" << name << "\":" << (ptr->load() ? "true" : "false");
        }
        ss << "}";
        return ss.str();
    }


    void registerRoutes(httplib::Server& svr)
    {
        // CORS headers helper
        auto addCors = [](httplib::Response& res)
        {
            res.set_header("Access-Control-Allow-Origin",  "*");
            res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type");
        };

      
        svr.Get("/", [&addCors](const httplib::Request&, httplib::Response& res)
        {
            res.set_content(UI_PAGE, "text/html; charset=utf-8");
            addCors(res);
        });

        
        svr.Get("/toggle", [&addCors](const httplib::Request& req, httplib::Response& res)
        {
            addCors(res);

            if (!req.has_param("mod") || !req.has_param("state"))
            {
                res.status = 400;
                res.set_content(R"({"error":"missing mod or state param"})", "application/json");
                return;
            }

            const std::string modName  = req.get_param_value("mod");
            const std::string stateStr = req.get_param_value("state");
            bool newState = (stateStr == "1" || stateStr == "true");

            if (!applyToggle(modName, newState))
            {
                res.status = 404;
                res.set_content(R"({"error":"unknown mod"})", "application/json");
                return;
            }

            res.set_content(R"({"ok":true})", "application/json");
        });

      
        svr.Get("/status", [&addCors](const httplib::Request&, httplib::Response& res)
        {
            addCors(res);
            res.set_content(buildStatusJson(), "application/json");
        });

 
        svr.Options(".*", [&addCors](const httplib::Request&, httplib::Response& res)
        {
            addCors(res);
            res.status = 204;
        });


        svr.Get("/favicon.ico", [](const httplib::Request&, httplib::Response& res)
        {
            res.status = 204;
        });
    }

} 

void ModMenu::Init(int port, ToggleCallback onToggleChange)
{
    if (g_running.load())
    {
        LOGE("ModMenu::Init called but server is already running.");
        return;
    }

    g_callback = std::move(onToggleChange);

    g_serverThread = std::thread([port]()
    {
        g_server = std::make_unique<httplib::Server>();

        // Bind to loopback only — never exposed to the network
        if (!g_server->bind_to_port("127.0.0.1", port))
        {
            LOGE("Failed to bind HTTP server to 127.0.0.1:%d", port);
            return;
        }

        registerRoutes(*g_server);
        g_running.store(true);

        LOGI("HTTP server started at http://127.0.0.1:%d", port);
        LOGI("Open that URL in your browser to access the Mod Menu.");

        g_server->listen_after_bind();  

        g_running.store(false);
        LOGI("HTTP server stopped.");
    });

    g_serverThread.detach();  
}

void ModMenu::Shutdown()
{
    if (g_server && g_running.load())
    {
        g_server->stop();
     
        LOGI("ModMenu shutdown requested.");
    }
}

bool ModMenu::IsRunning()
{
    return g_running.load();
}

bool ModMenu::SetMod(const std::string& modName, bool state)
{
    return applyToggle(modName, state);
}

bool ModMenu::GetMod(const std::string& modName)
{
    auto it = g_modMap.find(modName);
    if (it == g_modMap.end()) return false;
    return it->second->load(std::memory_order_relaxed);
}



#ifdef __ANDROID__

#include <jni.h>

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* /*vm*/, void* /*reserved*/)
{
    ModMenu::Init(8080);
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* /*vm*/, void* /*reserved*/)
{
    ModMenu::Shutdown();
}


__attribute__((constructor))
static void onSoLoad()
{
    
    if (!ModMenu::IsRunning())
        ModMenu::Init(8080);
}

__attribute__((destructor))
static void onSoUnload()
{
    ModMenu::Shutdown();
}

