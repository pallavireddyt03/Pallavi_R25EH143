#ifndef SHAPES_H
#define SHAPES_H

#define ROWS 20
#define COLS 60
#define MAX_SHAPES 100

// Enumeration for supported shape types
typedef enum {
    SHAPE_LINE,
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE,
    SHAPE_TRIANGLE
} ShapeType;

// Structure parameters for a Line
typedef struct {
    int x1, y1;
    int x2, y2;
} LineParams;

// Structure parameters for a Rectangle
typedef struct {
    int x, y;
    int width, height;
} RectParams;

// Structure parameters for a Circle
typedef struct {
    int cx, cy;
    int radius;
} CircleParams;

// Structure parameters for a Triangle
typedef struct {
    int x1, y1;
    int x2, y2;
    int x3, y3;
} TriangleParams;

// Unified shape structure
typedef struct {
    ShapeType type;
    union {
        LineParams line;
        RectParams rect;
        CircleParams circle;
        TriangleParams triangle;
    } data;
} Shape;

// Canvas operations
void init_canvas(char canvas[ROWS][COLS]);
void display_canvas(const char canvas[ROWS][COLS]);
void render_shapes(char canvas[ROWS][COLS], const Shape shapes[], int count);

// Drawing functions
void draw_line(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2);
void draw_rectangle(char canvas[ROWS][COLS], int x, int y, int w, int h);
void draw_circle(char canvas[ROWS][COLS], int cx, int cy, int r);
void draw_triangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2, int x3, int y3);

#endif // SHAPES_H
