#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shapes.h"

// Robust helper function to read an integer and handle invalid inputs without infinite loops
int get_int(const char *prompt, int *value) {
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0; // End-of-file (EOF) or reading error
        }
        
        // Remove trailing newline character
        buffer[strcspn(buffer, "\n")] = 0;
        
        // Trim leading spaces
        char *start = buffer;
        while (*start == ' ' || *start == '\t') {
            start++;
        }
        
        if (*start == '\0') {
            printf("Error: Input cannot be empty. Please enter an integer.\n");
            continue;
        }
        
        char *endptr;
        long val = strtol(start, &endptr, 10);
        
        // Trim trailing spaces after parsed number
        while (*endptr == ' ' || *endptr == '\t') {
            endptr++;
        }
        
        // Check if there are unparsed trailing characters
        if (*endptr != '\0') {
            printf("Error: Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        *value = (int)val;
        return 1;
    }
}

// Print detailed text description of a shape
void print_shape_info(const Shape *shape, int index) {
    printf("%2d. ", index + 1);
    switch (shape->type) {
        case SHAPE_LINE:
            printf("[Line] from (%d, %d) to (%d, %d)\n",
                   shape->data.line.x1, shape->data.line.y1,
                   shape->data.line.x2, shape->data.line.y2);
            break;
        case SHAPE_RECTANGLE:
            printf("[Rectangle] top-left: (%d, %d), width: %d, height: %d\n",
                   shape->data.rect.x, shape->data.rect.y,
                   shape->data.rect.width, shape->data.rect.height);
            break;
        case SHAPE_CIRCLE:
            printf("[Circle] center: (%d, %d), radius: %d\n",
                   shape->data.circle.cx, shape->data.circle.cy,
                   shape->data.circle.radius);
            break;
        case SHAPE_TRIANGLE:
            printf("[Triangle] vertices: (%d, %d), (%d, %d), (%d, %d)\n",
                   shape->data.triangle.x1, shape->data.triangle.y1,
                   shape->data.triangle.x2, shape->data.triangle.y2,
                   shape->data.triangle.x3, shape->data.triangle.y3);
            break;
    }
}

// Sub-menu for adding a shape
void add_shape_menu(Shape shapes[], int *count, char canvas[ROWS][COLS]) {
    if (*count >= MAX_SHAPES) {
        printf("Error: Maximum shape limit reached (%d shapes).\n", MAX_SHAPES);
        return;
    }

    printf("\n--- Add a New Shape ---\n");
    printf("1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Circle\n");
    printf("4. Triangle\n");
    printf("5. Cancel (Back to Main Menu)\n");
    
    int choice;
    if (!get_int("Enter selection (1-5): ", &choice)) {
        return;
    }
    
    if (choice < 1 || choice > 5) {
        printf("Error: Invalid selection.\n");
        return;
    }
    if (choice == 5) {
        return;
    }
    
    Shape new_shape;
    switch (choice) {
        case 1: // Line
            new_shape.type = SHAPE_LINE;
            printf("Enter line parameters (Canvas range X: 0-%d, Y: 0-%d):\n", COLS - 1, ROWS - 1);
            if (!get_int("  x1: ", &new_shape.data.line.x1)) return;
            if (!get_int("  y1: ", &new_shape.data.line.y1)) return;
            if (!get_int("  x2: ", &new_shape.data.line.x2)) return;
            if (!get_int("  y2: ", &new_shape.data.line.y2)) return;
            break;
            
        case 2: // Rectangle
            new_shape.type = SHAPE_RECTANGLE;
            printf("Enter rectangle parameters (Canvas range X: 0-%d, Y: 0-%d):\n", COLS - 1, ROWS - 1);
            if (!get_int("  Top-left x: ", &new_shape.data.rect.x)) return;
            if (!get_int("  Top-left y: ", &new_shape.data.rect.y)) return;
            if (!get_int("  Width (positive): ", &new_shape.data.rect.width)) return;
            if (!get_int("  Height (positive): ", &new_shape.data.rect.height)) return;
            if (new_shape.data.rect.width <= 0 || new_shape.data.rect.height <= 0) {
                printf("Error: Width and Height must be positive integers.\n");
                return;
            }
            break;
            
        case 3: // Circle
            new_shape.type = SHAPE_CIRCLE;
            printf("Enter circle parameters (Canvas range X: 0-%d, Y: 0-%d):\n", COLS - 1, ROWS - 1);
            if (!get_int("  Center x: ", &new_shape.data.circle.cx)) return;
            if (!get_int("  Center y: ", &new_shape.data.circle.cy)) return;
            if (!get_int("  Radius (non-negative): ", &new_shape.data.circle.radius)) return;
            if (new_shape.data.circle.radius < 0) {
                printf("Error: Radius must be non-negative.\n");
                return;
            }
            break;
            
        case 4: // Triangle
            new_shape.type = SHAPE_TRIANGLE;
            printf("Enter triangle vertices (Canvas range X: 0-%d, Y: 0-%d):\n", COLS - 1, ROWS - 1);
            if (!get_int("  x1: ", &new_shape.data.triangle.x1)) return;
            if (!get_int("  y1: ", &new_shape.data.triangle.y1)) return;
            if (!get_int("  x2: ", &new_shape.data.triangle.x2)) return;
            if (!get_int("  y2: ", &new_shape.data.triangle.y2)) return;
            if (!get_int("  x3: ", &new_shape.data.triangle.x3)) return;
            if (!get_int("  y3: ", &new_shape.data.triangle.y3)) return;
            break;
    }
    
    shapes[*count] = new_shape;
    (*count)++;
    printf("Shape added successfully!\n");
    
    // Automatically redraw and display canvas
    render_shapes(canvas, shapes, *count);
    display_canvas(canvas);
}

