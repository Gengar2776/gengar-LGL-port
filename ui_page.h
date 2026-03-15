#pragma once

static const char* UI_PAGE = R"HTMLPAGE(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Mod Menu</title>

<style>

body {
    background: #0b0b0b;
    font-family: Consolas, monospace;
    color: #00ff88;
    margin: 0;
}

#menu {
    width: 320px;
    margin: 40px auto;
    background: #111;
    border: 2px solid #00ff88;
    box-shadow: 0 0 15px #00ff88;
}

#title {
    background: #00ff88;
    color: black;
    text-align: center;
    font-weight: bold;
    padding: 8px;
}

.mod {
    display: flex;
    justify-content: space-between;
    padding: 10px;
    border-bottom: 1px solid #222;
}

.mod button {
    background: #111;
    color: #00ff88;
    border: 1px solid #00ff88;
    padding: 4px 10px;
    cursor: pointer;
}

.mod button:hover {
    background: #00ff88;
    color: black;
}

.footer {
    text-align: center;
    font-size: 12px;
    padding: 6px;
    color: #888;
}

</style>
</head>

<body>

<div id="menu">

<div id="title">LOCALHOST MOD MENU</div>

<div class="mod">
<span>Fly</span>
<button onclick="toggle('fly')">TOGGLE</button>
</div>

<div class="mod">
<span>No Clip</span>
<button onclick="toggle('noClip')">TOGGLE</button>
</div>

<div class="mod">
<span>Speed Hack</span>
<button onclick="toggle('speedHack')">TOGGLE</button>
</div>

<div class="mod">
<span>Infinite Health</span>
<button onclick="toggle('infiniteHealth')">TOGGLE</button>
</div>

<div class="mod">
<span>God Mode</span>
<button onclick="toggle('godMode')">TOGGLE</button>
</div>

<div class="mod">
<span>Unlimited Ammo</span>
<button onclick="toggle('unlimitedAmmo')">TOGGLE</button>
</div>

<div class="mod">
<span>ESP</span>
<button onclick="toggle('esp')">TOGGLE</button>
</div>

<div class="footer">
127.0.0.1:8080
</div>

</div>

<script>

function toggle(mod)
{
    fetch("/toggle?mod=" + mod + "&state=1")
}

</script>

</body>
</html>
)HTMLPAGE";
