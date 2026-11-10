# Camera integration research

The SPF Framework Camera API documents switching the game's active camera among supported types (for example interior, behind, bumper, window and top-down). It also exposes camera settings. It does not document a public API for acquiring simultaneous independent rendered camera textures for an external overlay. Therefore the current desktop panel remains an explicitly labelled preview; do not present it as live game footage.

Next engineering investigation: build a separate SPF Framework plugin against the framework's published headers and verify version compatibility with ETS2 1.62. Any active-camera switching must be opt-in because it changes the player's actual in-game view. Multi-camera capture requires a separately verified rendering/capture path.
