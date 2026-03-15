#pragma once

/*
 * ui_page.h — Embedded HTML/CSS/JS mod menu UI
 *
 * The entire browser interface lives here as a raw string literal.
 * Served by the HTTP server when GET / is requested.
 * No external files needed — self-contained inside the library.
 */

static const char* UI_PAGE = R"HTMLPAGE(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0">
<title>Mod Menu</title>
<style>
  
  *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

  @import url('https://fonts.googleapis.com/css2?family=Rajdhani:wght@400;500;600;700&family=Share+Tech+Mono&display=swap');

  :root {
    --bg:          #0a0c10;
    --panel:       #111318;
    --panel2:      #181c24;
    --border:      #2a2f3d;
    --accent:      #00e5a0;
    --accent2:     #7c3aed;
    --accent-glow: rgba(0,229,160,0.25);
    --text:        #e2e8f0;
    --text-dim:    #64748b;
    --danger:      #f43f5e;
    --on-track:    #00e5a0;
    --off-track:   #2a2f3d;
    --radius:      14px;
    --font-ui:     'Rajdhani', sans-serif;
    --font-mono:   'Share Tech Mono', monospace;
    --shadow:      0 8px 40px rgba(0,0,0,0.6);
  }

  html, body {
    width: 100%; height: 100%;
    background: var(--bg);
    font-family: var(--font-ui);
    color: var(--text);
    overflow: hidden;
    user-select: none;
  }

  
  body::before {
    content: '';
    position: fixed; inset: 0; z-index: 0;
    background-image:
      linear-gradient(rgba(0,229,160,0.04) 1px, transparent 1px),
      linear-gradient(90deg, rgba(0,229,160,0.04) 1px, transparent 1px);
    background-size: 40px 40px;
    animation: gridScroll 20s linear infinite;
    pointer-events: none;
  }
  @keyframes gridScroll {
    from { background-position: 0 0; }
    to   { background-position: 40px 40px; }
  }


  body::after {
    content: '';
    position: fixed;
    width: 300px; height: 300px;
    border-radius: 50%;
    background: radial-gradient(circle, rgba(124,58,237,0.15) 0%, transparent 70%);
    bottom: -80px; right: -80px;
    pointer-events: none;
  }


  #panel {
    position: fixed;
    top: 60px; left: 50%;
    transform: translateX(-50%);
    width: 340px;
    background: var(--panel);
    border: 1px solid var(--border);
    border-radius: var(--radius);
    box-shadow: var(--shadow), 0 0 0 1px rgba(0,229,160,0.08) inset;
    z-index: 10;
    overflow: hidden;
    animation: panelIn 0.4s cubic-bezier(0.34,1.56,0.64,1) both;
    backdrop-filter: blur(20px);
  }
  @keyframes panelIn {
    from { opacity:0; transform: translateX(-50%) scale(0.88) translateY(-20px); }
    to   { opacity:1; transform: translateX(-50%) scale(1)    translateY(0); }
  }


  #titlebar {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 14px 18px;
    background: var(--panel2);
    border-bottom: 1px solid var(--border);
    cursor: grab;
    position: relative;
    overflow: hidden;
  }
  #titlebar::after {
    content: '';
    position: absolute;
    bottom: 0; left: 0; right: 0;
    height: 2px;
    background: linear-gradient(90deg, transparent, var(--accent), var(--accent2), transparent);
    animation: shimmer 3s ease-in-out infinite;
  }
  @keyframes shimmer {
    0%,100% { opacity: 0.5; }
    50%      { opacity: 1;   }
  }
  #titlebar:active { cursor: grabbing; }

  .title-left {
    display: flex; align-items: center; gap: 10px;
  }
  .title-icon {
    width: 28px; height: 28px;
    background: linear-gradient(135deg, var(--accent), var(--accent2));
    border-radius: 7px;
    display: grid; place-items: center;
    font-size: 14px;
    box-shadow: 0 0 12px var(--accent-glow);
  }
  .title-text {
    font-size: 17px;
    font-weight: 700;
    letter-spacing: 0.08em;
    text-transform: uppercase;
    background: linear-gradient(90deg, var(--accent), #a78bfa);
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
    background-clip: text;
  }
  .title-sub {
    font-family: var(--font-mono);
    font-size: 10px;
    color: var(--text-dim);
    letter-spacing: 0.05em;
  }

 
  .status-dot {
    width: 8px; height: 8px;
    border-radius: 50%;
    background: var(--accent);
    box-shadow: 0 0 8px var(--accent);
    animation: pulse 2s ease-in-out infinite;
  }
  @keyframes pulse {
    0%,100% { box-shadow: 0 0 4px var(--accent); }
    50%      { box-shadow: 0 0 14px var(--accent); }
  }


  #active-badge {
    font-family: var(--font-mono);
    font-size: 11px;
    color: var(--text-dim);
    padding: 4px 10px;
    background: var(--panel2);
    border-bottom: 1px solid var(--border);
    text-align: right;
    letter-spacing: 0.05em;
    transition: color 0.3s;
  }
  #active-badge span {
    color: var(--accent);
    font-weight: 700;
  }

 
  #mod-list {
    padding: 8px 0 12px;
    max-height: 480px;
    overflow-y: auto;
    scrollbar-width: thin;
    scrollbar-color: var(--border) transparent;
  }
  #mod-list::-webkit-scrollbar { width: 4px; }
  #mod-list::-webkit-scrollbar-track { background: transparent; }
  #mod-list::-webkit-scrollbar-thumb { background: var(--border); border-radius: 4px; }


  .toggle-row {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 13px 18px;
    margin: 2px 10px;
    border-radius: 10px;
    background: transparent;
    border: 1px solid transparent;
    transition: background 0.2s, border-color 0.2s;
    cursor: pointer;
  }
  .toggle-row:hover {
    background: rgba(255,255,255,0.03);
    border-color: var(--border);
  }
  .toggle-row.active {
    background: rgba(0,229,160,0.06);
    border-color: rgba(0,229,160,0.2);
  }

  .mod-info {
    display: flex; flex-direction: column; gap: 2px;
  }
  .mod-name {
    font-size: 15px;
    font-weight: 600;
    letter-spacing: 0.03em;
    color: var(--text);
    transition: color 0.25s;
  }
  .toggle-row.active .mod-name { color: var(--accent); }
  .mod-desc {
    font-family: var(--font-mono);
    font-size: 10px;
    color: var(--text-dim);
    letter-spacing: 0.04em;
  }


  .switch {
    position: relative;
    width: 52px;
    height: 28px;
    flex-shrink: 0;
  }
  .switch input { opacity: 0; width: 0; height: 0; position: absolute; }

  .track {
    position: absolute; inset: 0;
    border-radius: 14px;
    background: var(--off-track);
    border: 1.5px solid rgba(255,255,255,0.07);
    transition: background 0.3s cubic-bezier(0.4,0,0.2,1),
                box-shadow 0.3s cubic-bezier(0.4,0,0.2,1);
    cursor: pointer;
  }
  .switch input:checked + .track {
    background: var(--on-track);
    box-shadow: 0 0 16px rgba(0,229,160,0.4),
                0 0 6px  rgba(0,229,160,0.6) inset;
  }


  .track::after {
    content: '';
    position: absolute;
    top: 3px; left: 3px;
    width: 20px; height: 20px;
    border-radius: 50%;
    background: #fff;
    box-shadow: 0 2px 6px rgba(0,0,0,0.4);
    transition: transform 0.3s cubic-bezier(0.34,1.56,0.64,1),
                box-shadow 0.3s ease;
  }
  .switch input:checked + .track::after {
    transform: translateX(24px);
    box-shadow: 0 2px 8px rgba(0,0,0,0.3),
                0 0 4px var(--accent);
  }


  .track::before {
    content: '';
    position: absolute; inset: -6px;
    border-radius: 50%;
    background: var(--accent);
    opacity: 0;
    transform: scale(0.5);
    transition: opacity 0.15s, transform 0.15s;
    pointer-events: none;
  }
  .switch input:checked + .track::before {
    opacity: 0; /* fires via JS ripple */
  }


  .divider {
    height: 1px;
    background: var(--border);
    margin: 6px 18px;
    opacity: 0.5;
  }


  .category {
    font-family: var(--font-mono);
    font-size: 10px;
    color: var(--accent2);
    letter-spacing: 0.15em;
    text-transform: uppercase;
    padding: 10px 18px 4px;
  }


  #footer {
    padding: 10px 18px;
    background: var(--panel2);
    border-top: 1px solid var(--border);
    display: flex;
    align-items: center;
    justify-content: space-between;
  }
  .footer-text {
    font-family: var(--font-mono);
    font-size: 10px;
    color: var(--text-dim);
    letter-spacing: 0.04em;
  }
  .footer-text em { color: var(--accent); font-style: normal; }

  /* panic button */
  #panic-btn {
    font-family: var(--font-mono);
    font-size: 10px;
    color: var(--danger);
    background: rgba(244,63,94,0.1);
    border: 1px solid rgba(244,63,94,0.3);
    border-radius: 6px;
    padding: 4px 10px;
    cursor: pointer;
    letter-spacing: 0.06em;
    transition: background 0.2s, box-shadow 0.2s;
  }
  #panic-btn:hover {
    background: rgba(244,63,94,0.2);
    box-shadow: 0 0 10px rgba(244,63,94,0.3);
  }


  #toast {
    position: fixed;
    bottom: 30px; left: 50%;
    transform: translateX(-50%) translateY(20px);
    background: var(--panel2);
    border: 1px solid var(--border);
    border-left: 3px solid var(--accent);
    border-radius: 8px;
    padding: 10px 18px;
    font-size: 13px;
    font-weight: 600;
    color: var(--text);
    letter-spacing: 0.04em;
    z-index: 100;
    pointer-events: none;
    opacity: 0;
    transition: opacity 0.25s, transform 0.25s;
    white-space: nowrap;
    box-shadow: 0 4px 20px rgba(0,0,0,0.5);
  }
  #toast.show {
    opacity: 1;
    transform: translateX(-50%) translateY(0);
  }
  #toast.off { border-left-color: var(--text-dim); }
