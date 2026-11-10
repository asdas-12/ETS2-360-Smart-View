# ETS2 360 Smart View

Development repository for the ETS2 360 Smart View companion application and telemetry bridge.

## Current milestone
The first bridge reads truck speed, engine RPM and gear from the SCS Telemetry SDK and writes:
`Documents/Euro Truck Simulator 2/360smartview_telemetry.json`

## Build
GitHub Actions downloads the official SCS Telemetry SDK 1.16 archive during the Windows x64 build, extracts its headers, compiles the plugin, and publishes the DLL as a workflow artifact.

## Scope and limitations
Telemetry provides vehicle state; it does not provide rendered camera feeds or obstacle distances. Those capabilities need a separate investigation and implementation. The initial bridge is not yet compiled or runtime-tested in ETS2 1.62.
