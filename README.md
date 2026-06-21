# SparkFlowFX

A node based, procedural particle FX editor written in C++20.

## Architecture

SparkFlowFX has three layers. The central rule
is that **lower layers never depend on upper layers** — in particular the
simulation core has no dependency on the renderer, the UI, or OpenGL, so it
builds and runs headless and is fully testable on its own.

![SparkFlowFX high-level layer architecture](docs/images/architecture.png)

- **simulator** (static lib): headless simulation engine.
- **render** (static lib): OpenGL renderer. Both for the live viewport and captures (renders).
- **app** (executable): the executable that hosts the window and UI and orchestrates everything above.