</style>
</head>
<body>


<div id="panel">

  <!-- Title Bar -->
  <div id="titlebar">
    <div class="title-left">
      <div class="title-icon"></div>
      <div>
        <div class="title-text">lgl port</div>
        <div class="title-sub">idkwhattoaddhere// v1.0</div>
      </div>
    </div>
    <div style="display:flex;align-items:center;gap:8px;">
      <div class="status-dot" title="Server connected"></div>
    </div>
  </div>

 
  <div id="active-badge">ACTIVE MODS: <span id="active-count">0</span></div>

  <!-- Toggle List -->
  <div id="mod-list">

    <div class="category">// Movement</div>

    <div class="toggle-row" id="row-fly" onclick="toggle('fly')">
      <div class="mod-info">
        <div class="mod-name">Fly Mode</div>
        <div class="mod-desc">OVERRIDE_GRAVITY // BYPASS_COLLISION</div>
      </div>
      <label class="switch" onclick="event.stopPropagation()">
        <input type="checkbox" id="chk-fly" onchange="toggle('fly')">
        <div class="track"></div>
      </label>
    </div>

    <div class="toggle-row" id="row-noClip" onclick="toggle('noClip')">
      <div class="mod-info">
        <div class="mod-name">No Clip</div>
        <div class="mod-desc">PHYS_COLLIDER // DISABLED</div>
      </div>
      <label class="switch" onclick="event.stopPropagation()">
        <input type="checkbox" id="chk-noClip" onchange="toggle('noClip')">
        <div class="track"></div>
      </label>
    </div>

    <div class="toggle-row" id="row-speedHack" onclick="toggle('speedHack')">
      <div class="mod-info">
        <div class="mod-name">Speed Hack</div>
        <div class="mod-desc">MOVESPEED // 10x MULTIPLIER</div>
      </div>
      <label class="switch" onclick="event.stopPropagation()">
        <input type="checkbox" id="chk-speedHack" onchange="toggle('speedHack')">
        <div class="track"></div>
      </label>
    </div>

    <div class="divider"></div>
    <div class="category">// Combat</div>

    <div class="toggle-row" id="row-infiniteHealth" onclick="toggle('infiniteHealth')">
      <div class="mod-info">
        <div class="mod-name">Infinite Health</div>
        <div class="mod-desc">HP_DEDUCTION // NULLED</div>
      </div>
      <label class="switch" onclick="event.stopPropagation()">
        <input type="checkbox" id="chk-infiniteHealth" onchange="toggle('infiniteHealth')">
        <div class="track"></div>
      </label>
    </div>

    <div class="toggle-row" id="row-godMode" onclick="toggle('godMode')">
      <div class="mod-info">
        <div class="mod-name">God Mode</div>
        <div class="mod-desc">DAMAGE_HOOK // RETURN_ZERO</div>
      </div>
      <label class="switch" onclick="event.stopPropagation()">
        <input type="checkbox" id="chk-godMode" onchange="toggle('godMode')">
        <div class="track"></div>
      </label>
    </div>

    <div class="toggle-row" id="row-unlimitedAmmo" onclick="toggle('unlimitedAmmo')">
      <div class="mod-info">
        <div class="mod-name">Unlimited Ammo</div>
        <div class="mod-desc">AMMO_COUNT // LOCK_MAX</div>
      </div>
      <label class="switch" onclick="event.stopPropagation()">
        <input type="checkbox" id="chk-unlimitedAmmo" onchange="toggle('unlimitedAmmo')">
        <div class="track"></div>
      </label>
    </div>

    <div class="divider"></div>
    <div class="category">// Visuals</div>

    <div class="toggle-row" id="row-esp" onclick="toggle('esp')">
      <div class="mod-info">
        <div class="mod-name">ESP</div>
        <div class="mod-desc">ENTITY_RENDER // WALLHACK_DRAW</div>
      </div>
      <label class="switch" onclick="event.stopPropagation()">
        <input type="checkbox" id="chk-esp" onchange="toggle('esp')">
        <div class="track"></div>
      </label>
    </div>

  </div><!-- list -->

  <!-- Footer -->
  <div id="footer">
    <div class="footer-text">SRV <em>127.0.0.1:8080</em> // INJECTED</div>
    <button id="panic-btn" onclick="panicOff()">PANIC OFF</button>
  </div>

