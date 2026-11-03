#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

// SKB adds routes to the existing Bruce web server; Bruce's own routes and
// functionality remain registered unchanged.
static const char SKB_DASHBOARD_HTML[] PROGMEM = R"SKBHTML(
<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<meta name="theme-color" content="#0b0e13"><title>SKB | Headless Pro</title>
<style>
:root{color-scheme:dark;--metal:#c5d0d8;--ice:#8ccdf6;--line:#465461;--muted:#96a6b5}
*{box-sizing:border-box}body{margin:0;background:radial-gradient(ellipse at 50% -10%,#2d3946 0,#10151b 43%,#06080a 100%);color:#e7edf2;font:15px system-ui,-apple-system,Arial,sans-serif;min-height:100vh}
body:before{content:"SKB  ♛  SKB  ♛  SKB";position:fixed;inset:18% 0 auto;text-align:center;font-size:clamp(32px,9vw,110px);font-weight:900;letter-spacing:.15em;color:#ffffff06;pointer-events:none}
.shell{max-width:960px;margin:0 auto;padding:22px 15px 50px;position:relative}
header{text-align:center;padding:25px 12px 21px;border:1px solid #566371;border-radius:18px;background:linear-gradient(135deg,#28333d,#0a0e13 57%,#1b2732);box-shadow:inset 0 1px #a7b6c35c,0 15px 38px #0008}
.crown{font-size:26px;color:var(--metal)}h1{font-size:clamp(42px,9vw,72px);margin:0;font-weight:900;letter-spacing:.22em;line-height:1.2;color:#e0e8ee;text-shadow:0 2px 0 #222c35,0 4px 12px #8eacbe7a}
header p{margin:3px 0 0;letter-spacing:.24em;font-size:11px;color:#b0c0cc}.sub{margin:14px auto 0;color:var(--ice);font-size:12px;letter-spacing:.17em}
.grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:15px;margin-top:18px}
.panel{border:1px solid var(--line);background:linear-gradient(145deg,#202a34ed,#0c1117f5);border-radius:14px;padding:18px;box-shadow:inset 0 1px #ffffff20,0 8px 18px #0007}
h2{font-size:15px;letter-spacing:.12em;color:#d9e4ec;margin:0 0 13px;text-transform:uppercase}
p{line-height:1.5}.muted{color:var(--muted);font-size:13px}.status{display:grid;grid-template-columns:1fr 1fr;gap:12px}
.metric{padding:10px;background:#080d13;border:1px solid #2d3945;border-radius:8px}.metric small{display:block;color:var(--muted);font-size:11px}.metric strong{display:block;color:var(--ice);font-size:16px;margin-top:3px;overflow-wrap:anywhere}
.controls{display:grid;grid-template-columns:repeat(3,1fr);gap:8px}
button,.link{appearance:none;display:inline-flex;align-items:center;justify-content:center;min-height:43px;padding:9px 11px;color:#deebf4;text-decoration:none;border-radius:8px;border:1px solid #718393;background:linear-gradient(#394958,#141d26);font:600 13px system-ui;cursor:pointer;box-shadow:inset 0 1px #cbd7e133}
button:active,.link:active{transform:translateY(1px)}button:focus-visible,.link:focus-visible{outline:2px solid var(--ice)}
.wide{grid-column:span 3}.links{display:flex;flex-wrap:wrap;gap:8px}.feedback{min-height:21px;color:var(--ice);font-size:12px}.console{margin-top:16px}.screenframe{width:100%;height:610px;max-height:75vh;border:1px solid #667785;background:#090d12;border-radius:8px}.help{font-size:12px;color:#b3c3d0}.live-menu{background:#070e15;border:1px solid #53708a;border-radius:10px;padding:13px;margin:12px 0}.live-menu h3{margin:0 0 9px;font-size:13px;color:#8ccdf6}.live-item{padding:8px 10px;border-radius:5px;color:#c8d8e4;font-size:13px}.live-item.active{background:#334a60;color:white;border:1px solid #8ccdf6}.live-item{display:block;width:100%;margin:3px 0;text-align:left;min-height:36px}.live-item:disabled{opacity:.5}.live-item:hover{border-color:#8ccdf6}.live-meta{font-size:11px;color:#98acbf}.screenframe:focus-visible{outline:2px solid var(--ice)}
footer{margin-top:19px;text-align:center;color:#8b9aa7;font-size:12px}
@media(max-width:650px){.grid{grid-template-columns:1fr}.shell{padding:12px}.panel{padding:15px}}
</style>
</head>
<body><div class="shell">
<header><div class="crown">♛</div><h1>SKB</h1><p>MADE BY CHRIS NASH</p><div class="sub">HEADLESS PRO · BRUCE CORE</div></header>
<div class="grid">
<section class="panel"><h2>01 / System</h2><div class="status">
<div class="metric"><small>Connection</small><strong id="conn">Checking</strong></div>
<div class="metric"><small>Uptime</small><strong id="uptime">—</strong></div>
<div class="metric"><small>Free heap</small><strong id="heap">—</strong></div>
<div class="metric"><small>Connected clients</small><strong id="clients">—</strong></div>
</div><p class="muted">Access point: SKB-Headless-Pro<br>Address: 192.168.4.1</p><div id="feedback" class="feedback" role="status" aria-live="polite"></div></section>
<section class="panel"><h2>02 / Remote navigation</h2>
<p class="muted">Tap any live menu item to execute the real firmware action. The selected option and running activity come from the device.</p>
<div class="live-menu" aria-live="polite"><h3 id="menu-title">DEVICE MENU — CONNECTING</h3><div id="menu-options">Waiting for device state…</div><p id="menu-meta" class="live-meta">Tap a menu item to launch it; the selection comes from the actual device.</p></div>
<p class="live-meta">Device activity: <strong id="device-action">Connecting…</strong></p>
<div class="controls">
<button data-nav="esc">BACK</button><button data-nav="up">▲ UP</button><button data-nav="sel">SELECT</button>
<button data-nav="prev">◀ PREV</button><button data-nav="down">▼ DOWN</button><button data-nav="next">NEXT ▶</button>
</div></section>
<section class="panel"><h2>03 / Bruce modules</h2><p class="muted">Wi-Fi · BLE · CC1101 RF · NRF24 · Infrared · Files · Scripts</p>
<p>Use the Bruce interface for installed and supported modules. External CC1101/NRF24 hardware is required for those radio features.</p>
<div class="links"><a class="link" href="#live-console">Open live control console</a><a class="link" href="/bruce" target="_blank" rel="noopener">Full-screen Bruce controls</a><a class="link" href="/skb/api/status">Status API</a></div>
</section>
<section class="panel"><h2>04 / Management</h2>
<p class="muted">Bruce authentication protects this dashboard and its controls. Keep your device and AP password private.</p>
<div class="links"><a class="link" href="/logout">Sign out</a><button id="refresh">Refresh status</button></div>
<p class="muted">A restart and advanced Wi-Fi operations may disconnect your phone; they remain available through Bruce's native controls.</p>
</section></div>
<section class="panel console" id="live-console"><h2>05 / Live virtual screen &amp; controls</h2>
<p class="help">This embeds Bruce's real web-based screen emulator inside SKB. It shows Bruce's current menu and provides its existing controls. Use the navigation buttons above or the controls inside the console.</p>
<iframe class="screenframe" title="Bruce live virtual screen and controls" src="/bruce" loading="eager" referrerpolicy="same-origin"></iframe>
<p class="help">This view requires firmware screen-logging support. Radio operations that stop the Wi-Fi AP may disconnect your phone and require a reboot before reconnecting.</p>
</section>
<footer>SKB HEADLESS PRO · STYLE WITHOUT COMPROMISE</footer></div>
<script>
(function(){
const $=id=>document.getElementById(id);
const say=t=>$('feedback').textContent=t;
let selectionBusy=false;
async function menuState(){
try{
const r=await fetch('/skb/api/menu',{credentials:'same-origin',cache:'no-store'});
if(!r.ok)throw Error('HTTP '+r.status);
const m=await r.json();
$('menu-title').textContent=m.title||'DEVICE MENU';
const root=$('menu-options');root.replaceChildren();
if(!Array.isArray(m.items)||m.items.length===0){root.textContent='Menu not available';return;}
m.items.forEach((name,i)=>{
const row=document.createElement('button');
row.type='button';row.className='live-item'+(i===m.selected?' active':'');
row.textContent=(i===m.selected?'▶ ':'  ')+name;
row.disabled=selectionBusy;
row.addEventListener('click',()=>launchItem(i,m.revision,name));
root.appendChild(row);
});
$('device-action').textContent=m.action?(m.running?'Running: ':'Last action: ')+m.action:'Ready';
$('menu-meta').textContent='Selected '+(m.selected+1)+' of '+m.items.length+' · updated '+Math.max(0,Math.floor((m.updated_ms||0)/1000))+'s after boot';
}catch(e){$('menu-meta').textContent='Live menu unavailable: '+e.message;}
}
async function launchItem(index,revision,label){
if(selectionBusy)return;
selectionBusy=true;say('Requesting '+label+'…');
try{
const body=new URLSearchParams({index:String(index),revision:String(revision)});
const response=await fetch('/skb/api/select',{method:'POST',credentials:'same-origin',
headers:{'Content-Type':'application/x-www-form-urlencoded'},body});
if(!response.ok){const err=await response.json().catch(()=>({error:'HTTP '+response.status}));throw Error(err.error||'HTTP '+response.status);}
say('Selection queued: '+label+'. Waiting for device state…');
setTimeout(menuState,400);
}catch(e){say('Selection rejected: '+e.message);}
finally{selectionBusy=false;}
}
async function status(){
try{const r=await fetch('/skb/api/status',{credentials:'same-origin',cache:'no-store'});if(!r.ok)throw Error('HTTP '+r.status);
const d=await r.json();$('conn').textContent=d.build||'ONLINE';$('uptime').textContent=Math.floor(d.uptime_ms/60000)+' min';$('heap').textContent=Math.round(d.free_heap/1024)+' KB';$('clients').textContent=String(d.ap_clients);say('Connected to SKB firmware');}
catch(e){$('conn').textContent='OFFLINE';say('Status unavailable: '+e.message);}
}
document.querySelectorAll('[data-nav]').forEach(b=>b.addEventListener('click',async()=>{
const cmd=b.getAttribute('data-nav');say('Sending '+cmd+'…');
try{const body=new URLSearchParams({cmnd:'nav '+cmd});const r=await fetch('/cm',{method:'POST',credentials:'same-origin',headers:{'Content-Type':'application/x-www-form-urlencoded'},body});if(!r.ok)throw Error('HTTP '+r.status);say('Sent: '+cmd);setTimeout(menuState,350);}
catch(e){say('Command failed: '+e.message);}
}));
$('refresh').addEventListener('click',status);status();menuState();setInterval(status,15000);setInterval(menuState,900);
})();
</script></body></html>
)SKBHTML";

inline void registerSkbDashboardRoutes(AsyncWebServer *web, bool (*authorize)(AsyncWebServerRequest *, bool)) {
    web->on("/skb", HTTP_GET, [authorize](AsyncWebServerRequest *request) {
        if (!authorize(request, true)) return;
        request->send_P(200, "text/html", SKB_DASHBOARD_HTML);
    });
    web->on("/skb/api/status", HTTP_GET, [authorize](AsyncWebServerRequest *request) {
        if (!authorize(request, false)) return;
        char body[196];
        snprintf(body, sizeof(body),
                 "{\"build\":\"SKB-HEADLESS-V3\",\"uptime_ms\":%lu,\"free_heap\":%u,\"ap_clients\":%u,\"ssid\":\"SKB-Headless-Pro\"}",
                 (unsigned long)millis(), (unsigned)ESP.getFreeHeap(),
                 (unsigned)WiFi.softAPgetStationNum());
        request->send(200, "application/json", body);
    });
}
