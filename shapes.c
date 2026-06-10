#include "shapes.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize canvas with underscores
void init_canvas(char canvas[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            canvas[r][c] = '_';
        }
    }
}

// Display canvas with helpful row and column headers/rulers
void display_canvas(const char canvas[ROWS][COLS]) {
    // Print the tens line for columns
    printf("    ");
    for (int c = 0; c < COLS; c++) {
        if (c % 10 == 0) {
            printf("%d", c / 10);
        } else {
            printf(" ");
        }
    }
    printf("\n");

    // Print the units line for columns
    printf("    ");
    for (int c = 0; c < COLS; c++) {
        printf("%d", c % 10);
    }
    printf("\n");

    // Print top boundary
    printf("   +");
    for (int c = 0; c < COLS; c++) {
        printf("-");
    }
    printf("+\n");

    // Print rows with row indices
    for (int r = 0; r < ROWS; r++) {
        printf("%2d |", r);
        for (int c = 0; c < COLS; c++) {
            printf("%c", canvas[r][c]);
        }
        printf("|\n");
    }

    // Print bottom boundary
    printf("   +");
    for (int c = 0; c < COLS; c++) {
        printf("-");
    }
    printf("+\n");
}

// Clear the canvas and redraw all active shapes in order
void render_shapes(char canvas[ROWS][COLS], const Shape shapes[], int count) {
    init_canvas(canvas);
    for (int i = 0; i < count; i++) {
        switch (shapes[i].type) {
            case SHAPE_LINE:
                draw_line(canvas, shapes[i].data.line.x1, shapes[i].data.line.y1,
                          shapes[i].data.line.x2, shapes[i].data.line.y2);
                break;
            case SHAPE_RECTANGLE:
                draw_rectangle(canvas, shapes[i].data.rect.x, shapes[i].data.rect.y,
                               shapes[i].data.rect.width, shapes[i].data.rect.height);
                break;
            case SHAPE_CIRCLE:
                draw_circle(canvas, shapes[i].data.circle.cx, shapes[i].data.circle.cy,
                            shapes[i].data.circle.radius);
                break;
            case SHAPE_TRIANGLE:
                draw_triangle(canvas, shapes[i].data.triangle.x1, shapes[i].data.triangle.y1,
                              shapes[i].data.triangle.x2, shapes[i].data.triangle.y2,
                              shapes[i].data.triangle.x3, shapes[i].data.triangle.y3);
                break;
        }
    }
}

// Bresenham's Line Algorithm
void draw_line(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = -abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;

    while (1) {
        // Clipper to keep coordinates inside the 2D array canvas
        if (x1 >= 0 && x1 < COLS && y1 >= 0 && y1 < ROWS) {
            canvas[y1][x1] = '*';
        }
        if (x1 == x2 && y1 == y2) {
            break;
        }
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Draw boundary of a rectangle
void draw_rectangle(char canvas[ROWS][COLS], int x, int y, int w, int h) {
    if (w <= 0 || h <= 0) {
        return;
    }
    // Draw top and bottom edges
    for (int col = x; col < x + w; col++) {
        if (col >= 0 && col < COLS) {
            if (y >= 0 && y < ROWS) {
                canvas[y][col] = '*';
            }
            if (y + h - 1 >= 0 && y + h - 1 < ROWS) {
                canvas[y + h - 1][col] = '*';
            }
        }
    }
    // Draw left and right edges
    for (int row = y; row < y + h; row++) {
        if (row >= 0 && row < ROWS) {
            if (x >= 0 && x < COLS) {
                canvas[row][x] = '*';
            }
            if (x + w - 1 >= 0 && x + w - 1 < COLS) {
                canvas[row][x + w - 1] = '*';
            }
        }
    }
}

// Midpoint Circle Algorithm (Bresenham's Circle)
void draw_circle(char canvas[ROWS][COLS], int cx, int cy, int r) {
    if (r < 0) {
        return;
    }
    
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    // Plot helper macro for 8-way symmetry with clipping check
    #define PLOT_POINT(px, py) do { \
        if ((px) >= 0 && (px) < COLS && (py) >= 0 && (py) < ROWS) { \
            canvas[py][px] = '*'; \
        } \
    } while(0)

    // Plot initial points
    PLOT_POINT(cx + x, cy + y);
    PLOT_POINT(cx - x, cy + y);
    PLOT_POINT(cx + x, cy - y);
    PLOT_POINT(cx - x, cy - y);
    PLOT_POINT(cx + y, cy + x);
    PLOT_POINT(cx - y, cy + x);
    PLOT_POINT(cx + y, cy - x);
    PLOT_POINT(cx - y, cy - x);

    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        PLOT_POINT(cx + x, cy + y);
        PLOT_POINT(cx - x, cy + y);
        PLOT_POINT(cx + x, cy - y);
        PLOT_POINT(cx - x, cy - y);
        PLOT_POINT(cx + y, cy + x);
        PLOT_POINT(cx - y, cy + x);
        PLOT_POINT(cx + y, cy - x);
        PLOT_POINT(cx - y, cy - x);
    }
    #undef PLOT_POINT
}

// Draw a triangle by drawing lines between its three vertices
void draw_triangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(canvas, x1, y1, x2, y2);
    draw_line(canvas, x2, y2, x3, y3);
    draw_line(canvas, x3, y3, x1, y1);
}