</div><!-- end panel -->

<!-- Toast -->
<div id="toast"></div>

<script>

const state = {
  fly: false, esp: false, infiniteHealth: false,
  speedHack: false, noClip: false, godMode: false, unlimitedAmmo: false
};

const LABELS = {
  fly:           'Fly Mode',
  esp:           'ESP',
  infiniteHealth:'Infinite Health',
  speedHack:     'Speed Hack',
  noClip:        'No Clip',
  godMode:       'God Mode',
  unlimitedAmmo: 'Unlimited Ammo',
};


function toggle(mod) {
  state[mod] = !state[mod];
  const chk = document.getElementById('chk-' + mod);
  const row = document.getElementById('row-' + mod);
  if (chk) chk.checked = state[mod];
  if (row) row.classList.toggle('active', state[mod]);

  updateBadge();
  showToast(mod, state[mod]);
  sendToggle(mod, state[mod]);
}


function sendToggle(mod, enabled) {
  const url = `/toggle?mod=${mod}&state=${enabled ? 1 : 0}`;
  fetch(url, { method: 'GET', cache: 'no-store' })
    .catch(() => {/* server may not respond with body */});
}


function panicOff() {
  Object.keys(state).forEach(mod => {
    if (state[mod]) {
      state[mod] = false;
      const chk = document.getElementById('chk-' + mod);
      const row = document.getElementById('row-' + mod);
      if (chk) chk.checked = false;
      if (row) row.classList.remove('active');
      sendToggle(mod, false);
    }
  });
  updateBadge();
  showToast('ALL', false, true);
}


