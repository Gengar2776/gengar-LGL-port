#pragma once


 

#include <string>
#include <functional>
#include <atomic>
#include <thread>
#include <vector>



struct ModStates
{
    std::atomic<bool> fly           { false };
    std::atomic<bool> esp           { false };
    std::atomic<bool> infiniteHealth{ false };
    std::atomic<bool> speedHack     { false };
    std::atomic<bool> noClip        { false };
    std::atomic<bool> godMode       { false };
    std::atomic<bool> unlimitedAmmo { false };
};


extern ModStates g_mods;


//  Callback type — called whenever any toggle changes
//  ayo glitch heres how to call, Signature: void callback(const std::string& modName, bool newState)

using ToggleCallback = std::function<void(const std::string&, bool)>;


namespace ModMenu
{
    /**
     * Start the embedded HTTP server on 127.0.0.1:8080.

     *
     * @param port           
     * @param onToggleChange  
     */
    void Init(int port = 8080, ToggleCallback onToggleChange = nullptr);

    
    void Shutdown();

    
    bool IsRunning();

   
    bool SetMod(const std::string& modName, bool state);

 
    bool GetMod(const std::string& modName);
}


#define MOD_FLY_ENABLED            (g_mods.fly.load())
#define MOD_ESP_ENABLED            (g_mods.esp.load())
#define MOD_INFINITE_HEALTH        (g_mods.infiniteHealth.load())
#define MOD_SPEED_HACK             (g_mods.speedHack.load())
#define MOD_NO_CLIP                (g_mods.noClip.load())
#define MOD_GOD_MODE               (g_mods.godMode.load())
#define MOD_UNLIMITED_AMMO         (g_mods.unlimitedAmmo.load())
