# Real Camera and Proximity Feasibility (ETS2 1.62)

Research checked against SPF Framework v1.2.7 public API (2026-11-10).

## Confirmed public capabilities

- SPF plugin lifecycle exposes OnActivated and OnUpdate.
- SPF Camera API exposes Cam_GetCurrentCamera and Cam_SwitchTo, plus settings for the game's active camera types (interior, behind, bumper, window, top-down, etc.).
- SPF Framework documents a graphics/UI layer that supports DirectX 11, DirectX 12 and OpenGL and a Dear ImGui-based in-game UI.
- SPF TelemetryData exposes player truck speed, RPM, signed gear, left/right blinkers, lights and world placement.
- SPF Vehicle API exposes traffic vehicle handles and properties such as IDs and speeds.

## What the public APIs do not establish

- No documented API returns multiple independent rendered camera textures or frame buffers for front/left/right/rear to a separate desktop window.
- Cam_SwitchTo changes the game's active camera. It does not create an extra camera feed while preserving the driver's current view.
- The documented Vehicle API does not expose public world-position/placement getters for arbitrary traffic handles. It therefore does not currently support reliable left/right distance-to-neighbour calculations using only the documented public API.

## Consequence for the Smart View design

Do not replace the external overlay with an in-game SPF window; the user explicitly locked the standalone movable monitor design. Do not label vector drawings or a single active game camera as simultaneous live multi-camera video.

## Engineering next steps

1. Keep the existing SCS telemetry bridge and desktop overlay stable.
2. Build an SPF-compatible diagnostic plugin against the v1.2.7 API to verify camera service readiness and current camera type in ETS2 1.62.
3. Investigate a supported renderer capture path for independent views. This may require a separate rendering integration and must be tested in-game before promising multi-feed capture.
4. Investigate whether SPF maintainers expose traffic actor positions through a supported API; do not read undocumented raw addresses.
5. Only after a verified data source exists, implement side-distance warnings and beeps.

## Sources

- SPF Framework: https://github.com/TrackAndTruckDevs/SPF-Framework
- SPF Camera API: https://github.com/TrackAndTruckDevs/SPF-Framework/blob/main/include/SPF/SPF_API/SPF_Camera_API.h
- SPF Telemetry data: https://github.com/TrackAndTruckDevs/SPF-Framework/blob/main/include/SPF/SPF_API/SPF_TelemetryData.h
- SPF Vehicle API: https://github.com/TrackAndTruckDevs/SPF-Framework/blob/main/include/SPF/SPF_API/SPF_Vehicle_API.h
- SPF Framework release v1.2.7: https://github.com/TrackAndTruckDevs/SPF-Framework/releases/tag/v1.2.7
