#!/usr/bin/env python3
"""SKB source-level integration checks. Not a hardware or behavioural test."""
from pathlib import Path

def require(path, *needles):
    source = Path(path).read_text(encoding="utf-8")
    missing = [item for item in needles if item not in source]
    if missing:
        raise SystemExit(f"FAIL {path}: missing {missing}")
    print(f"PASS {path}: {len(needles)} integration markers")

require("src/main.cpp", "skbStartHeadless();", 'tft.drawCentreString("SKB"', "startSerialCommandsHandlerTask(true)")
require("src/skb_headless.h", 'WiFi.softAP("SKB-Headless-Pro"', "skbHeadlessBackground = true;")
require("src/core/display.cpp", "skbCaptureMenuState(", "skbSelectMenuItem(", "forceMenuOption = index", "skbSetDeviceActivity(", "SKB PRO")
require("src/core/display.h", "skbMenuStateJson()", "skbSelectMenuItem(")
require("src/core/wifi/webInterface.cpp", '"/skb/api/menu"', '"/skb/api/select"', '"/getscreen"', '"/cm"', '"/bruce"', 'request->redirect("/skb")')
require("src/skb_dashboard.h", '"/skb/api/status"', 'SKB-HEADLESS-V3', '"/skb/api/menu"', '"/skb/api/select"', 'data-nav=', 'src="/bruce"')
require("src/core/main_menu.cpp", "&wifiMenu", "&bleMenu", "&rfMenu", "&nrf24Menu", "&irMenu", "&fileMenu", "&scriptsMenu", "&configMenu")
print("PASS SKB integration source checks. Compilation, browser behaviour and hardware still require independent verification.")
