# 🚀 Canny Edge Detector from Scratch (C++)

A **complete implementation of the Canny Edge Detection algorithm from scratch in C++**,  
built **without using OpenCV or any external image-processing library**.

This project focuses on **low-level image processing**, **algorithmic clarity**, and  
a **modular C++ build system**, making it suitable for **embedded systems and FPGA workflows**.

---

## ✨ Key Features

- ✅ Pure C++ implementation (no OpenCV)
- ✅ Gaussian Blur (noise reduction)
- ✅ Scharr-based gradient computation
- ✅ Gradient magnitude & direction
- ✅ Non-Maximum Suppression (edge thinning)
- ✅ Double Thresholding
- ✅ Hysteresis edge tracking
- ✅ Morphological refinement (dilation + erosion)
- ✅ Modular pipeline with static library build

---

## 🧠 Why this project?

Most edge-detection examples simply call:
```cpp
cv::Canny(...)
```

This project rebuilds the entire Canny pipeline from scratch, demonstrating a deep understanding of:
* Image matrices
* Convolution & filtering
* Gradient math
* Edge connectivity
* System-level C++ design

---

## 📁 Project Structure
```
Canny-Edge-Detector-using-c-/
│
├── gaussian/
├── sobel/
├── non_maximum_suppression/
├── threshold/
├── hysteresis/
├── morphology/
│
├── stb/                  
├── image/                
├── training_data/       
│
├── build/                
├── main.cpp
├── README.md
└── .gitignore
```

---

## 🛠️ Build Instructions

### Prerequisites
- GCC/G++ compiler
- Make (optional)
- Basic C++ development environment

### Step 1: Compile object files
```bash
g++ -c stb/im_mat.cpp -o build/im_mat.o
g++ -c stb/mat_im.cpp -o build/mat_im.o
g++ -c gaussian/gaussian.cpp -o build/gaussian.o
g++ -c sobel/sobel.cpp -o build/sobel.o
g++ -c non_maximum_suppression/nms.cpp -o build/nms.o
g++ -c threshold/threshold.cpp -o build/threshold.o
g++ -c hysteresis/hy.cpp -o build/hy.o
g++ -c morphology/dilation.cpp -o build/dilation.o
g++ -c morphology/erosion.cpp -o build/erosion.o
```

### Step 2: Create static library
```bash
ar rcs build/libedge.a \
    build/im_mat.o build/mat_im.o \
    build/gaussian.o build/sobel.o \
    build/nms.o build/threshold.o \
    build/hy.o build/dilation.o build/erosion.o
```

### Step 3: Link and build executable
```bash
g++ main.cpp build/libedge.a -o edge_app
```

### Step 4: Run
```bash
./edge_app
```

---

## 🎯 Algorithm Pipeline

The Canny Edge Detection algorithm follows these steps:

1. **Gaussian Blur** - Reduce noise using a 3 by 3 Gaussian kernel
2. **Gradient Calculation** - Compute intensity gradients using Scharr operators
3. **Non-Maximum Suppression** - Thin edges by suppressing non-peak gradients
4. **Double Thresholding** - Classify edges as strong, weak, or non-edges
5. **Hysteresis** - Connect weak edges to strong edges
6. **Morphological Operations** - Refine edges with dilation and erosion

---

## 📊 Training Data

Each pipeline stage outputs intermediate results to `training_data/` as CSV files for analysis and debugging.

This allows you to:
- Visualize intermediate processing steps
- Debug algorithm behavior
- Compare results with reference implementations
- Understand the transformation at each stage

---

## 🔧 Technical Details

### Gaussian Blur
- **Kernel Size**: 3×3
- **Sigma**: Configurable (default: 1)
- **Purpose**: Noise reduction before edge detection

### Gradient Computation
- **Method**: Scharr operator (improved Sobel)
- **Output**: Gradient magnitude and direction
- **Precision**: Floating-point calculations

### Non-Maximum Suppression
- **Algorithm**: Directional edge thinning
- **Angles**: 0°, 45°, 90°, 135° quantization
- **Result**: Single-pixel wide edges

### Double Thresholding
- **High Threshold**: Strong edge pixels
- **Low Threshold**: Weak edge candidates
- **Ratio**: Typically 2:1 or 3:1

### Hysteresis
- **Method**: Breadth-first search (BFS) or depth-first search (DFS)
- **Purpose**: Connect weak edges to strong edges
- **Result**: Complete edge contours

---

## 📸 Sample Results

Place your input and output images in the `image/` directory:
- `image/input.png` - Original image
- `image/output.png` - Detected edges

---

## 🐛 Troubleshooting

### Build errors
- Ensure `build/` directory exists: `mkdir -p build`
- Check GCC/G++ version: `g++ --version`
- Verify file paths match your directory structure

### Runtime errors
- Check input image exists in `image/` directory
- Verify image format is supported (PNG, JPG)
- Ensure sufficient memory for large images


## 📝 License

This project is open-source and available under the MIT License.
```
MIT License

Copyright (c) 2025 [Adeeb_Ali]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---



## 📧 Contact

For questions, suggestions, or feedback:
- Open an issue on GitHub
- Email: [aliadee2003@gmail.com]


---

## 🙏 Acknowledgments

- **John Canny** - For the original edge detection algorithm
- **STB Libraries** - For image I/O utilities
- **Community** - For various tutorials and resources



> "Understanding the fundamentals makes you a better engineer" 🚀
