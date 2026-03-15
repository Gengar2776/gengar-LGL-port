# gengar-LGL-port
A standalone LGL port to run in the local browser

this is inspired by those old ass mod menus from happymod lmao


if you want the other menu https://github.com/LGLTeam/Android-Mod-Menu this is where this orginated from

# GenMenu/ Local Mod Menu (Android and Quest)

A lightweight **localhost-based mod menu** for Android games (tested for Unity / VR projects).
The menu runs an internal HTTP server and serves a web UI that can toggle mods in real time.
comes with its own logger for mods (also known as a notilib)

The UI is accessible from the device at:

```
http://127.0.0.1:8080
```

---

# Features

* Local HTTP mod menu
* Toggle mods in real time
* Simple C++ API
* Works on Android / Quest
* No external UI libraries required
* Runs entirely on `localhost`

---



# i also added a few buttons and stuff for people to start of with

# How It Works

1. The mod loads the `.so` library.
2. The HTTP server starts on **127.0.0.1:8080**.
3. The menu UI is served from memory.
4. When a button is clicked in the UI, it sends a request like:

```
/toggle?mod=fly&state=true
```

5. The mod state is updated inside the game.

---

# Project Structure

```
project/
│
├── modmenu.cpp
├── modmenu.h
├── httplib.h
├── ui_page.h
└── README.md
```



---

# Setup Guide

## 1. Add Files To Your Project

Copy the following files into your native project:

```
modmenu.cpp
modmenu.h
httplib.h
ui_page.h
```

---

## 2. Include the Menu

Include the header in your code:

```cpp
#include "modmenu.h"
```

---

## 3. Start the Menu

Start the server when your mod loads:

```cpp
ModMenu::Init(8080, nullptr);
```

This starts the menu on:

```
http://127.0.0.1:8080
```

---

## 4. Stop the Menu

(Optional) stop the server when unloading:

```cpp
ModMenu::Shutdown();

```

---

# Mod States

Mods are stored in a global structure:

```cpp
struct ModStates
{
    std::atomic<bool> fly;
    std::atomic<bool> esp;
    std::atomic<bool> infiniteHealth;
    std::atomic<bool> speedHack;
    std::atomic<bool> noClip;
    std::atomic<bool> godMode;
    std::atomic<bool> unlimitedAmmo;
};
```

---

# Reading Mod Values

Inside your game loop you can check mods like this:

```cpp
if (ModMenu::GetMod("fly"))
{
    // call ur fly logic here
}
```

Example:

```cpp
if (ModMenu::GetMod("speedHack"))
{
this would be called with dobby or whatever one uses to hook
    playerSpeed = 10.0f;
}
```

---

# Setting Mods From Code

Mods can also be changed directly:

```cpp
ModMenu::SetMod("fly", true);
```

---

# HTTP API

The menu exposes a small API.

### Get mod states

```
/status
```

Example response:

```json
{
 "fly": true,
 "esp": false
}
```

---

### Toggle mod

```
/toggle?mod=fly&state=true
```



# Adding New Mods

### Step 1 — Add to `ModStates`

```cpp
std::atomic<bool> superJump;
```

---

### Step 2 — Add to `buildModMap()`

```cpp
{ "superJump", &g_mods.superJump },
```

---

### Step 3 — Add button in UI

Inside `ui_page.h` add:

```html
<button onclick="toggleMod('superJump')">
Super Jump
</button>
```

---

# Notes

* The server runs **locally only**.
* It binds to:

```
127.0.0.1
```

So it is **not accessible from the network**.

---

# Example Usage

```
http://127.0.0.1:8080
```




Open the URL inside:

* Quest browser
* WebView
* Debug browser

Then toggle mods in the interface.

---

### ENJOY THIS TOOK A FEW DAYS 
if u need help blahblublahblu is my discord user js lmk ill dm u as fast as i can
