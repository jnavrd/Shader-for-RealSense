# customShadersRS

GPU-based depth visualization pipeline using Intel RealSense and GLSL.

Experimental C++ graphics pipeline.

---

## What it does

Depth frames are captured from a RealSense camera, converted to float32, uploaded to the GPU as single-channel textures, and visualized using fragment shaders. All visual interpretation of depth happens in the shader stage.

---

## Architecture

The codebase is split by responsibility. 
Capture handles RealSense interaction and depth scale retrieval.
Processing contains CPU-side conversions. Currently unused, kept for comparison and experimentation.
Rendering manages window creation, texture upload, and shader execution.
Shaders are currently loaded via hardcoded paths and live in a dedicated directory.

---

## Data flow

RealSense depth frame
    → uint16
    → float32
    → GPU texture (R32)
    → fragment shader
    → grayscale output

---

## Planned improvements
Migrating to pure OpenGL to gain full control over texture formats, reduce unnecessary conversions (uint16 to float32), and improve performance. 
Runtime shader selection and a dedicated shader management layer.

---

## Current shaders
### Grayscale 
pictured: Me holding my hand up </br>
<img width="620" height="400" alt="Pasted image 20260212194356" src="https://github.com/user-attachments/assets/491e248b-07a1-4c6b-8701-968844c53dfe" />

### Object + background detection
pictured: Me holding a hair clip </br>
<img width="620" height="400" alt="Pasted image 20260212195329" src="https://github.com/user-attachments/assets/d628503b-f1de-4566-a092-baa6aaec6681" />

### Depth change
pictured: Boots with highlighted depth transitions </br>
<img width="620" height="400" alt="Pasted image 20260212200611" src="https://github.com/user-attachments/assets/404957e1-ec78-4c28-9775-3bb57606dd9a" />

### Ripple effect + FBM (Feedback Loop)
(currently in renderer-refactor)
![IMG_0097](https://github.com/user-attachments/assets/797c1e5f-490f-4da5-a390-d1a696a097f6)

