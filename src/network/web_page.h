#pragma once
// Giao diện đã hoàn chỉnh. Không cần học file này để giải thích phần cứng.
const char SMARTROOM_PAGE[] PROGMEM = R"SMARTROOM(<!doctype html>
<html lang="vi">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<meta name="theme-color" content="#285c47">
<title>SmartRoom · Không gian của bạn</title>
<style>
:root{--paper:#f5f6f2;--white:#fff;--ink:#24392f;--muted:#768178;--line:#e5e9e2;--green:#285c47;--mint:#eaf1e7;--orange:#bb6845;--blue:#5c84a4;--gold:#a58334;--red:#b9503e;--radius:22px}
*{box-sizing:border-box}html{scroll-behavior:smooth}body{margin:0;background:var(--paper);color:var(--ink);font-family:Inter,"Segoe UI",Arial,sans-serif;font-size:14px;-webkit-font-smoothing:antialiased}button,select{font:inherit}button,a,select{-webkit-tap-highlight-color:transparent}button{cursor:pointer}button:disabled{opacity:.46;cursor:not-allowed}button:focus-visible,a:focus-visible,select:focus-visible{outline:3px solid #96b999;outline-offset:4px}button{color:inherit}a{color:inherit;text-decoration:none}svg{display:block}.icon{width:21px;height:21px;fill:none;stroke:currentColor;stroke-width:1.7;stroke-linecap:round;stroke-linejoin:round;flex:none}.symbols{position:absolute;width:0;height:0;overflow:hidden}.muted{color:var(--muted)}[hidden]{display:none!important}
.sidebar{position:fixed;inset:0 auto 0 0;width:218px;border-right:1px solid var(--line);padding:33px 22px;display:flex;flex-direction:column;background:#f8f9f5;z-index:5}.brand{display:flex;align-items:center;gap:10px;font-weight:720;font-size:22px;letter-spacing:-.8px}.brand-mark{height:37px;width:37px;border-radius:12px;background:var(--green);color:#e5f0d8;display:grid;place-items:center}.brand-mark .icon{width:23px;height:23px}.brand small{display:block;font-size:10px;letter-spacing:1.8px;font-weight:550;margin-top:5px;color:var(--muted)}.nav-caption{font-size:10px;font-weight:650;letter-spacing:1.6px;color:#8a938b;margin:57px 12px 16px}.nav{display:grid;gap:8px}.nav a,.nav button{border:0;background:none;display:flex;align-items:center;gap:12px;padding:13px 12px;border-radius:11px;color:#748077;text-align:left;font-size:13px;font-weight:550}.nav a:hover,.nav button:hover{background:#edf0e9}.nav a.active{background:var(--mint);color:var(--green)}.nav .icon{width:19px;height:19px}.side-bottom{margin-top:auto}.side-note{padding:18px 14px;background:#edf1e8;border-radius:16px}.side-note .icon{margin-bottom:12px;color:#6a825f}.side-note b{font-size:12px;font-weight:650}.side-note p{font-size:11px;color:#7a8477;line-height:1.7;margin:7px 0 0}.side-footer{margin:22px 12px 0;font-size:10px;letter-spacing:1px;color:#8c978d}
main{margin-left:218px;padding:33px 40px 26px;max-width:1690px}.topbar{display:flex;justify-content:space-between;align-items:center;margin-bottom:33px}.eyebrow{font-size:10px;letter-spacing:1.7px;font-weight:650;color:#8a958b}.eyebrow span{margin:0 10px;color:#bcc5ba}.top-right{display:flex;align-items:center;gap:14px}.status-pill{font-size:11px;font-weight:600;display:inline-flex;align-items:center;gap:7px;padding:8px 11px;border:1px solid var(--line);border-radius:30px;background:#fff}.dot{width:6px;height:6px;border-radius:100%;background:#9ba79b;flex:none}.status-pill.online .dot{background:#4a8b61;box-shadow:0 0 0 3px #edf4e9}.status-pill.offline{color:var(--red);background:#fff3ee}.status-pill.offline .dot{background:var(--red)}.round-btn{width:34px;height:34px;display:grid;place-items:center;border-radius:100%;border:1px solid var(--line);background:transparent}.round-btn:hover{background:#fff}.round-btn .icon{width:16px;height:16px}.heading{display:flex;align-items:flex-end;justify-content:space-between;gap:20px;margin-bottom:24px}h1{font-size:clamp(28px,3vw,38px);font-weight:580;letter-spacing:-1.55px;margin:0 0 9px;line-height:1.25}.heading p{color:var(--muted);font-size:13px;margin:0;line-height:1.7}.date{font-size:12px;color:#7d877b;text-align:right;white-space:nowrap;padding-bottom:4px}.source-banner{display:flex;align-items:center;gap:10px;margin-bottom:25px;padding:11px 15px;border:1px solid #dce6d7;background:#eef3e9;border-radius:12px;color:#62775a;font-size:11px;line-height:1.6}.source-banner .icon{width:17px;height:17px}.source-banner strong{font-weight:650}.source-banner select{margin-left:auto;border:1px solid #d5dfce;border-radius:7px;padding:5px 8px;max-width:210px;background:#f8faf5;font-size:11px;color:#536b4e}.offline-notice{padding:12px 16px;margin:-8px 0 20px;border:1px solid #edc9bd;border-radius:12px;background:#fff2eb;color:#a0503c;font-size:12px;line-height:1.6}
.sensor-grid{display:grid;grid-template-columns:repeat(4,minmax(0,1fr));gap:17px;margin-bottom:24px}.sensor-card{background:#fff;border:1px solid var(--line);border-radius:var(--radius);padding:21px 21px 18px;position:relative;overflow:hidden;min-height:176px}.card-label{display:flex;align-items:center;justify-content:space-between;font-size:12px;font-weight:600;color:#6a766c}.sensor-icon{width:33px;height:33px;border-radius:10px;display:grid;place-items:center;background:var(--wash);color:var(--accent)}.sensor-icon .icon{width:19px;height:19px}.temp{--accent:#c57e5a;--wash:#faf0e8}.humidity{--accent:#6a93b4;--wash:#edf4f8}.light{--accent:#ad9043;--wash:#f8f3e6}.motion{--accent:#598573;--wash:#edf3ee}.metric{display:flex;gap:6px;align-items:baseline;margin:18px 0 13px;min-height:40px;font-size:38px;font-weight:560;letter-spacing:-1.9px;line-height:1}.metric small{font-size:19px;color:#a0a9a0;letter-spacing:-.5px;font-weight:400}.metric.motion-value{font-size:24px;letter-spacing:-.7px;align-items:center}.card-foot{display:flex;align-items:center;justify-content:space-between;gap:8px;font-size:10px;color:#899286}.sensor-card .spark{width:57px;height:18px;fill:none;stroke:var(--accent);stroke-width:1.5;opacity:.8}.sensor-card .tag{display:inline-flex;gap:5px;align-items:center;color:var(--accent);font-size:10px}.tag .dot{width:4px;height:4px;background:currentColor}.sensor-card.error{border-color:#e9b7a6;background:#fffaf7}.meter{height:3px;border-radius:6px;background:#f4efdf;width:55px;overflow:hidden}.meter span{display:block;height:100%;width:0;background:#bea157;transition:width .5s}.motion-bars{display:flex;align-items:center;gap:3px;height:17px;color:#aac5b0}.motion-bars i{display:block;height:5px;width:3px;background:currentColor;border-radius:3px}.motion-bars.active{color:#598573}.motion-bars.active i:nth-child(2n){height:13px}.motion-bars.active i:nth-child(3n){height:8px}
.middle-grid{display:grid;grid-template-columns:minmax(0,1.7fr) minmax(280px,1fr);gap:22px;margin-bottom:23px}.panel{border:1px solid var(--line);border-radius:var(--radius);background:#fff}.chart-panel{padding:24px 25px 18px;min-width:0}.section-head{display:flex;align-items:flex-start;justify-content:space-between;gap:14px}h2{font-size:16px;letter-spacing:-.4px;margin:0;font-weight:620}.section-head p{font-size:11px;color:var(--muted);margin:7px 0 0}.chart-tabs{display:flex;gap:4px;padding:3px;border:1px solid #ebeee7;border-radius:9px;background:#f7f8f4}.chart-tabs button{border:0;background:transparent;color:#879080;font-size:10px;font-weight:600;padding:6px 10px;border-radius:6px;white-space:nowrap}.chart-tabs button.selected{background:#fff;box-shadow:0 1px 4px #263b3012;color:var(--green)}.chart-summary{display:flex;align-items:center;gap:9px;margin:23px 0 3px}.legend-dot{height:6px;width:6px;background:var(--orange);border-radius:100%}.chart-summary b{font-size:23px;font-weight:540;letter-spacing:-.7px}.chart-summary small{font-size:11px;color:#9aa393}.chart-wrap{height:190px;position:relative}.chart-wrap svg{width:100%;height:100%;overflow:visible}.chart-empty{position:absolute;inset:15px 0 35px;display:grid;place-items:center;font-size:11px;color:#889383;pointer-events:none}.chart-foot{display:flex;justify-content:space-between;gap:10px;border-top:1px solid #f0f2ec;padding-top:14px;font-size:10px;color:#929b8e}.chart-foot b{font-weight:500;color:#75866f}.controller{padding:24px 25px;background:var(--green);color:#f2f6e9;position:relative;overflow:hidden;border-color:var(--green);min-height:341px}.controller::after{content:"";position:absolute;pointer-events:none;width:270px;height:270px;border-radius:100%;border:1px solid #ffffff07;top:35px;left:70px;box-shadow:0 0 0 30px #ffffff03,0 0 0 60px #ffffff02}.controller .section-head{position:relative;z-index:1}.controller .section-head p{color:#afc2aa}.controller-badge{font-size:9px;font-weight:600;letter-spacing:.6px;padding:5px 8px;border:1px solid #ffffff25;border-radius:6px;color:#bbd0b0}.lamp-stage{display:flex;align-items:center;gap:20px;min-height:142px;position:relative;z-index:1}.lamp-orb{height:86px;width:86px;border-radius:100%;display:grid;place-items:center;background:#ffffff0c;border:1px solid #ffffff15;color:#9fb59a;transition:background .3s,box-shadow .3s,color .3s}.lamp-orb .icon{width:37px;height:37px;stroke-width:1.25}.controller.lamp-on .lamp-orb{background:#e6ecc5;color:#426049;box-shadow:0 0 32px #dfedba38}.lamp-state{font-size:30px;letter-spacing:-1px;font-weight:500}.lamp-sub{color:#afc3a9;font-size:11px;margin-top:9px;line-height:1.5}.mode-buttons{display:grid;grid-template-columns:1.35fr 1fr 1fr;gap:4px;background:#174a363d;border:1px solid #a5c39b27;padding:5px;border-radius:11px;position:relative;z-index:1}.mode-buttons button{border:0;background:transparent;border-radius:7px;padding:10px 5px;font-size:11px;font-weight:600;color:#b7caaf;display:flex;align-items:center;justify-content:center;gap:5px}.mode-buttons button .icon{height:13px;width:13px}.mode-buttons button[aria-pressed=true]{background:#e2e9cd;color:#355440;box-shadow:0 2px 4px #17342620}.control-note{position:relative;z-index:1;display:flex;align-items:flex-start;gap:8px;color:#b2c4ab;font-size:10px;line-height:1.6;margin-top:16px;min-height:32px}.control-note .icon{width:13px;height:13px;margin-top:2px}.command-status{font-size:10px;line-height:1.5;color:#ecdbb2;min-height:15px;margin-top:4px;position:relative;z-index:1}
.bottom-grid{display:grid;grid-template-columns:minmax(0,1.7fr) minmax(280px,1fr);gap:22px}.room-summary{padding:23px 25px;display:flex;align-items:center;justify-content:space-between;gap:22px;background:#edf1e7;border:1px solid #e1e7d9;min-height:180px;overflow:hidden}.summary-copy{max-width:320px}.tiny-label{font-size:9px;font-weight:600;letter-spacing:1.6px;color:#91a084;margin-bottom:12px}.summary-title{font-size:20px;line-height:1.35;font-weight:560;letter-spacing:-.6px;margin:0 0 9px}.summary-text{font-size:11px;line-height:1.7;color:#819074;margin:0}.summary-chip{display:inline-flex;align-items:center;gap:5px;margin-top:14px;font-size:9px;color:#668057;background:#e2e9d8;border-radius:30px;padding:5px 8px}.summary-chip .icon{width:11px;height:11px}.room-art{width:160px;height:133px;flex:none}.room-summary.warning{background:#fbf0e4;border-color:#ecdbc8}.room-summary.warning .summary-title{color:#a66c3e}.room-summary.warning .summary-text{color:#977e65}.room-summary.warning .summary-chip{background:#f2e2cc;color:#9c744a}.devices{padding:23px 25px;scroll-margin-top:25px}.devices-heading{display:flex;align-items:center;justify-content:space-between}.devices-heading span{font-size:10px;color:#8b9684}.device-row{display:flex;justify-content:space-between;gap:10px;margin-top:15px;font-size:11px;align-items:center}.device-row span:first-child{display:flex;align-items:center;gap:8px;color:#788572}.device-row .icon{width:15px;height:15px;color:#98a28f}.device-row b{font-size:10px;font-weight:500;color:#6b815e}.device-row b.warn{color:#b97549}.footer{display:flex;justify-content:space-between;gap:15px;margin-top:24px;color:#9ba392;font-size:10px}.footer div{display:flex;align-items:center;gap:6px}.footer .icon{width:13px;height:13px}.stale .sensor-grid,.stale .chart-panel{opacity:.58}.stale #lamp-state{color:#c9d4c1}
dialog{border:1px solid var(--line);border-radius:23px;padding:28px;width:min(470px,calc(100% - 30px));color:var(--ink);box-shadow:0 20px 80px #14281d30;background:#fafbf7}dialog::backdrop{background:#1b322c68;backdrop-filter:blur(4px)}.dialog-head{display:flex;justify-content:space-between;align-items:center}.help-intro{font-size:12px;color:#7c8877;line-height:1.7}.help-step{display:flex;align-items:flex-start;gap:13px;margin:22px 0}.step-num{height:26px;width:26px;background:#e9efe1;border-radius:8px;display:grid;place-items:center;font-size:11px;flex:none;color:#658054}.help-step b{font-size:12px}.help-step p{font-size:12px;line-height:1.8;color:#7d8877;margin:5px 0}code{font-family:Consolas,monospace;font-size:11px;border:1px solid #e3e8db;border-radius:5px;padding:1px 4px;background:#f1f4eb}.noscript{padding:16px;background:#fff0df;color:#975e26;margin:16px;border-radius:12px}.chart-path{transition:stroke .2s}
@media(min-width:1700px){main{margin-right:auto;margin-left:max(218px,calc((100vw - 1460px)/2 + 180px))}}
@media(max-width:1200px){.sidebar{width:185px;padding:30px 17px}main{margin-left:185px;padding:30px 26px}.brand{font-size:19px}.sensor-grid{gap:12px}.sensor-card{padding:17px 15px}.card-foot{font-size:9px}.metric{font-size:34px}.metric.motion-value{font-size:21px}.middle-grid,.bottom-grid{gap:17px;grid-template-columns:minmax(0,1.5fr) minmax(265px,1fr)}.chart-panel,.controller,.room-summary,.devices{padding-left:20px;padding-right:20px}.room-art{width:118px}.chart-tabs button{padding:6px 8px}}
@media(max-width:1000px){.sidebar{width:76px;padding:27px 16px;align-items:center}.brand-copy,.nav-caption,.nav-label,.side-bottom{display:none}.brand-mark{height:39px;width:39px}.nav{margin-top:37px;gap:13px}.nav a,.nav button{padding:12px}.nav .icon{height:20px;width:20px}main{margin-left:76px;padding:29px 25px}.topbar{margin-bottom:26px}.sensor-grid{grid-template-columns:repeat(2,minmax(0,1fr));gap:15px}.sensor-card{padding:20px;min-height:168px}.card-foot{font-size:10px}.metric{font-size:37px}.metric.motion-value{font-size:24px}.room-art{display:none}.chart-summary{margin-top:22px}.chart-panel .section-head{display:block}.chart-tabs{display:inline-flex;margin-top:13px}.chart-wrap{height:174px}.lamp-stage{min-height:158px}.controller{min-height:373px}.date{display:none}}
@media(max-width:720px){.sidebar{position:static;width:100%;height:69px;flex-direction:row;justify-content:space-between;padding:0 20px;border-right:0;border-bottom:1px solid var(--line)}.brand-copy{display:block}.brand{font-size:18px;letter-spacing:-.5px;gap:9px}.brand-mark{height:32px;width:32px;border-radius:10px}.brand-mark .icon{height:20px;width:20px}.brand small{display:none}.nav{display:flex;margin:0;gap:5px}.nav a,.nav button{padding:9px}.nav a:first-child{display:none}.nav .icon{height:18px;width:18px}main{margin:0;padding:23px 20px}.topbar{margin-bottom:23px}.top-right{gap:9px}.eyebrow{font-size:8px;letter-spacing:1.2px}.eyebrow span{margin:0 4px}.status-pill{padding:6px 8px;font-size:9px}.round-btn{height:29px;width:29px}.heading{margin-bottom:18px}h1{font-size:30px;letter-spacing:-1.1px}.heading p{font-size:12px}.source-banner{font-size:10px;align-items:flex-start;flex-wrap:wrap;gap:8px;padding:11px 12px;margin-bottom:19px}.source-banner .icon{margin-top:1px}.source-banner .source-copy{flex:1;min-width:210px}.source-banner select{margin-left:25px;width:calc(100% - 25px);max-width:none}.sensor-grid{gap:11px;margin-bottom:18px}.sensor-card{padding:16px 15px;min-height:156px;border-radius:17px}.card-label{font-size:11px}.sensor-icon{width:28px;height:28px;border-radius:8px}.sensor-icon .icon{width:16px;height:16px}.metric{font-size:33px;margin:18px 0 11px;min-height:32px}.metric small{font-size:16px}.metric.motion-value{font-size:20px}.card-foot{font-size:9px}.sensor-card .spark{width:37px}.meter{width:37px}.card-foot .tag{font-size:9px}.motion-bars{gap:2px}.motion-bars i{width:2px}.middle-grid,.bottom-grid{grid-template-columns:1fr;gap:18px;margin-bottom:18px}.chart-panel{padding:22px 19px 17px}.chart-panel .section-head{display:flex}.section-head h2{font-size:15px}.chart-tabs{margin-top:0}.chart-tabs button{font-size:9px;padding:6px 7px}.section-head p{font-size:10px}.chart-wrap{height:183px}.chart-summary{margin-top:23px}.controller{min-height:310px;padding:22px}.lamp-stage{min-height:125px;gap:20px}.lamp-orb{height:78px;width:78px}.lamp-state{font-size:29px}.control-note{min-height:0}.room-summary{padding:23px 21px;min-height:177px;gap:5px}.room-art{display:block;width:115px}.summary-title{font-size:18px}.summary-text{font-size:10px}.summary-chip{font-size:8px}.devices{padding:23px}.footer{font-size:9px;flex-wrap:wrap;gap:9px;margin:19px 0 0}.chart-foot{font-size:9px}.bottom-grid{margin-bottom:0}.device-row{font-size:12px;margin-top:19px}}
@media(prefers-reduced-motion:reduce){html{scroll-behavior:auto}*,*::before,*::after{transition:none!important}}
/* Cỡ chữ và độ tương phản cho màn hình nhỏ; không tải font từ Internet. */
.brand{font-size:20px}.brand-mark{flex:none}.brand small{font-size:8px;letter-spacing:1.1px;white-space:nowrap}.nav-caption{font-size:9px;letter-spacing:1.1px}
.temp{--accent:#a96240}.humidity{--accent:#4e7898}.light{--accent:#8c7129}.motion{--accent:#47735d}
.heading p,.section-head p,.card-foot,.chart-foot,.chart-foot b,.summary-text,.device-row span:first-child,.footer,.help-intro,.help-step p{color:#6d7c67}
.controller .section-head p{color:#bacdb2}.controller .control-note{color:#c3d2b9}.sensor-card .tag{font-size:10px}
@media(max-width:720px){.brand{font-size:18px}.card-foot .tag{font-size:9px}.chart-panel .section-head{display:block}.chart-tabs{display:flex;margin-top:14px}.chart-tabs button{flex:1;font-size:11px;min-height:35px}.mode-buttons button{min-height:44px;font-size:12px}.nav button,.nav a{min-width:42px;min-height:42px}.top-right .round-btn{width:35px;height:35px}.chart-foot,.footer{font-size:10px}}
</style>
</head>
<body>
<svg class="symbols" xmlns="http://www.w3.org/2000/svg" aria-hidden="true"><defs>
<symbol id="i-home" viewBox="0 0 24 24"><path d="M3 10.5 12 3l9 7.5M5 9v11h14V9M9 20v-7h6v7"/></symbol>
<symbol id="i-grid" viewBox="0 0 24 24"><rect x="3" y="3" width="7" height="7" rx="1.5"/><rect x="14" y="3" width="7" height="7" rx="1.5"/><rect x="3" y="14" width="7" height="7" rx="1.5"/><rect x="14" y="14" width="7" height="7" rx="1.5"/></symbol>
<symbol id="i-chip" viewBox="0 0 24 24"><rect x="6" y="6" width="12" height="12" rx="2"/><path d="M9 6V3m6 3V3M9 21v-3m6 3v-3M6 9H3m3 6H3m18-6h-3m3 6h-3"/><rect x="9" y="9" width="6" height="6" rx="1"/></symbol>
<symbol id="i-help" viewBox="0 0 24 24"><circle cx="12" cy="12" r="9"/><path d="M9.5 9a2.5 2.5 0 1 1 3.7 2.2c-1.2.7-1.2 1-1.2 2M12 17h.01"/></symbol>
<symbol id="i-leaf" viewBox="0 0 24 24"><path d="M20 3C9 2 3 8 5 14s14 9 15-11ZM4 21l11-12"/></symbol>
<symbol id="i-wifi" viewBox="0 0 24 24"><path d="M2 8a17 17 0 0 1 20 0M5 12a12 12 0 0 1 14 0M8.5 16a6 6 0 0 1 7 0M12 20h.01"/></symbol>
<symbol id="i-refresh" viewBox="0 0 24 24"><path d="M20 8A8 8 0 0 0 6 5L3 8m0-5v5h5M4 16a8 8 0 0 0 14 3l3-3m0 5v-5h-5"/></symbol>
<symbol id="i-flask" viewBox="0 0 24 24"><path d="M9 3h6M10 3v6L4 19a1.4 1.4 0 0 0 1 2h14a1.4 1.4 0 0 0 1-2L14 9V3M7 15h10"/></symbol>
<symbol id="i-temp" viewBox="0 0 24 24"><path d="M9 14.5V5a3 3 0 0 1 6 0v9.5a5 5 0 1 1-6 0ZM12 8v10M18 6h2M18 10h2"/><circle cx="12" cy="18" r="1"/></symbol>
<symbol id="i-drop" viewBox="0 0 24 24"><path d="M12 2C9 6 5 11 5 15a7 7 0 0 0 14 0c0-4-4-9-7-13ZM8 15a4 4 0 0 0 4 4"/></symbol>
<symbol id="i-sun" viewBox="0 0 24 24"><circle cx="12" cy="12" r="4"/><path d="M12 2v2m0 16v2M2 12h2m16 0h2M5 5l1.4 1.4m11.2 11.2L19 19M5 19l1.4-1.4M17.6 6.4 19 5"/></symbol>
<symbol id="i-motion" viewBox="0 0 24 24"><circle cx="13" cy="4" r="2"/><path d="m8 22 3-7 4 3v4M5 12l4-5 5 2 3 5h4M10 8l-1 6 6 4M3 6h3M2 17h3"/></symbol>
<symbol id="i-bulb" viewBox="0 0 24 24"><path d="M9 18h6m-5 3h4M8 15a7 7 0 1 1 8 0c-1 .7-1 1.5-1 3H9c0-1.5 0-2.3-1-3ZM10 11l2 2 2-2m-2 2v5"/></symbol>
<symbol id="i-auto" viewBox="0 0 24 24"><path d="m13 2-9 12h7l-1 8 10-12h-7l1-8"/></symbol>
<symbol id="i-info" viewBox="0 0 24 24"><circle cx="12" cy="12" r="9"/><path d="M12 11v6m0-10h.01"/></symbol>
<symbol id="i-check" viewBox="0 0 24 24"><path d="m5 12 4 4L19 6"/></symbol>
<symbol id="i-close" viewBox="0 0 24 24"><path d="m6 6 12 12M6 18 18 6"/></symbol>
<symbol id="i-clock" viewBox="0 0 24 24"><circle cx="12" cy="12" r="9"/><path d="M12 7v5l3 2"/></symbol>
</defs></svg>
<aside class="sidebar" aria-label="Điều hướng chính">
  <a class="brand" href="#overview" aria-label="SmartRoom — tổng quan"><span class="brand-mark"><svg class="icon"><use href="#i-home"/></svg></span><span class="brand-copy">SmartRoom<small>GIÁM SÁT CĂN PHÒNG</small></span></a>
  <p class="nav-caption">KHÔNG GIAN CỦA BẠN</p>
  <nav class="nav">
    <a class="active" href="#overview" aria-label="Tổng quan"><svg class="icon"><use href="#i-grid"/></svg><span class="nav-label">Tổng quan</span></a>
    <a href="#devices" aria-label="Thiết bị"><svg class="icon"><use href="#i-chip"/></svg><span class="nav-label">Thiết bị</span></a>
    <button id="help-open" type="button" aria-label="Hướng dẫn kết nối"><svg class="icon"><use href="#i-help"/></svg><span class="nav-label">Cách kết nối</span></button>
  </nav>
  <div class="side-bottom"><div class="side-note"><svg class="icon"><use href="#i-leaf"/></svg><b>Một nguồn USB.<br>Một góc thông minh.</b><p>Quan sát môi trường và tự động bật đèn khi cần.</p></div><div class="side-footer">ESP32-C3 · 3 CẢM BIẾN</div></div>
</aside>
<main id="overview">
  <div class="topbar"><div class="eyebrow">SMARTROOM <span>/</span> KHÔNG GIAN 01</div><div class="top-right"><span id="connection" class="status-pill" role="status"><span class="dot"></span><span id="connection-text">Đang kết nối</span></span><button id="refresh" class="round-btn" title="Cập nhật dữ liệu" aria-label="Cập nhật dữ liệu"><svg class="icon"><use href="#i-refresh"/></svg></button></div></div>
  <header class="heading"><div><h1>Phòng của bạn.</h1><p>Một cái nhìn, nắm bắt cả không gian.</p></div><time id="today" class="date"></time></header>
<div class="source-banner"><svg class="icon"><use href="#i-leaf"/></svg><span class="source-copy"><strong id="source-title">Đang chờ ESP32</strong><span id="source-note">Dữ liệu từ các cảm biến trên mô hình.</span></span></div>
  <div class="offline-notice" id="offline-notice" role="alert" hidden>Mất kết nối ESP32. Các số mờ là lần đọc cuối, không phải dữ liệu hiện tại. Kiểm tra Wi-Fi SmartRoom-C3; trang sẽ tự kết nối lại.</div>
  <noscript><p class="noscript">Bật JavaScript trong trình duyệt để xem dữ liệu và điều khiển đèn.</p></noscript>
  <section class="sensor-grid" aria-label="Số đo cảm biến">
    <article class="sensor-card temp" id="temp-card"><div class="card-label">Nhiệt độ<span class="sensor-icon"><svg class="icon"><use href="#i-temp"/></svg></span></div><div class="metric"><span id="temperature">—</span><small>°C</small></div><div class="card-foot"><span class="tag"><span class="dot"></span><span id="temp-note">Chờ DHT11</span></span><svg class="spark" viewBox="0 0 60 20" aria-label="Xu hướng nhiệt độ"><path id="temp-spark"/></svg></div></article>
    <article class="sensor-card humidity" id="humidity-card"><div class="card-label">Độ ẩm<span class="sensor-icon"><svg class="icon"><use href="#i-drop"/></svg></span></div><div class="metric"><span id="humidity">—</span><small>%</small></div><div class="card-foot"><span class="tag"><span class="dot"></span><span id="humidity-note">Chờ DHT11</span></span><svg class="spark" viewBox="0 0 60 20" aria-label="Xu hướng độ ẩm"><path id="humidity-spark"/></svg></div></article>
    <article class="sensor-card light"><div class="card-label">Ánh sáng<span class="sensor-icon"><svg class="icon"><use href="#i-sun"/></svg></span></div><div class="metric"><span id="light">—</span><small>lux</small></div><div class="card-foot"><span class="tag"><span class="dot"></span><span id="light-note">Chờ BH1750</span></span><div class="meter" aria-hidden="true"><span id="light-meter"></span></div></div></article>
    <article class="sensor-card motion"><div class="card-label">Chuyển động<span class="sensor-icon"><svg class="icon"><use href="#i-motion"/></svg></span></div><div class="metric motion-value" id="motion">Đang chờ</div><div class="card-foot"><span class="tag"><span class="dot"></span><span id="motion-note">PIR HC-SR501</span></span><span class="motion-bars" id="motion-bars" aria-hidden="true"><i></i><i></i><i></i><i></i><i></i><i></i><i></i></span></div></article>
  </section>
  <div class="middle-grid">
    <section class="panel chart-panel" aria-labelledby="chart-title">
      <div class="section-head"><div><h2 id="chart-title">Nhịp điệu căn phòng</h2><p>Số đo mới mỗi 2 giây</p></div><div class="chart-tabs" role="group" aria-label="Đại lượng biểu đồ"><button class="selected" data-chart="t" aria-pressed="true">Nhiệt độ</button><button data-chart="h" aria-pressed="false">Độ ẩm</button><button data-chart="lux" aria-pressed="false">Sáng</button></div></div>
      <div class="chart-summary"><span class="legend-dot" id="legend-dot"></span><b id="chart-number">—</b><small id="chart-unit">°C</small></div>
      <div class="chart-wrap"><svg id="history-chart" viewBox="0 0 600 190" preserveAspectRatio="none" role="img" aria-label="Biểu đồ nhiệt độ theo thời gian"><defs><linearGradient id="area-fill" x1="0" y1="0" x2="0" y2="1"><stop id="fill-color" offset="0%" stop-color="#bb6845" stop-opacity=".15"/><stop offset="100%" stop-color="#bb6845" stop-opacity="0"/></linearGradient></defs><g id="chart-grid"></g><g id="chart-data"></g></svg><div id="chart-empty" class="chart-empty">Chờ mẫu đầu tiên…</div></div>
      <div class="chart-foot"><span id="history-caption">Lịch sử trong lần mở trang này</span><b id="sample-count">0 mẫu</b></div>
    </section>
    <section class="panel controller" id="controller" aria-labelledby="control-title">
      <div class="section-head"><div><h2 id="control-title">Ánh sáng theo ý bạn</h2><p>Đèn LED mô hình</p></div><span class="controller-badge" id="mode-badge">AUTO</span></div>
      <div class="lamp-stage"><div class="lamp-orb"><svg class="icon"><use href="#i-bulb"/></svg></div><div><div class="lamp-state" id="lamp-state">Đang chờ</div><div class="lamp-sub" id="lamp-sub">Chờ trạng thái từ thiết bị</div></div></div>
      <div class="mode-buttons" role="group" aria-label="Chế độ đèn"><button data-mode="0" aria-pressed="false" disabled><svg class="icon"><use href="#i-auto"/></svg>Tự động</button><button data-mode="1" aria-pressed="false" disabled>Bật đèn</button><button data-mode="2" aria-pressed="false" disabled>Tắt đèn</button></div>
      <div class="control-note"><svg class="icon"><use href="#i-info"/></svg><span id="control-note">Tự bật khi trời tối và PIR báo chuyển động.</span></div><div id="command-status" class="command-status" role="status"></div>
    </section>
  </div>
  <div class="bottom-grid">
    <section class="panel room-summary" id="room-summary" aria-label="Tình trạng môi trường"><div class="summary-copy"><div class="tiny-label">LẮNG NGHE KHÔNG GIAN</div><h2 id="summary-title" class="summary-title">Sẵn sàng quan sát.</h2><p id="summary-text" class="summary-text">Chờ số đo đầu tiên để đánh giá theo ngưỡng của mô hình.</p><span class="summary-chip"><svg class="icon"><use href="#i-leaf"/></svg><span id="summary-chip">Nhiệt độ 32°C · Độ ẩm 80%</span></span></div>
      <svg class="room-art" viewBox="0 0 180 145" role="img" aria-label="Minh họa góc phòng"><path d="m16 45 72-33 75 37v65l-72 28-75-34Z" fill="#e3e9db"/><path d="m88 12 75 37v65l-75-34Z" fill="#d5decb"/><path d="m16 108 72-28 75 34-72 28Z" fill="#c9d3bf"/><path d="M36 50 66 37v31L36 81Z" fill="#f8faf1"/><path d="M50 44v30M36 66l30-13" stroke="#ccd8bf" stroke-width="2"/><path d="m34 94 30-12 40 18-29 12Z" fill="#f7f7e9"/><path d="m34 94 41 18v17l-41-18Z" fill="#a9baa0"/><path d="m75 112 29-12v16l-29 13Z" fill="#93ab8c"/><path d="m38 96 27-10 32 14-24 10Z" fill="#e0e7cf"/><path d="m40 89 8-4 14 6-8 4Z" fill="#bec9aa"/><path d="m110 95 18-7 18 9-17 7Z" fill="#f5f3df"/><path d="M129 104v17M112 96v17m31-17v16" stroke="#819979" stroke-width="3"/><path d="M129 89V70" stroke="#718c67" stroke-width="2"/><path d="m121 62 13 1 7 13-25 1Z" fill="#e9e6c2" id="room-lamp"/><path d="M151 107v-18m0 5c-10 0-15-8-13-13 11 0 13 8 13 13Zm0-6c9-1 12-10 9-14-9 1-11 8-9 14Z" fill="#8fa57b" stroke="#829b70"/><path d="m142 104 16 1-3 14-10-2Z" fill="#bdc8af"/></svg>
    </section>
    <section class="panel devices" id="devices" aria-labelledby="devices-title"><div class="devices-heading"><h2 id="devices-title">Trạm SmartRoom</h2><span>ESP32-C3</span></div><div class="device-row"><span><svg class="icon"><use href="#i-chip"/></svg>DHT11 · BH1750 · PIR</span><b id="sensor-source">Đang chờ</b></div><div class="device-row"><span><svg class="icon"><use href="#i-clock"/></svg>Timer phần cứng</span><b id="timer-state">Đang chờ</b></div><div class="device-row"><span><svg class="icon"><use href="#i-wifi"/></svg>BLE / OLED I²C</span><b id="peripheral-state">Đang chờ</b></div><div class="device-row"><span><svg class="icon"><use href="#i-auto"/></svg>Thời gian hoạt động</span><b id="uptime">—</b></div></section>
  </div>
  <footer class="footer"><div><svg class="icon"><use href="#i-wifi"/></svg><span id="footer-network">SmartRoom-C3 · 192.168.4.1</span></div><span id="last-update">Chưa nhận dữ liệu</span></footer>
</main>
<dialog id="help-dialog" aria-labelledby="help-title"><div class="dialog-head"><h2 id="help-title">Kết nối SmartRoom</h2><button class="round-btn" id="help-close" aria-label="Đóng">×</button></div><p>1. Cấp nguồn USB cho ESP32 và chờ vài giây.</p><p>2. Kết nối Wi-Fi <b>SmartRoom-C3</b>, mật khẩu <b>smart1234</b>. Chọn giữ kết nối dù mạng không có Internet.</p><p>3. Mở <b>http://192.168.4.1</b> trên trình duyệt.</p><p>PIR cần 60 giây ổn định. Bật tay và tắt tay dùng được trong lúc chờ. Nếu số đo hiện dấu gạch, kiểm tra dây cảm biến.</p></dialog>
<script>
'use strict';
// Giao diện độc lập; file:// dùng mô phỏng rõ nhãn, HTTP luôn đọc ESP32.
const $ = id => document.getElementById(id);
const history = [], LIMIT = 150;
const chartOptions = {t:{name:'nhiệt độ',unit:'°C',color:'#bb6845',low:20,high:36},h:{name:'độ ẩm',unit:'%',color:'#5c84a4',low:40,high:90},lux:{name:'ánh sáng',unit:'lux',color:'#a58334',low:0,high:500}};
let latest = null, online = false, busy = false, chartKey = 't', lastSample = null, lastUptime = null;
let pollTimer = 0, requestId = 0;
const valueText = (value, decimals=1) => Number.isFinite(value) ? value.toFixed(decimals) : '—';
const clock = ms => new Date(ms).toLocaleTimeString('vi-VN',{hour:'2-digit',minute:'2-digit',second:'2-digit'});
$('today').textContent = new Date().toLocaleDateString('vi-VN',{weekday:'long',day:'numeric',month:'long'});

function validStatus(s) {
  const flag = v => v === 0 || v === 1 || typeof v === 'boolean';
  return s && [0,1,2].includes(s.mode) && ['pir','lamp','alert','dark','ready','pirReady','timer','oled'].every(k=>flag(s[k])) &&
    ['t','h','lux'].every(k=>s[k]===null || Number.isFinite(s[k])) && Number.isInteger(s.adc) && s.adc>=0 && s.adc<=4095 &&
    Number.isFinite(s.uptime) && s.uptime>=0 && Number.isFinite(s.sampleMs) && s.sampleMs>=0 && Number.isInteger(s.bleClients) && s.bleClients>=0 && Number.isFinite(s.threshold) && Number.isFinite(s.tMax) && Number.isFinite(s.hMax);
}
function connect(isOnline) {
  if (isOnline && !online) $('command-status').textContent='';
  online = isOnline;
  $('connection').className = 'status-pill ' + (isOnline ? 'online':'offline');
  $('connection-text').textContent = isOnline ? 'Đang kết nối ESP32':'Mất kết nối';
  $('offline-notice').hidden = isOnline;
  document.body.classList.toggle('stale',!isOnline);
  document.querySelectorAll('[data-mode]').forEach(b=>b.disabled=!isOnline || busy);
  if (!isOnline) {
    $('lamp-state').textContent='Chưa xác định';
  $('lamp-sub').textContent='Trạng thái đèn LED trên mô hình';
    $('controller').classList.remove('lamp-on');
    $('summary-title').textContent='Tạm mất kết nối.';
    $('summary-text').textContent='Kiểm tra nguồn ESP32 và mạng Wi-Fi SmartRoom-C3. Trang sẽ tự thử lại.';
    $('room-summary').classList.add('warning');
    $('sensor-source').textContent='Dữ liệu cũ';
    $('timer-state').textContent='Chưa xác định';
    $('peripheral-state').textContent='Chưa xác định';
    $('uptime').textContent='—';
    $('mode-badge').textContent='NGOẠI TUYẾN';
    document.querySelectorAll('[data-mode]').forEach(b=>b.setAttribute('aria-pressed','false'));
  }
}
function render(s) {
  latest=s; connect(true);
  const sampled=!!s.ready, dhtError=sampled&&(s.t===null||s.h===null);
  const warm=sampled&&s.t!==null&&s.t>=s.tMax, humid=sampled&&s.h!==null&&s.h>=s.hMax;
  $('source-title').textContent='DỮ LIỆU CẢM BIẾN THẬT';
  $('source-note').textContent=' · DHT11 · BH1750 · PIR · OLED I2C';
  $('temperature').textContent=sampled?valueText(s.t):'—';
  $('humidity').textContent=sampled?valueText(s.h):'—';
  $('light').textContent=sampled?valueText(s.lux):'—';
  $('motion').textContent=!sampled?'Đang chờ':!s.pirReady?'Khởi động':s.pir?'Có chuyển động':'Chưa phát hiện';
  $('temp-note').textContent=!sampled?'Chờ mẫu đầu':s.t===null?'Lỗi đọc DHT':(warm?'Cảnh báo ≥ ':'Dưới ngưỡng ')+s.tMax+'°C';
  $('humidity-note').textContent=!sampled?'Chờ mẫu đầu':s.h===null?'Lỗi đọc DHT':(humid?'Cảnh báo ≥ ':'Dưới ngưỡng ')+s.hMax+'%';
  $('light-note').textContent=!sampled?'Chờ BH1750':s.lux===null?'Lỗi BH1750':(s.dark?'Tối':'Sáng')+' · ngưỡng '+Math.round(s.threshold)+' lux';
  $('motion-note').textContent=!s.pirReady?'Chờ ổn định 60 giây':'PIR HC-SR501';
  $('temp-card').classList.toggle('error',sampled&&(s.t===null||warm));
  $('humidity-card').classList.toggle('error',sampled&&(s.h===null||humid));
  $('light-meter').style.width=(sampled&&s.lux!==null?Math.min(s.lux/500*100,100):0)+'%';
  $('motion-bars').classList.toggle('active',sampled&&!!s.pirReady&&!!s.pir);
  $('controller').classList.toggle('lamp-on',!!s.lamp);
  $('lamp-state').textContent=s.lamp?'Đang bật':'Đang tắt';
  $('lamp-sub').textContent='Trạng thái đèn LED trên mô hình';
  $('mode-badge').textContent=['AUTO','BẬT TAY','TẮT TAY'][s.mode];
  document.querySelectorAll('[data-mode]').forEach(b=>b.setAttribute('aria-pressed',String(Number(b.dataset.mode)===s.mode)));
  $('control-note').textContent=s.mode===0?'Tự bật khi tối và có chuyển động. Biến trở: '+Math.round(s.threshold)+' lux (ADC '+s.adc+').':'Điều khiển thủ công. Cảnh báo nhiệt / ẩm vẫn hoạt động.';
  $('room-summary').classList.toggle('warning',dhtError||!!s.alert||!s.timer);
  $('summary-title').textContent=!sampled?'Sẵn sàng quan sát.':dhtError?'DHT cần được kiểm tra.':s.lux===null?'BH1750 cần được kiểm tra.':s.alert?'Căn phòng cần chú ý.':!s.timer?'Kiểm tra timer phần cứng.':'Mọi thứ đang ổn.';
  $('summary-text').textContent=!sampled?'Chờ số đo đầu tiên để đánh giá theo ngưỡng của mô hình.':dhtError?'Không đọc được nhiệt độ hoặc độ ẩm. Kiểm tra dây DATA và nguồn của DHT.':s.lux===null?'Không đọc được BH1750. AUTO tắt đèn; kiểm tra dây I²C.':s.alert?(warm&&humid?'Nhiệt độ và độ ẩm đã chạm ngưỡng cảnh báo của mô hình.':warm?'Nhiệt độ đã chạm ngưỡng cảnh báo '+s.tMax+'°C.':'Độ ẩm đã chạm ngưỡng cảnh báo '+s.hMax+'%.'):!s.timer?'Đang dùng millis dự phòng. Cần sửa timer để trình diễn ngắt phần cứng.':'Nhiệt độ và độ ẩm đang dưới ngưỡng cảnh báo của mô hình.';
  $('summary-chip').textContent=dhtError?'LED cảnh báo bật':s.alert?'Theo ngưỡng do nhóm đặt':'Nhiệt độ '+s.tMax+'°C · Độ ẩm '+s.hMax+'%';
  $('room-lamp').setAttribute('fill',s.lamp?'#f5d58a':'#e9e6c2');
  $('sensor-source').textContent=!sampled?'Chờ đọc':dhtError?'Lỗi DHT':'Đang đọc';
  $('timer-state').textContent=s.timer?'Timer 2 giây/lần':'Lỗi · millis dự phòng';
  $('timer-state').classList.toggle('warn',!s.timer);
  $('peripheral-state').textContent='BLE: '+s.bleClients+' · OLED '+(s.oled?'đã nhận':'chưa nhận');
  $('uptime').textContent=Math.floor(s.uptime/3600)+' giờ '+Math.floor(s.uptime%3600/60)+' phút '+s.uptime%60+' giây';
  $('footer-network').textContent='SmartRoom-C3 · 192.168.4.1';
  $('last-update').textContent='Cập nhật lúc '+clock(Date.now());
  if(lastUptime!==null && s.uptime<lastUptime){history.length=0;lastSample=null;}
  lastUptime=s.uptime;
  if(sampled && s.sampleMs!==lastSample){history.push({time:Date.now(),t:s.t,h:s.h,lux:s.lux});if(history.length>LIMIT)history.shift();lastSample=s.sampleMs;}
  drawChart(); drawSpark('t','temp-spark'); drawSpark('h','humidity-spark');
}
function svgNode(tag,attrs,text) {
  const node=document.createElementNS('http://www.w3.org/2000/svg',tag);
  Object.entries(attrs).forEach(([k,v])=>node.setAttribute(k,v));
  if(text!==undefined)node.textContent=text;
  return node;
}
function drawChart() {
  const opt=chartOptions[chartKey], points=history.filter(p=>Number.isFinite(p[chartKey]));
  $('chart-number').textContent=latest&&latest.ready?valueText(latest[chartKey],chartKey==='lux'?0:1):'—';
  $('chart-unit').textContent=opt.unit;
  $('legend-dot').style.background=opt.color;
  $('fill-color').setAttribute('stop-color',opt.color);
  $('history-chart').setAttribute('aria-label','Biểu đồ '+opt.name+'; '+points.length+' mẫu hợp lệ trong lần mở trang');
  $('chart-empty').hidden=points.length>0;
  $('chart-empty').textContent=history.length?'Chưa có số đo hợp lệ':'Chờ mẫu đầu tiên…';
  $('sample-count').textContent=history.length+' / '+LIMIT+' mẫu';
  const grid=$('chart-grid'), data=$('chart-data');grid.replaceChildren();data.replaceChildren();
  const min=Math.min(opt.low,...points.map(p=>p[chartKey])), max=Math.max(opt.high,...points.map(p=>p[chartKey]));
  const width=Math.max(260,$('history-chart').clientWidth);
  $('history-chart').setAttribute('viewBox','0 0 '+width+' 190');
  const left=32,right=width-8,top=16,bottom=151;
  const start=history.length?history[0].time:Date.now(), end=history.length>1?history[history.length-1].time:start+2000;
  const x=p=>left+(p.time-start)/Math.max(1,end-start)*(right-left), y=p=>bottom-(p[chartKey]-min)/(max-min)*(bottom-top);
  for(let i=0;i<4;i++){
    const gy=top+(bottom-top)*i/3, val=max-(max-min)*i/3;
    grid.append(svgNode('line',{x1:left,y1:gy,x2:right,y2:gy,stroke:'#edf0e8','stroke-dasharray':'3 5'}));
    grid.append(svgNode('text',{x:24,y:gy+3,fill:'#7d8a74','font-size':10,'text-anchor':'end'},String(Math.round(val))));
  }
  if(history.length){
    const count=history.length===1?1:3;
    for(let i=0;i<count;i++){const f=count===1?0:i/2;grid.append(svgNode('text',{x:left+f*(right-left),y:178,fill:'#7d8a74','font-size':10,'text-anchor':f===0?'start':f===1?'end':'middle'},clock(start+f*(end-start))));}
  }
  // Mẫu DHT lỗi tạo khoảng trống; không tự thay số lỗi bằng số 0.
  let segment=[];
  function flush(){if(!segment.length)return;const path=segment.map((p,i)=>(i?'L':'M')+x(p).toFixed(1)+','+y(p).toFixed(1)).join(' ');if(segment.length>1)data.append(svgNode('path',{d:path+' L'+x(segment.at(-1))+','+bottom+' L'+x(segment[0])+','+bottom+' Z',fill:'url(#area-fill)'}));data.append(svgNode('path',{d:path,fill:'none',stroke:opt.color,'stroke-width':2.4,'stroke-linecap':'round','stroke-linejoin':'round','vector-effect':'non-scaling-stroke',class:'chart-path'}));segment=[];}
  history.forEach((p,i)=>{if(i&&p.time-history[i-1].time>5000)flush();if(Number.isFinite(p[chartKey]))segment.push(p);else flush();});flush();
  const last=history.at(-1);if(last&&Number.isFinite(last[chartKey]))data.append(svgNode('circle',{cx:x(last),cy:y(last),r:3.5,fill:opt.color,stroke:'#fff','stroke-width':2}));
}
function drawSpark(key,id){
  const points=history.slice(-18), valid=points.filter(p=>Number.isFinite(p[key]));
  if(!valid.length){$(id).setAttribute('d','');return;}
  const min=Math.min(...valid.map(p=>p[key]))-1,max=Math.max(...valid.map(p=>p[key]))+1;let path='',gap=true;
  points.forEach((p,i)=>{if(!Number.isFinite(p[key])){gap=true;return;}path+=(gap?'M':'L')+(i/Math.max(1,points.length-1)*58+1).toFixed(1)+','+(18-(p[key]-min)/(max-min)*16).toFixed(1)+' ';gap=false;});$(id).setAttribute('d',path);
}
async function request(path,options={}) {
  const controller=new AbortController(), timeout=setTimeout(()=>controller.abort(),4000);
  try {const response=await fetch(path,{...options,cache:'no-store',signal:controller.signal});if(!response.ok)throw new Error('HTTP '+response.status);const state=await response.json();if(!validStatus(state))throw new Error('JSON không hợp lệ');return state;}finally{clearTimeout(timeout);}
}
function schedule(){clearTimeout(pollTimer);pollTimer=setTimeout(poll,2000);}
async function poll(){
  if(busy){schedule();return;} const id=++requestId;
  try {const s=await request('/api/status');if(id===requestId)render(s);}catch(error){if(id===requestId)connect(false);}finally{schedule();}
}
async function setMode(mode){
  if(busy||!online)return;
  clearTimeout(pollTimer);busy=true;++requestId;connect(true);$('command-status').textContent='Đang gửi lệnh…';
  try {const s=await request('/api/mode',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'mode='+mode});render(s);$('command-status').textContent='ESP32 đã xác nhận chế độ '+['AUTO','BẬT','TẮT'][s.mode]+'.';}
  catch(error){connect(false);$('command-status').textContent='Chưa xác nhận được lệnh. Kiểm tra trạng thái sau khi kết nối lại.';}
  finally{busy=false;document.querySelectorAll('[data-mode]').forEach(b=>b.disabled=!online);schedule();}
}
document.querySelectorAll('[data-mode]').forEach(b=>b.addEventListener('click',()=>setMode(Number(b.dataset.mode))));
document.querySelectorAll('[data-chart]').forEach(b=>b.addEventListener('click',()=>{chartKey=b.dataset.chart;document.querySelectorAll('[data-chart]').forEach(tab=>{tab.classList.toggle('selected',tab===b);tab.setAttribute('aria-pressed',String(tab===b));});drawChart();}));
$('refresh').addEventListener('click',()=>{clearTimeout(pollTimer);poll();});
$('help-open').addEventListener('click',()=>$('help-dialog').showModal());
$('help-close').addEventListener('click',()=>$('help-dialog').close());
$('help-dialog').addEventListener('click',e=>{if(e.target===$('help-dialog')){const r=e.target.getBoundingClientRect();if(e.clientX<r.left||e.clientX>r.right||e.clientY<r.top||e.clientY>r.bottom)e.target.close();}});
document.addEventListener('visibilitychange',()=>{if(!document.hidden){clearTimeout(pollTimer);poll();}});
drawChart();poll();
window.addEventListener('resize',drawChart);
</script>
</body>
</html>
)SMARTROOM";