// Sub-menu for deleting a shape
void delete_shape_menu(Shape shapes[], int *count, char canvas[ROWS][COLS]) {
    if (*count == 0) {
        printf("\nNo shapes in the picture to delete.\n");
        return;
    }
    
    printf("\n--- Select a Shape to Delete ---\n");
    for (int i = 0; i < *count; i++) {
        print_shape_info(&shapes[i], i);
    }
    
    int index;
    if (!get_int("Enter the number of the shape to delete (or 0 to cancel): ", &index)) {
        return;
    }
    
    if (index == 0) {
        return;
    }
    if (index < 1 || index > *count) {
        printf("Error: Invalid shape selection.\n");
        return;
    }
    
    int delete_idx = index - 1;
    // Shift remaining shapes left
    for (int i = delete_idx; i < *count - 1; i++) {
        shapes[i] = shapes[i + 1];
    }
    (*count)--;
    printf("Shape deleted successfully!\n");
    
    // Automatically redraw and display canvas
    render_shapes(canvas, shapes, *count);
    display_canvas(canvas);
}

// Sub-menu for modifying a shape
void modify_shape_menu(Shape shapes[], int count, char canvas[ROWS][COLS]) {
    if (count == 0) {
        printf("\nNo shapes in the picture to modify.\n");
        return;
    }
    
    printf("\n--- Select a Shape to Modify ---\n");
    for (int i = 0; i < count; i++) {
        print_shape_info(&shapes[i], i);
    }
    
    int index;
    if (!get_int("Enter the number of the shape to modify (or 0 to cancel): ", &index)) {
        return;
    }
    
    if (index == 0) {
        return;
    }
    if (index < 1 || index > count) {
        printf("Error: Invalid shape selection.\n");
        return;
    }
    
    int modify_idx = index - 1;
    Shape *shape = &shapes[modify_idx];
    
    printf("\nModifying: ");
    print_shape_info(shape, modify_idx);
    
    switch (shape->type) {
        case SHAPE_LINE: {
            LineParams new_line;
            printf("Enter new coordinates:\n");
            if (!get_int("  new x1: ", &new_line.x1)) return;
            if (!get_int("  new y1: ", &new_line.y1)) return;
            if (!get_int("  new x2: ", &new_line.x2)) return;
            if (!get_int("  new y2: ", &new_line.y2)) return;
            shape->data.line = new_line;
            break;
        }
        case SHAPE_RECTANGLE: {
            RectParams new_rect;
            printf("Enter new coordinates and size:\n");
            if (!get_int("  new Top-left x: ", &new_rect.x)) return;
            if (!get_int("  new Top-left y: ", &new_rect.y)) return;
            if (!get_int("  new Width (positive): ", &new_rect.width)) return;
            if (!get_int("  new Height (positive): ", &new_rect.height)) return;
            if (new_rect.width <= 0 || new_rect.height <= 0) {
                printf("Error: Width and Height must be positive integers. Modification aborted.\n");
                return;
            }
            shape->data.rect = new_rect;
            break;
        }
        case SHAPE_CIRCLE: {
            CircleParams new_circle;
            printf("Enter new parameters:\n");
            if (!get_int("  new Center x: ", &new_circle.cx)) return;
            if (!get_int("  new Center y: ", &new_circle.cy)) return;
            if (!get_int("  new Radius (non-negative): ", &new_circle.radius)) return;
            if (new_circle.radius < 0) {
                printf("Error: Radius must be non-negative. Modification aborted.\n");
                return;
            }
            shape->data.circle = new_circle;
            break;
        }
        case SHAPE_TRIANGLE: {
            TriangleParams new_tri;
            printf("Enter new coordinates for the three vertices:\n");
            if (!get_int("  new x1: ", &new_tri.x1)) return;
            if (!get_int("  new y1: ", &new_tri.y1)) return;
            if (!get_int("  new x2: ", &new_tri.x2)) return;
            if (!get_int("  new y2: ", &new_tri.y2)) return;
            if (!get_int("  new x3: ", &new_tri.x3)) return;
            if (!get_int("  new y3: ", &new_tri.y3)) return;
            shape->data.triangle = new_tri;
            break;
        }
    }
    
    printf("Shape modified successfully!\n");
    
    // Automatically redraw and display canvas
    render_shapes(canvas, shapes, count);
    display_canvas(canvas);
}

