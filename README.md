# Canny Edge Detector — From Scratch in C++

> A full reimplementation of the Canny Edge Detection pipeline, built without OpenCV or any external image-processing dependency.

---

## Overview

Most edge-detection code reduces to a single call:

```cpp
cv::Canny(src, dst, low, high);
```

This project does the opposite — every stage of the algorithm is implemented by hand, from convolution kernels to BFS-based hysteresis. The result is a modular, dependency-free C++ library suitable for embedded systems, FPGA integration, and anyone who wants to understand what actually happens inside an edge detector.

---

## Features

| Stage | Implementation |
|---|---|
| Noise Reduction | 3×3 Gaussian blur with configurable sigma |
| Gradient Computation | Scharr operator (superior isotropy vs. Sobel) |
| Edge Thinning | Non-maximum suppression over 4 quantized directions |
| Edge Classification | Double thresholding (strong / weak / non-edge) |
| Edge Connectivity | Hysteresis via BFS / DFS traversal |
| Edge Refinement | Morphological dilation and erosion |
| I/O | STB-based PNG and JPG read/write |

No OpenCV. No Boost. No external image libraries beyond STB header utilities.

---

## Project Structure

```
edge-detector-cpp/
│
├── Gussian/                   # Gaussian blur kernel
├── sobel/                     # Scharr gradient computation
├── non_maximum_suppression/   # NMS — directional edge thinning
├── Threshold/                 # Double thresholding
├── hysteresis/                # BFS/DFS edge connectivity
├── morphology/                # Dilation and erosion
├── stb/                       # Image I/O utilities (STB)
│
├── image/                     # Input images
├── results/                   # Output images
├── training_data/             # Per-stage CSV dumps for analysis
├── build/                     # Compiled objects and static library
│
├── main.cpp
├── Dockerfile
└── README.md
```

---

## Quick Start

### Option 1 — Docker (Recommended)

Docker eliminates all compiler and dependency configuration.

```bash
# Build the image (one-time)
docker build -t edge-detector .

# Run on your image
docker run -v ${PWD}:/data edge-detector /data/image/input.jpg /data/results/output.png
```

`${PWD}` maps your local project directory into `/data` inside the container.

---

### Option 2 — Manual Compilation

**Requirements:** GCC/G++ compiler, GNU `ar`

**Step 1 — Compile all modules**

```bash
g++ -c stb/im_mat.cpp              -o build/im_mat.o
g++ -c stb/mat_im.cpp              -o build/mat_im.o
g++ -c Gussian/gussian.cpp         -o build/gussian.o
g++ -c sobel/sobel.cpp             -o build/sobel.o
g++ -c non_maximum_suppression/nms.cpp -o build/nms.o
g++ -c Threshold/threshold.cpp     -o build/threshold.o
g++ -c hysteresis/hy.cpp           -o build/hy.o
g++ -c morphology/dilation.cpp     -o build/dilation.o
g++ -c morphology/erosion.cpp      -o build/erosion.o
```

**Step 2 — Build the static library**

```bash
ar rcs build/libedge.a \
    build/im_mat.o    \
    build/mat_im.o    \
    build/gussian.o   \
    build/sobel.o     \
    build/nms.o       \
    build/threshold.o \
    build/hy.o        \
    build/dilation.o  \
    build/erosion.o
```

**Step 3 — Link and compile**

```bash
g++ main.cpp build/libedge.a -o edge_app
```

**Step 4 — Run**

```bash
./edge_app image/input.jpg results/output.png
```

---

## Algorithm Pipeline

```
Input Image
    │
    ▼
┌─────────────────────┐
│   Gaussian Blur     │  Suppress noise with a 3×3 kernel before differentiation
└─────────┬───────────┘
          │
    ▼
┌─────────────────────┐
│ Gradient (Scharr)   │  Compute ∂I/∂x and ∂I/∂y; derive magnitude and direction
└─────────┬───────────┘
          │
    ▼
┌─────────────────────┐
│ Non-Max Suppression │  Retain only local gradient maxima along edge direction
└─────────┬───────────┘
          │
    ▼
┌─────────────────────┐
│ Double Thresholding │  Classify pixels: strong edge / weak edge / suppressed
└─────────┬───────────┘
          │
    ▼
┌─────────────────────┐
│    Hysteresis       │  Promote weak edges connected to strong edges (BFS/DFS)
└─────────┬───────────┘
          │
    ▼
┌─────────────────────┐
│  Morphological Ops  │  Dilation then erosion to close gaps and remove noise
└─────────┬───────────┘
          │
    ▼
Output Edge Map
```

---

## Technical Reference

### Gaussian Blur
- **Kernel:** 3×3 separable Gaussian
- **Sigma:** Configurable (default: 1.0)
- **Purpose:** Reduces high-frequency noise that would otherwise produce false gradients

### Gradient Computation — Scharr Operator
- **Why Scharr over Sobel:** Better rotational symmetry, more accurate gradient direction at 45° angles
- **Output:** Per-pixel magnitude `G = √(Gx² + Gy²)` and quantized direction θ ∈ {0°, 45°, 90°, 135°}

### Non-Maximum Suppression
- For each pixel, compare gradient magnitude against the two neighbors along the gradient direction
- Suppress (zero out) the pixel if it is not a local maximum
- Result: single-pixel-wide edge candidates

### Double Thresholding
- **High threshold:** Pixels above this are classified as *strong edges*
- **Low threshold:** Pixels between low and high are *weak edge candidates*
- **Typical ratio:** 2:1 to 3:1 (high:low)
- Pixels below the low threshold are discarded

### Hysteresis
- Starting from every strong-edge pixel, traverse the graph of connected weak-edge pixels
- A weak edge is kept if it is reachable from a strong edge (BFS or DFS)
- Disconnected weak edges are suppressed
- Produces complete, continuous edge contours

### Morphological Operations
- **Dilation:** Expands edge regions — closes small gaps in contours
- **Erosion:** Shrinks edge regions — removes thin noise spurs introduced by dilation
- Applied sequentially to refine the final edge map

---

## Intermediate Output (Training Data)

Each pipeline stage writes its output to `training_data/` as a CSV file. This is useful for:

- Visualizing what each transformation does to the image matrix
- Debugging unexpected edges or missing contours
- Comparing intermediate results against reference implementations
- Building intuition for threshold and sigma tuning

---

## Troubleshooting

**`build/` directory not found**
```bash
mkdir -p build
```

**Compiler not found**
```bash
g++ --version   # Should be GCC 9+ for full C++17 support
```

**Unsupported image format**
Supported formats: JPEG, PNG. Ensure the input file extension matches its actual encoding.

**Large images cause slow processing**
The Gaussian and gradient stages are O(n) per pixel with no SIMD optimization in the base implementation. For large inputs, consider downscaling before processing.

---

## License

MIT License — Copyright (c) 2025 Adeeb Ali

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.

---

## Contact

**Email:** aliadee2003@gmail.com  
**Issues:** Open a GitHub issue for bugs, questions, or feature requests

---

## Acknowledgements

- **John F. Canny** — *A Computational Approach to Edge Detection* (1986), IEEE TPAMI
- **STB** — Sean Barrett's single-header image I/O libraries
- The broader signal processing community whose open explanations made this possible
