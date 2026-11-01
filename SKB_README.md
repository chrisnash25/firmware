# SKB headless Bruce source — experimental build

Source archive derived from the user-uploaded `firmware-main.zip` (nested in `Files (1).zip`). Original Bruce licence and attribution preserved.

## Current implementation
- NVS-stored random 20-character AP password generated at first boot; printed to USB serial at 115200 baud. **USB serial access is required to retrieve it.**
- SSID `SKB-Headless-Pro`, IP `192.168.4.1`.
- `/skb` dashboard behind existing Bruce WebUI authentication, linking to existing Bruce interface.
- GitHub Actions workflow for `m5stack-cplus2`.

## Critical limitations
- NOT COMPILED or hardware tested; treat as development source only.
- The first-boot AP password is displayed on USB serial, not through a public captive portal. Existing Bruce web UI credentials are separate and may need provisioning; check original Bruce settings before exposing the device.
- This is an initial dashboard, not the exact uploaded SKB logo/embossed artwork integration.
- Existing Bruce Wi-Fi tools may stop the AP. Automatic recovery is not implemented.
- Before flashing, verify firmware layout, partition offsets, bootloader, and build artifact compatibility.

## Build
Upload the contents of this directory to the `skb-dev` branch, then Actions -> SKB M5StickC Plus2 -> Run workflow. Do not upload the ZIP itself as a source file. Prefer a Git client capable of preserving the full directory tree.
