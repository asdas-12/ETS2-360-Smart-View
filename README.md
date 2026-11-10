# ETS2 360 Smart View

A Windows companion overlay and SCS Telemetry SDK plugin for Euro Truck Simulator 2.

## Current build 1.2
- Reads live speed, engine RPM, signed gear, pause state and left/right indicator state.
- The companion overlay automatically selects the corresponding FRONT / LEFT / RIGHT / REAR preview from telemetry.
- Reverse detection uses negative SCS gear values; indicator states use the SCS logical blinker channels.
- Movable/resizable overlay with saved window geometry and the locked Scania layout.

## Install
1. Install `smartview_telemetry.dll` into `Euro Truck Simulator 2/bin/win_x64/plugins`.
2. Launch ETS2 and enter a profile.
3. Run `RUN_SMART_VIEW.bat` from the companion app folder.
4. The plugin writes `Documents/Euro Truck Simulator 2/360smartview_telemetry.json`.

## Camera limitation
The right-side camera panel currently shows a labelled preview, not an independent rendered ETS2 camera feed. The SCS telemetry SDK supplies vehicle state, not rendered images or obstacle distances. SPF camera control can switch the game's active camera, but that is not the same as rendering simultaneous independent camera feeds. Real multi-view rendering and object-distance alerts remain separate development work and are not claimed as complete.
