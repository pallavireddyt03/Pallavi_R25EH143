# 2D Graphics Editor in C

A menu-driven, text-based 2D Graphics Editor written in C. It uses a 2D character array as a drawing canvas, with `_` representing empty space and `*` representing drawn shapes. It supports adding, deleting, and modifying shapes dynamically.

## Features

- **Vector-List State Management**: Retains a list of shapes to support robust shape modification and deletion without raster information loss.
- **Robust Input Handling**: Prevents terminal/menu loops when users enter letters or invalid coordinates.
- **Shape Drawing Algorithms**:
  - **Lines**: Rendered using Bresenham's Line Algorithm.
  - **Rectangles**: Rendered as wireframe outlines using boundaries.
  - **Circles**: Rendered using the Midpoint (Bresenham) Circle Algorithm.
  - **Triangles**: Rendered by connecting three line segments.
- **Bounds Clipping**: Prevents array index out-of-bounds crashes when drawing shapes that are partially off-screen.
- **Ruler Coordinates**: Displays vertical and horizontal rulers to make coordinate input easy.

---

## File Structure

- [main.c](file:///c:/Users/Admin/Desktop/c%20program/main.c): Main program loop, interactive menu, and user input validation.
- [shapes.c](file:///c:/Users/Admin/Desktop/c%20program/shapes.c): Graphics logic, canvas rendering, and drawing algorithms.
- [shapes.h](file:///c:/Users/Admin/Desktop/c%20program/shapes.h): Data structures, type definitions, and function declarations.

---

## How to Compile and Run

If you have a C compiler (like `gcc` or `clang`) installed on your system, you can compile the editor using:

```bash
gcc -Wall -Wextra main.c shapes.c -o editor
```

To run the program:
- **Windows**:
  ```cmd
  editor.exe
  ```
- **macOS / Linux**:
  ```bash
  ./editor
  ```

---

## How to Use

1. **Add Shape**: Select `1` from the main menu, then choose the shape type:
   - **Line**: Enter start coordinates `(x1, y1)` and end coordinates `(x2, y2)`.
   - **Rectangle**: Enter top-left `(x, y)`, `width`, and `height`.
   - **Circle**: Enter center `(cx, cy)` and `radius`.
   - **Triangle**: Enter three coordinates `(x1, y1)`, `(x2, y2)`, and `(x3, y3)`.
2. **Delete Shape**: Select `2` from the main menu, view the active shapes list, and enter the index of the shape you wish to remove.
3. **Modify Shape**: Select `3` from the main menu, select the index of the shape to edit, and input its new coordinates/dimensions.
4. **Display Canvas**: Select `4` to reprint the canvas and list all active shapes with their coordinates.
5. **Clear Canvas**: Select `5` to remove all shapes and reset the canvas.
