# Real Camera Feeds — ETS2 1.62 Implementation Route

Research date: 2026-11-10.

## Breakthrough: ETS2 can render multiple live camera viewports in one game window

The official SCS custom multi-monitor mode (r_multimon_mode 4) supports up to four independently configured view regions in one virtual display. Each region has its own heading_offset, pitch_offset, FOV overrides, and render_interior/render_exterior flags.

This gives Smart View a viable route to simultaneous views without relying on SPF to create multiple camera render targets:

- Main viewport: FRONT / driving view.
- PiP viewport 1: LEFT, with heading offset to the driver's left.
- PiP viewport 2: RIGHT, with heading offset to the driver's right.
- PiP viewport 3: REAR, with a rear-facing heading offset.

The game renders the views simultaneously into their configured screen rectangles. The standalone Smart View app can then capture those rectangles from the ETS2 game window and display the selected view in its right-hand panel. The locked left bird's-eye panel can remain the existing top-down truck/trailer visualization.

## Official limitations and prerequisites

- SCS custom multimon mode supports up to four view regions.
- Official instructions describe a virtual display surface and normalized viewport rectangles.
- TruckDeck-Multimon demonstrates a practical PiP-on-main layout: primary view plus independently oriented, free-placed left/right/mirror viewports on the same physical display.
- This approach does not make four separate game windows. All four views occupy regions of the same ETS2 window.
- The app must capture the ETS2 window without capturing its own overlay window. Ordinary desktop screenshot capture can include the Smart View overlay itself, so target-window capture is required.
- Exact positions, sizes, heading offsets and FOV values must be calibrated against the user's display and driving side, then tested in ETS2 1.62.

## Proposed Smart View mapping

| Game viewport | Purpose | Smart View behavior |
|---|---|---|
| Center | Front / road | Default |
| Left PiP | Left side view | Left indicator |
| Right PiP | Right side view | Right indicator |
| Rear PiP | Rear view | Reverse gear |

The three PiP regions can be placed at screen edges. Smart View samples each region and displays only the selected one in its existing right-side camera panel. The app's locked layout is not redesigned.

## Current verified build state

- smartview_telemetry.dll successfully compiled with SCS Telemetry SDK 1.16 in GitHub Actions, run #8.
- The DLL reads speed, RPM, gear, pause state, and left/right blinkers to a JSON file.
- It is not a camera-capture DLL and does not provide camera frames itself.
- Real independent views are provided by the game's multi-monitor renderer; a separate capture path is still required to transfer those pixels into the standalone app.
- Side-object proximity warnings remain unimplemented. Traffic position/distance must come from a supported data source before they can be claimed as real.

## Sources

- SCS official multi-monitor configuration: https://eurotrucksimulator2.com/multimon_config.php
- TruckDeck-Multimon (PiP-on-main, up to four viewports): https://github.com/dawood-shfuk/TruckDeck-Multimon
- TruckDeck's PiP layout implementation: https://github.com/dawood-shfuk/TruckDeck-Multimon/blob/main/src/TruckDeck.Multimon/Services/MainPipLayoutService.cs
- SPF Camera API (active camera control, not simultaneous independent camera render targets): https://github.com/TrackAndTruckDevs/SPF-Framework/blob/main/docs/api/SPF_Camera_API.md