int main() {
    char canvas[ROWS][COLS];
    Shape shapes[MAX_SHAPES];
    int shape_count = 0;

    init_canvas(canvas);

    printf("=========================================================\n");
    printf("              Welcome to the 2D Graphics Editor          \n");
    printf("=========================================================\n");
    printf("Canvas size: %d columns x %d rows.\n", COLS, ROWS);
    printf("Coordinates: (0,0) is top-left, x goes right, y goes down.\n");
    
    display_canvas(canvas);

    int running = 1;
    while (running) {
        printf("\n==================== Menu ====================\n");
        printf("1. Add shape (Line, Rectangle, Circle, Triangle)\n");
        printf("2. Delete shape\n");
        printf("3. Modify shape\n");
        printf("4. Display canvas and list shapes\n");
        printf("5. Clear canvas (remove all shapes)\n");
        printf("6. Exit\n");
        printf("==============================================\n");

        int choice;
        if (!get_int("Enter selection (1-6): ", &choice)) {
            printf("\nExiting. Goodbye!\n");
            break;
        }

        switch (choice) {
            case 1:
                add_shape_menu(shapes, &shape_count, canvas);
                break;
            case 2:
                delete_shape_menu(shapes, &shape_count, canvas);
                break;
            case 3:
                modify_shape_menu(shapes, shape_count, canvas);
                break;
            case 4:
                render_shapes(canvas, shapes, shape_count);
                printf("\n--- Current Drawing Canvas ---\n");
                display_canvas(canvas);
                printf("\n--- Active Shape List ---\n");
                if (shape_count == 0) {
                    printf("No shapes present.\n");
                } else {
                    for (int i = 0; i < shape_count; i++) {
                        print_shape_info(&shapes[i], i);
                    }
                }
                break;
            case 5:
                shape_count = 0;
                render_shapes(canvas, shapes, shape_count);
                printf("Canvas cleared successfully.\n");
                display_canvas(canvas);
                break;
            case 6:
                printf("\nExiting. Thank you for using 2D Graphics Editor!\n");
                running = 0;
                break;
            default:
                printf("Error: Invalid menu choice. Please select 1-6.\n");
                break;
        }
    }

    return 0;
}