function updateBadge() {
  const count = Object.values(state).filter(Boolean).length;
  document.getElementById('active-count').textContent = count;
}


let toastTimer;
function showToast(mod, on, panic = false) {
  const el = document.getElementById('toast');
  clearTimeout(toastTimer);
  if (panic) {
    el.textContent = '⚠ ALL MODS DISABLED';
    el.className = 'off';
  } else {
    const label = LABELS[mod] || mod;
    el.textContent = on
      ? `✓ ${label} Enabled`
      : `✗ ${label} Disabled`;
    el.className = on ? 'show' : 'show off';
  }
  el.classList.add('show');
  toastTimer = setTimeout(() => el.classList.remove('show'), 2200);
}


(function initDrag() {
  const panel    = document.getElementById('panel');
  const titlebar = document.getElementById('titlebar');
  let dragging = false, startX, startY, origL, origT;

  titlebar.addEventListener('pointerdown', e => {
    dragging = true;
    titlebar.setPointerCapture(e.pointerId);
    const rect = panel.getBoundingClientRect();
    startX = e.clientX; startY = e.clientY;
    origL  = rect.left;  origT  = rect.top;
    panel.style.transform = 'none';
    panel.style.left = origL + 'px';
    panel.style.top  = origT + 'px';
  });

  titlebar.addEventListener('pointermove', e => {
    if (!dragging) return;
    const dx = e.clientX - startX;
    const dy = e.clientY - startY;
    const nx = Math.max(0, Math.min(window.innerWidth  - panel.offsetWidth,  origL + dx));
    const ny = Math.max(0, Math.min(window.innerHeight - panel.offsetHeight, origT + dy));
    panel.style.left = nx + 'px';
    panel.style.top  = ny + 'px';
  });

  titlebar.addEventListener('pointerup', () => { dragging = false; });
})();


async function pollStatus() {
  try {
    const res  = await fetch('/status', { cache: 'no-store' });
    const data = await res.json();
    Object.keys(data).forEach(mod => {
      if (mod in state && state[mod] !== data[mod]) {
        state[mod] = data[mod];
        const chk = document.getElementById('chk-' + mod);
        const row = document.getElementById('row-' + mod);
        if (chk) chk.checked = data[mod];
        if (row) row.classList.toggle('active', data[mod]);
      }
    });
    updateBadge();
  } catch(_) {}
}
setInterval(pollStatus, 3000);
</script>
</body>
</html>
)HTMLPAGE";
