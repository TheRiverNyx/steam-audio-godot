# Project Guidelines

## Project overview
SteamAudioGodot is a Godot 4 GDExtension that integrates Valve’s Steam Audio SDK to provide physically based spatial audio (HRTF, occlusion, reflections, ray‑traced propagation). The repository builds a shared native library that Godot loads via a `.gdextension` manifest located under the demo addon.

- Language/tech: C++17, CMake, Godot 4 GDExtension, Steam Audio SDK, `godot-cpp` bindings
- Primary library target: `SteamAudioGodot` (shared library)
- Bindings target: `godot-cpp`
- Helper targets: `templates`, `generate_bindings`
- Entry point symbol: `steam_audio_library_init` (see `src/register_types.cpp`)
- Example registered classes: `SteamAudio`, `SteamAudioListener`, `SteamAudioSource`, `SteamAudioStaticMesh`, `SteamAudioDynamicMesh`, server singleton `SteamAudioServer`

### Repository layout (high level)
- `src/` — C++ sources for the GDExtension
- `extern/godot-cpp/` — Godot C++ bindings (submodule)
- `extern/steam-audio/` — Steam Audio SDK
- `support_files/lib/<platform>/` — Prebuilt runtime libs staged next to the built extension
- `demo/` — Example Godot project with the addon under `addons/SteamAudioGodot`
- `templates/` — CMake templates and supplemental files
- `Writerside/` — Documentation sources (JetBrains Writerside)

See also the detailed overview in `README.md` (sections: Overview, Building, Installing into a Godot project, Project structure).

## How Junie should build and verify
- Use the existing CLion CMake profile only.
  - Active profile: `Debug`
  - Build directory: `cmake-build-debug`
  - Toolchain: local MinGW
- When a build is required, prefer building a specific target:
  - Main target: `SteamAudioGodot`
  - Example command (run via CLion profile environment): `cmake --build cmake-build-debug --target SteamAudioGodot`
  - In this environment, always use the special CMake-profile execution tool when issuing build commands (not raw shell).
- Do not create new CMake build directories or presets unless explicitly requested.
- For documentation‑only or comments‑only changes: do not build.
- For native code or CMake changes: build `SteamAudioGodot` in Debug to smoke‑check.

### Build outputs (typical)
- Artifacts are written under the CMake build directory, e.g.: `cmake-build-debug/SteamAudioGodot/lib/<Platform-Arch>/`
- Runtime Steam Audio binaries are staged next to the built library after build.

## Tests policy
- There are currently no automated tests in this repository.
- Do not create or run tests unless explicitly requested for a task.

## Code style and contribution guidance
- Follow existing file/module style: indentation, naming, include order, and comments.
- C++ standard: C++17.
- Keep changes minimal and localized; avoid sweeping refactors unless requested.
- When renaming any code element (class/function/variable), use the project’s rename mechanism that updates all references consistently.
- Match Windows path separators (`\`) in paths and adapt shell commands to PowerShell when needed.

## Quick reference: CMake targets
- `SteamAudioGodot` — main shared library (GDExtension)
- `godot-cpp` — bindings library (dependency)
- `generate_bindings` — bindings generator (from `godot-cpp`)
- `templates` — installs helper files/templates

## When to run the application
- This repository produces a library to be loaded by Godot. Running the editor/project is generally outside the scope of routine CI checks here.
- To try the addon, open the `demo/` folder in Godot 4 after building and ensure the addon is present under `demo/addons/SteamAudioGodot`.

## Troubleshooting pointers
- Missing `godot-cpp` submodule: run `git submodule update --init --recursive`.
- Godot cannot load the extension: verify the `.gdextension` file library paths and that required runtime DLLs/SOs exist under `addons/SteamAudioGodot/lib/<Platform-Arch>/`.

## External API Documentation
- Steam Audio SDK: https://valvesoftware.github.io/steam-audio/doc/capi/index.html
- Godot GDExtension: https://docs.godotengine.org/en/stable