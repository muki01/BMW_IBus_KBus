// -----------------------------------------------------------------------------
// WebPage.h - the web interface served by the ESP32
//
// One self-contained page: no external files, so it also works when the phone
// has no internet connection. It has three tabs - Control, Monitor and
// Settings - and builds the control buttons from the list in Commands.h.
// -----------------------------------------------------------------------------

#ifndef WEB_PAGE_H
#define WEB_PAGE_H

const char WEB_PAGE[] PROGMEM = R"rawliteral(<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<meta name="theme-color" content="#0b1220">
<meta name="color-scheme" content="dark">
<title>BMW E46</title>
<style>
:root{--bg:#0b1220;--card:#121b2d;--raised:#18233a;--line:#22314c;--text:#e8eef7;--muted:#8a99b3;--accent:#22d3ee;--accent-ink:#06222a;--ok:#34d399;--bad:#fb7185}
*{box-sizing:border-box;-webkit-tap-highlight-color:transparent}
html{background:var(--bg)}
body{margin:0;color:var(--text);font:16px/1.4 -apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,Arial,sans-serif;padding-bottom:calc(76px + env(safe-area-inset-bottom))}
button{font:inherit;color:inherit;cursor:pointer}
svg.i{width:22px;height:22px;flex:none;fill:none;stroke:currentColor;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round}

/* top bar */
.top{position:sticky;top:0;z-index:5;display:flex;align-items:center;justify-content:space-between;gap:12px;padding:calc(12px + env(safe-area-inset-top)) 16px 12px;background:rgba(11,18,32,.92);backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px);border-bottom:1px solid var(--line)}
.brand{display:flex;align-items:center;gap:10px;min-width:0}
.logo{display:grid;place-items:center;width:38px;height:38px;border-radius:11px;background:linear-gradient(135deg,#67e8f9,#06b6d4);color:var(--accent-ink)}
.brand b{display:block;font-size:17px;letter-spacing:.01em}
.brand b span{color:var(--accent)}
.brand small{display:block;color:var(--muted);font-size:12px}
.pill{display:flex;align-items:center;gap:7px;padding:7px 12px;border:1px solid var(--line);border-radius:999px;background:var(--card);font-size:13px;color:var(--muted);white-space:nowrap}
.dot{width:8px;height:8px;border-radius:50%;background:var(--muted)}
.pill.on .dot{background:var(--ok);box-shadow:0 0 0 4px rgba(52,211,153,.18)}
.pill.on{color:var(--text)}
.pill.off .dot{background:var(--bad)}

/* layout */
main{max-width:640px;margin:0 auto;padding:14px 16px}
.card{background:var(--card);border:1px solid var(--line);border-radius:16px;padding:14px;margin-bottom:12px}
.card h2{display:flex;align-items:center;gap:8px;margin:0 0 12px;font-size:15px;font-weight:600}
.card h2 svg{color:var(--accent)}
.card h2 small{margin-left:auto;color:var(--muted);font-size:12px;font-weight:400}
.notice{display:flex;gap:10px;border-color:rgba(251,113,133,.5);color:#fecdd3;font-size:14px}
.notice svg{color:var(--bad)}

/* sleep card */
.sleep{display:flex;align-items:center;justify-content:space-between;gap:12px}
.sleep small{display:block;color:var(--muted);font-size:13px}
.sleep strong{font-size:30px;font-variant-numeric:tabular-nums;letter-spacing:.01em}
.bar{height:6px;margin-top:12px;border-radius:3px;background:var(--raised);overflow:hidden}
.bar i{display:block;height:100%;width:0;border-radius:3px;background:linear-gradient(90deg,#06b6d4,#67e8f9);transition:width .4s}

/* buttons */
.btn{display:inline-flex;align-items:center;justify-content:center;gap:8px;min-height:44px;padding:10px 16px;border:1px solid var(--line);border-radius:12px;background:var(--raised);font-size:15px}
.btn:active{background:#22304c}
.btn.primary{border-color:var(--accent);background:var(--accent);color:var(--accent-ink);font-weight:600}
.btn.danger{border-color:rgba(251,113,133,.5);color:#fda4af}
.btn.wide{width:100%}

/* category chips */
.chips{display:flex;gap:8px;margin:2px -16px 12px;padding:0 16px;overflow-x:auto;scrollbar-width:none}
.chips::-webkit-scrollbar{display:none}
.chip{flex:none;padding:8px 14px;border:1px solid var(--line);border-radius:999px;background:var(--card);color:var(--muted);font-size:14px}
.chip.on{border-color:var(--accent);background:rgba(34,211,238,.12);color:var(--accent)}

/* command tiles */
.grid{display:grid;grid-template-columns:1fr 1fr;gap:10px}
.cmd{position:relative;display:flex;align-items:center;min-height:54px;padding:10px 12px;border:1px solid var(--line);border-radius:12px;background:var(--raised);font-size:14.5px;line-height:1.25;text-align:left;transition:border-color .15s,background .15s,transform .05s}
.cmd:active{transform:scale(.98)}
.cmd.busy{opacity:.6}
.cmd.ok{border-color:var(--accent);background:rgba(34,211,238,.14)}
.cmd.err{border-color:var(--bad);background:rgba(251,113,133,.12)}

/* monitor */
.log{margin:0;padding:0;list-style:none}
.log li{display:flex;align-items:center;gap:10px;padding:9px 0;border-top:1px solid var(--line)}
.log li:first-child{border-top:0}
.route{flex:none;width:92px;font-size:12px;color:var(--muted)}
.route b{color:var(--accent);font-weight:600}
.hex{font:13px/1.4 ui-monospace,SFMono-Regular,Menlo,Consolas,monospace;color:#cbd5e1;word-break:break-word}
.empty{padding:18px 0;color:var(--muted);text-align:center;font-size:14px}

/* settings */
.row{display:flex;align-items:center;justify-content:space-between;gap:12px;padding:12px 0;border-top:1px solid var(--line)}
.row:first-of-type{border-top:0;padding-top:0}
.row:last-child{padding-bottom:0}
.row small{display:block;color:var(--muted);font-size:13px}
.row output{color:var(--muted);font-variant-numeric:tabular-nums}
.field{display:flex;align-items:center;gap:6px;color:var(--muted);font-size:14px}
input[type=number]{width:84px;min-height:44px;padding:8px 10px;border:1px solid var(--line);border-radius:10px;background:var(--bg);color:var(--text);font:inherit;text-align:right}
input[type=number]:focus{outline:2px solid var(--accent);outline-offset:1px}
.switch{position:relative;flex:none;width:52px;height:32px}
.switch input{position:absolute;inset:0;margin:0;opacity:0}
.switch span{position:absolute;inset:0;border-radius:16px;background:var(--raised);border:1px solid var(--line);transition:background .2s}
.switch span::after{content:"";position:absolute;top:3px;left:3px;width:24px;height:24px;border-radius:50%;background:var(--muted);transition:transform .2s,background .2s}
.switch input:checked+span{background:rgba(34,211,238,.25);border-color:var(--accent)}
.switch input:checked+span::after{transform:translateX(20px);background:var(--accent)}

/* bottom navigation */
.tabs{position:fixed;left:0;right:0;bottom:0;z-index:5;display:flex;justify-content:center;padding:6px 8px calc(6px + env(safe-area-inset-bottom));background:rgba(11,18,32,.94);backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px);border-top:1px solid var(--line)}
.tabs button{flex:1;max-width:160px;display:flex;flex-direction:column;align-items:center;gap:3px;padding:7px 4px;border:0;border-radius:12px;background:none;color:var(--muted);font-size:12px}
.tabs button.on{color:var(--accent)}
.tabs button.on svg{filter:drop-shadow(0 0 6px rgba(34,211,238,.5))}

#toast{position:fixed;left:50%;bottom:calc(88px + env(safe-area-inset-bottom));z-index:6;transform:translateX(-50%);padding:10px 16px;border-radius:12px;background:var(--ok);color:#06281c;font-size:14px;font-weight:600;opacity:0;transition:opacity .2s;pointer-events:none;white-space:nowrap}
#toast.bad{background:var(--bad);color:#3b0712}
#toast.show{opacity:1}
[hidden]{display:none!important}
</style>
</head>
<body>
<svg width="0" height="0" style="position:absolute" aria-hidden="true">
  <symbol id="i-car" viewBox="0 0 24 24"><path d="M5 16h14M5 16l1.6-5.2A2.5 2.5 0 0 1 9 9h6a2.5 2.5 0 0 1 2.4 1.8L19 16M5 16v2.5M19 16v2.5M8 13.5h.01M16 13.5h.01"/></symbol>
  <symbol id="i-control" viewBox="0 0 24 24"><rect x="4" y="4" width="7" height="7" rx="2"/><rect x="13" y="4" width="7" height="7" rx="2"/><rect x="4" y="13" width="7" height="7" rx="2"/><rect x="13" y="13" width="7" height="7" rx="2"/></symbol>
  <symbol id="i-monitor" viewBox="0 0 24 24"><path d="M3 12h4l2.5-7 5 14 2.5-7h4"/></symbol>
  <symbol id="i-settings" viewBox="0 0 24 24"><path d="M4 7h9M17 7h3M4 17h3M11 17h9"/><circle cx="15" cy="7" r="2"/><circle cx="9" cy="17" r="2"/></symbol>
  <symbol id="i-lights" viewBox="0 0 24 24"><path d="M9 18h6M10 21h4M12 3a6 6 0 0 0-3.5 10.9c.6.5 1 1.2 1 2.1h5c0-.9.4-1.6 1-2.1A6 6 0 0 0 12 3z"/></symbol>
  <symbol id="i-locks" viewBox="0 0 24 24"><rect x="5" y="11" width="14" height="9" rx="2"/><path d="M8 11V8a4 4 0 0 1 8 0v3"/></symbol>
  <symbol id="i-windows" viewBox="0 0 24 24"><path d="M4 18V11a7 7 0 0 1 7-7h9v14zM4 12h16"/></symbol>
  <symbol id="i-interior" viewBox="0 0 24 24"><path d="M5 13a7 7 0 0 1 14 0zM12 17v3M7 16.5l-1.2 2M17 16.5l1.2 2"/></symbol>
  <symbol id="i-wipers" viewBox="0 0 24 24"><path d="M3 16a13 13 0 0 1 18 0M12 20 7.5 8.5"/></symbol>
  <symbol id="i-group" viewBox="0 0 24 24"><circle cx="12" cy="12" r="8"/><path d="M12 8v8M8 12h8"/></symbol>
  <symbol id="i-moon" viewBox="0 0 24 24"><path d="M20 14.5A8 8 0 1 1 9.5 4a6.5 6.5 0 0 0 10.5 10.5z"/></symbol>
  <symbol id="i-alert" viewBox="0 0 24 24"><path d="M12 4 3 19h18zM12 10v4M12 16.5h.01"/></symbol>
</svg>

<header class="top">
  <div class="brand">
    <div class="logo"><svg class="i"><use href="#i-car"/></svg></div>
    <div><b>BMW <span>E46</span></b><small>K-Bus control</small></div>
  </div>
  <div class="pill" id="pill"><i class="dot"></i><span id="status">Connecting</span></div>
</header>

<main>
  <div class="card notice" id="offline" hidden>
    <svg class="i"><use href="#i-alert"/></svg>
    <div>No connection to the ESP32. It may be asleep: wake the car up and reconnect to the Wi-Fi network.</div>
  </div>

  <section id="tab-control">
    <div class="card">
      <div class="sleep">
        <div><small>Goes to sleep in</small><strong id="sleepIn">-:--</strong></div>
        <button class="btn" id="awake"><svg class="i"><use href="#i-moon"/></svg>Stay awake</button>
      </div>
      <div class="bar"><i id="sleepBar"></i></div>
    </div>
    <div class="chips" id="chips"></div>
    <div id="groups"></div>
  </section>

  <section id="tab-monitor" hidden>
    <div class="card">
      <h2><svg class="i"><use href="#i-monitor"/></svg>Bus monitor<small id="logCount"></small></h2>
      <ul class="log" id="log"></ul>
      <button class="btn wide" id="pause" style="margin-top:12px">Pause</button>
    </div>
  </section>

  <section id="tab-settings" hidden>
    <div class="card">
      <h2><svg class="i"><use href="#i-lights"/></svg>Key fob</h2>
      <div class="row">
        <label for="keyFob">Light functions<small>Welcome lights, goodbye lights, follow-me-home</small></label>
        <span class="switch"><input type="checkbox" id="keyFob"><span></span></span>
      </div>
    </div>
    <div class="card">
      <h2><svg class="i"><use href="#i-moon"/></svg>Sleep</h2>
      <div class="row">
        <label for="sleepAfter">After bus silence<small>Time without bus traffic</small></label>
        <span class="field"><input type="number" id="sleepAfter" min="10" max="3600" inputmode="numeric">s</span>
      </div>
      <div class="row">
        <label for="webAwake">After web use<small>Time after the last action here</small></label>
        <span class="field"><input type="number" id="webAwake" min="30" max="3600" inputmode="numeric">s</span>
      </div>
    </div>
    <button class="btn primary wide" id="save">Save settings</button>
    <div class="card" style="margin-top:12px">
      <h2><svg class="i"><use href="#i-car"/></svg>Device</h2>
      <div class="row"><span>Address</span><output id="address"></output></div>
      <div class="row"><span>Running for</span><output id="uptime">-</output></div>
      <div class="row"><span>Messages in monitor</span><output id="messages">-</output></div>
    </div>
    <button class="btn danger wide" id="sleep"><svg class="i"><use href="#i-moon"/></svg>Sleep now</button>
  </section>
</main>

<nav class="tabs">
  <button data-tab="control"><svg class="i"><use href="#i-control"/></svg>Control</button>
  <button data-tab="monitor"><svg class="i"><use href="#i-monitor"/></svg>Monitor</button>
  <button data-tab="settings"><svg class="i"><use href="#i-settings"/></svg>Settings</button>
</nav>
<div id="toast"></div>

<script>
const $ = id => document.getElementById(id);
const TABS = ['control', 'monitor', 'settings'];
const MODULES = {'00':'GM','08':'SHD','18':'CDC','3B':'GT','3F':'DIA','44':'EWS','50':'MFL','5B':'IHKA','60':'PDC','68':'RAD','6A':'DSP','80':'IKE','BF':'ALL','C0':'MID','C8':'TEL','D0':'LCM','E8':'RLS','ED':'VID','F0':'BMBT','FF':'ALL'};
let settingsLoaded = false, paused = false, failures = 0, toastTimer;

async function api(path, options) {
  const response = await fetch(path, options);
  if (!response.ok) throw new Error(response.status);
  return response.json();
}

function icon(name) {
  return '<svg class="i"><use href="#i-' + name + '"/></svg>';
}

function toast(text, bad) {
  const t = $('toast');
  t.textContent = text;
  t.className = 'show' + (bad ? ' bad' : '');
  clearTimeout(toastTimer);
  toastTimer = setTimeout(() => t.className = '', 1600);
}

function clock(seconds) {
  const h = Math.floor(seconds / 3600), m = Math.floor(seconds % 3600 / 60), s = seconds % 60;
  return (h ? h + ':' + String(m).padStart(2, '0') : m) + ':' + String(s).padStart(2, '0');
}

function showTab(name) {
  TABS.forEach(tab => $('tab-' + tab).hidden = tab !== name);
  document.querySelectorAll('.tabs button').forEach(b => b.classList.toggle('on', b.dataset.tab === name));
  history.replaceState(null, '', '#' + name);
  window.scrollTo(0, 0);
}

function showGroup(name) {
  document.querySelectorAll('#groups .card').forEach(card => card.hidden = name && card.dataset.group !== name);
  document.querySelectorAll('.chip').forEach(chip => chip.classList.toggle('on', chip.dataset.group === name));
}

async function send(button, id) {
  button.classList.add('busy');
  try {
    await api('/api/cmd?id=' + id, {method: 'POST'});
    button.classList.add('ok');
    if (navigator.vibrate) navigator.vibrate(15);
  } catch (e) {
    button.classList.add('err');
    toast('Not sent', true);
  }
  button.classList.remove('busy');
  setTimeout(() => button.classList.remove('ok', 'err'), 700);
  refresh();
}

async function post(path, done) {
  try {
    await api(path, {method: 'POST'});
    toast(done);
    refresh();
  } catch (e) {
    toast('Not sent', true);
  }
}

async function loadCommands(startGroup) {
  const groups = {};
  (await api('/api/commands')).forEach(c => (groups[c.group] = groups[c.group] || []).push(c));

  const addChip = (label, group) => {
    const chip = document.createElement('button');
    chip.className = 'chip';
    chip.dataset.group = group;
    chip.textContent = label;
    chip.onclick = () => showGroup(group);
    $('chips').appendChild(chip);
  };
  addChip('All', '');

  for (const name in groups) {
    addChip(name, name);
    const known = document.getElementById('i-' + name.toLowerCase());
    const card = document.createElement('div');
    card.className = 'card';
    card.dataset.group = name;
    card.innerHTML = '<h2>' + icon(known ? name.toLowerCase() : 'group') + '<span></span></h2><div class="grid"></div>';
    card.querySelector('span').textContent = name;
    groups[name].forEach(c => {
      const button = document.createElement('button');
      button.className = 'cmd';
      button.textContent = c.name;
      button.onclick = () => send(button, c.id);
      card.querySelector('.grid').appendChild(button);
    });
    $('groups').appendChild(card);
  }
  showGroup(groups[startGroup] ? startGroup : '');
}

function showLog(log) {
  $('logCount').textContent = log.length ? 'newest first' : '';
  $('messages').textContent = log.length;
  if (paused) return;
  const list = $('log');
  list.textContent = '';
  if (!log.length) {
    list.innerHTML = '<li class="empty">No messages yet</li>';
    return;
  }
  log.forEach(line => {
    const bytes = line.split(' ');
    const item = document.createElement('li');
    const route = document.createElement('span');
    const hex = document.createElement('span');
    route.className = 'route';
    route.innerHTML = '<b></b> &rarr; <b></b>';
    route.firstChild.textContent = MODULES[bytes[0]] || bytes[0];
    route.lastChild.textContent = MODULES[bytes[2]] || bytes[2];
    hex.className = 'hex';
    hex.textContent = line;
    item.append(route, hex);
    list.appendChild(item);
  });
}

async function refresh() {
  try {
    const s = await api('/api/state');
    failures = 0;
    const active = s.busIdle < 5;
    $('pill').className = 'pill' + (active ? ' on' : '');
    $('status').textContent = active ? 'Bus active' : 'Bus idle';
    $('offline').hidden = true;
    $('sleepIn').textContent = clock(s.sleepIn);
    $('sleepBar').style.width = Math.min(100, 100 * s.sleepIn / Math.max(s.sleepAfter, s.webAwake)) + '%';
    $('uptime').textContent = clock(s.uptime);
    showLog(s.log);
    if (!settingsLoaded) {
      $('keyFob').checked = s.keyFob;
      $('sleepAfter').value = s.sleepAfter;
      $('webAwake').value = s.webAwake;
      settingsLoaded = true;
    }
  } catch (e) {
    if (++failures < 2) return;
    $('pill').className = 'pill off';
    $('status').textContent = 'Offline';
    $('offline').hidden = false;
  }
}

document.querySelectorAll('.tabs button').forEach(b => b.onclick = () => showTab(b.dataset.tab));
$('awake').onclick = () => post('/api/awake', 'Staying awake');
$('pause').onclick = () => {
  paused = !paused;
  $('pause').textContent = paused ? 'Resume' : 'Pause';
};
$('save').onclick = () => {
  settingsLoaded = false;
  post('/api/settings?keyFob=' + ($('keyFob').checked ? 1 : 0) + '&sleepAfter=' + $('sleepAfter').value + '&webAwake=' + $('webAwake').value, 'Settings saved');
};
$('sleep').onclick = () => {
  if (confirm('Put the ESP32 to sleep now?')) post('/api/sleep', 'Going to sleep');
};

const [startTab, startGroup] = decodeURIComponent(location.hash.slice(1)).split(':');
$('address').textContent = location.host;
showTab(TABS.includes(startTab) ? startTab : 'control');
loadCommands(startGroup).catch(() => toast('Could not load the commands', true));
refresh();
setInterval(refresh, 2000);
</script>
</body>
</html>
)rawliteral";

#endif
