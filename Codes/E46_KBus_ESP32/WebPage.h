// -----------------------------------------------------------------------------
// WebPage.h - the web interface served by the ESP32
//
// One self-contained page: no external files, so it also works when the phone
// has no internet connection. The buttons are built from the list in Commands.h.
// -----------------------------------------------------------------------------

#ifndef WEB_PAGE_H
#define WEB_PAGE_H

const char WEB_PAGE[] PROGMEM = R"rawliteral(<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<meta name="theme-color" content="#0b1220">
<title>BMW E46</title>
<style>
:root{--bg:#0b1220;--card:#131c2e;--line:#243450;--text:#e6ecf5;--muted:#8a99b3;--accent:#22d3ee;--ok:#34d399;--bad:#fb7185}
*{box-sizing:border-box}
body{margin:0;background:var(--bg);color:var(--text);font:16px/1.4 -apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,Arial,sans-serif;-webkit-tap-highlight-color:transparent}
header{position:sticky;top:0;z-index:2;display:flex;align-items:center;justify-content:space-between;gap:12px;padding:14px 16px;background:rgba(11,18,32,.94);border-bottom:1px solid var(--line)}
h1{margin:0;font-size:18px;letter-spacing:.02em}
h1 span{color:var(--accent)}
.pill{display:flex;align-items:center;gap:8px;padding:6px 12px;border:1px solid var(--line);border-radius:999px;font-size:13px;color:var(--muted);white-space:nowrap}
.dot{width:8px;height:8px;border-radius:50%;background:var(--muted)}
.dot.on{background:var(--ok)}
.dot.off{background:var(--bad)}
main{max-width:760px;margin:0 auto;padding:16px}
section{background:var(--card);border:1px solid var(--line);border-radius:14px;padding:14px;margin-bottom:14px}
h2{margin:0 0 12px;font-size:12px;letter-spacing:.1em;text-transform:uppercase;color:var(--muted)}
.grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(150px,1fr));gap:10px}
button{min-height:48px;padding:10px 12px;border:1px solid var(--line);border-radius:10px;background:#1a2640;color:var(--text);font:inherit;font-size:15px;cursor:pointer}
button:active{background:var(--accent);border-color:var(--accent);color:#06222a}
button:disabled{opacity:.5}
button.primary{background:var(--accent);border-color:var(--accent);color:#06222a;font-weight:600}
.row{display:flex;align-items:center;justify-content:space-between;gap:12px;padding:8px 0}
.row+.row{border-top:1px solid var(--line)}
.row small{display:block;color:var(--muted);font-size:13px}
input[type=number]{width:96px;min-height:44px;padding:8px 10px;border:1px solid var(--line);border-radius:10px;background:var(--bg);color:var(--text);font:inherit;text-align:right}
input[type=checkbox]{width:24px;height:24px;accent-color:var(--accent)}
.actions{display:flex;flex-wrap:wrap;gap:10px;margin-top:12px}
.actions button{flex:1 1 140px}
pre{margin:0;max-height:260px;overflow:auto;font:13px/1.5 ui-monospace,SFMono-Regular,Menlo,Consolas,monospace;color:#c7d2e0;white-space:pre-wrap}
#toast{position:fixed;left:50%;bottom:20px;transform:translateX(-50%);padding:10px 16px;border-radius:10px;background:var(--ok);color:#06281c;font-weight:600;opacity:0;transition:opacity .2s;pointer-events:none}
#toast.bad{background:var(--bad);color:#3b0712}
#toast.show{opacity:1}
</style>
</head>
<body>
<header>
  <h1>BMW <span>E46</span></h1>
  <div class="pill"><span class="dot" id="dot"></span><span id="status">Connecting</span></div>
</header>
<main>
  <div id="commands"></div>

  <section>
    <h2>Settings</h2>
    <div class="row"><span>Goes to sleep in</span><strong id="sleepIn">-</strong></div>
    <div class="row">
      <label for="keyFob">Key fob light functions<small>Welcome lights, goodbye lights, follow-me-home</small></label>
      <input type="checkbox" id="keyFob">
    </div>
    <div class="row">
      <label for="sleepAfter">Sleep after bus silence<small>Seconds without bus traffic</small></label>
      <input type="number" id="sleepAfter" min="10" max="3600" inputmode="numeric">
    </div>
    <div class="row">
      <label for="webAwake">Stay awake after web use<small>Seconds after the last action on this page</small></label>
      <input type="number" id="webAwake" min="30" max="3600" inputmode="numeric">
    </div>
    <div class="actions">
      <button class="primary" id="save">Save settings</button>
      <button id="awake">Stay awake</button>
      <button id="sleep">Sleep now</button>
    </div>
  </section>

  <section>
    <h2>Bus monitor</h2>
    <pre id="log">No messages yet</pre>
  </section>
</main>
<div id="toast"></div>
<script>
const $ = id => document.getElementById(id);
let settingsLoaded = false, toastTimer;

async function api(path, options) {
  const response = await fetch(path, options);
  if (!response.ok) throw new Error(response.status);
  return response.json();
}

function toast(text, bad) {
  const t = $('toast');
  t.textContent = text;
  t.className = 'show' + (bad ? ' bad' : '');
  clearTimeout(toastTimer);
  toastTimer = setTimeout(() => t.className = '', 1500);
}

function time(seconds) {
  return Math.floor(seconds / 60) + ':' + String(seconds % 60).padStart(2, '0');
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

async function loadCommands() {
  const groups = {};
  (await api('/api/commands')).forEach(c => (groups[c.group] = groups[c.group] || []).push(c));
  for (const name in groups) {
    const section = document.createElement('section');
    const title = document.createElement('h2');
    const grid = document.createElement('div');
    title.textContent = name;
    grid.className = 'grid';
    groups[name].forEach(c => {
      const button = document.createElement('button');
      button.textContent = c.name;
      button.onclick = () => post('/api/cmd?id=' + c.id, 'Sent');
      grid.appendChild(button);
    });
    section.append(title, grid);
    $('commands').appendChild(section);
  }
}

async function refresh() {
  try {
    const s = await api('/api/state');
    $('dot').className = 'dot ' + (s.busIdle < 5 ? 'on' : '');
    $('status').textContent = s.busIdle < 5 ? 'Bus active' : 'Bus idle';
    $('sleepIn').textContent = time(s.sleepIn);
    $('log').textContent = s.log.length ? s.log.join('\n') : 'No messages yet';
    if (!settingsLoaded) {
      $('keyFob').checked = s.keyFob;
      $('sleepAfter').value = s.sleepAfter;
      $('webAwake').value = s.webAwake;
      settingsLoaded = true;
    }
  } catch (e) {
    $('dot').className = 'dot off';
    $('status').textContent = 'Not connected';
  }
}

$('save').onclick = () => {
  settingsLoaded = false;
  post('/api/settings?keyFob=' + ($('keyFob').checked ? 1 : 0) + '&sleepAfter=' + $('sleepAfter').value + '&webAwake=' + $('webAwake').value, 'Saved');
};
$('awake').onclick = () => post('/api/awake', 'Staying awake');
$('sleep').onclick = () => post('/api/sleep', 'Going to sleep');

loadCommands().catch(() => toast('Could not load the commands', true));
refresh();
setInterval(refresh, 2000);
</script>
</body>
</html>
)rawliteral";

#endif
