#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <time.h>

#define REQUIRED_POSITIONS 5
#define CAPTURE_INTERVAL_MS 50
#define THRESHOLD_DEGREES 45.0
#define PI 3.141592653589793

// Structure to hold mouse position
typedef struct {
    double x;
    double y;
    DWORD timestamp;
} MousePosition;

// Structure to hold vector information
typedef struct {
    double x;
    double y;
    double magnitude;
} Vector;

// Calculate Euclidean distance (magnitude of vector)
double calculate_distance(MousePosition p1, MousePosition p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double sum_square = dx * dx + dy * dy;
    return sqrt(sum_square);
}

// Calculate dot product of two vectors
double dot_product(Vector v1, Vector v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

// Calculate angle between two vectors formed by three consecutive points
double calculate_angle_degrees(MousePosition p0, MousePosition p1, MousePosition p2) {
    // Create vector v1: p1 - p0
    Vector v1;
    v1.x = p1.x - p0.x;
    v1.y = p1.y - p0.y;

    // Create vector v2: p2 - p1
    Vector v2;
    v2.x = p2.x - p1.x;
    v2.y = p2.y - p1.y;

    // Calculate magnitudes (Euclidean distance)
    double sum_square_v1 = v1.x * v1.x + v1.y * v1.y;
    double magnitude_v1 = sqrt(sum_square_v1);

    double sum_square_v2 = v2.x * v2.x + v2.y * v2.y;
    double magnitude_v2 = sqrt(sum_square_v2);

    // Avoid division by zero
    if (magnitude_v1 == 0.0 || magnitude_v2 == 0.0) {
        return 0.0;
    }

    // Calculate dot product
    double dot_prod = dot_product(v1, v2);

    // Calculate cosine of angle: dot_product / (mag1 * mag2)
    double cos_angle = dot_prod / (magnitude_v1 * magnitude_v2);

    // Clamp to [-1, 1] to handle floating point errors
    if (cos_angle > 1.0) cos_angle = 1.0;
    if (cos_angle < -1.0) cos_angle = -1.0;

    // Calculate angle in radians
    double angle_radians = acos(cos_angle);

    // Convert to degrees
    double angle_degrees = angle_radians * 180.0 / PI;

    return angle_degrees;
}

// Main detection function that loops until human activity is detected
void MouseActivity() {
    int attempt = 1;
    int human_detected = 0;

    printf("=== Mouse Behavior Detection System ===\n");
    printf("Waiting for human activity...\n\n");

    // Loop until human behavior is detected
    while (!human_detected) {
        MousePosition positions[REQUIRED_POSITIONS];
        DWORD start_time = GetTickCount();
        int captured = 0;
        POINT last_point = {-1, -1};
        DWORD last_capture_time = 0;

        printf("[Attempt %d] Capturing %d mouse positions...\n", attempt, REQUIRED_POSITIONS);

        // Capture mouse positions
        while (captured < REQUIRED_POSITIONS) {
            POINT current_point;
            if (GetCursorPos(&current_point)) {
                DWORD current_time = GetTickCount();
                DWORD elapsed = current_time - start_time;

                // Check if enough time has passed since last capture
                if (current_time - last_capture_time >= CAPTURE_INTERVAL_MS) {
                    // Check if mouse has moved
                    if (current_point.x != last_point.x || current_point.y != last_point.y) {
                        positions[captured].x = (double)current_point.x;
                        positions[captured].y = (double)current_point.y;
                        positions[captured].timestamp = elapsed;

                        last_point = current_point;
                        last_capture_time = current_time;
                        captured++;
                    }
                }
            }
            Sleep(10); // Small delay to prevent CPU spinning
        }

        printf("[Attempt %d] Positions captured. Analyzing...\n", attempt);

        // Analyze the captured positions
        int i;
        double angles[REQUIRED_POSITIONS - 2];
        int has_sharp_angle = 0;

        // Calculate angles between consecutive vectors
        for (i = 0; i < REQUIRED_POSITIONS - 2; i++) {
            angles[i] = calculate_angle_degrees(positions[i], positions[i + 1], positions[i + 2]);

            if (angles[i] < THRESHOLD_DEGREES) {
                has_sharp_angle = 1;
            }
        }

        // Check if human behavior detected
        if (has_sharp_angle) {
            printf("[Attempt %d] HUMAN ACTIVITY DETECTED!\n", attempt);
            printf("\n=== Vector Analysis ===\n");

            // Display vectors
            for (i = 0; i < REQUIRED_POSITIONS - 1; i++) {
                Vector v;
                v.x = positions[i + 1].x - positions[i].x;
                v.y = positions[i + 1].y - positions[i].y;
                v.magnitude = calculate_distance(positions[i], positions[i + 1]);

                printf("Vector V%d: dx=%.2f, dy=%.2f, magnitude=%.2f\n",
                       i, v.x, v.y, v.magnitude);
            }

            printf("\n=== Angle Analysis ===\n");

            // Display angles
            for (i = 0; i < REQUIRED_POSITIONS - 2; i++) {
                printf("Angle between V%d and V%d: %.2f degrees", i, i + 1, angles[i]);

                if (angles[i] < THRESHOLD_DEGREES) {
                    printf(" [SHARP ANGLE]");
                }
                printf("\n");
            }

            printf("\n=== Detection Result ===\n");
            printf("HUMAN BEHAVIOR CONFIRMED\n");
            printf("At least one angle is less than %.0f degrees (sharp turn)\n\n", THRESHOLD_DEGREES);

            human_detected = 1; // Exit loop
        } else {
            printf("[Attempt %d] Bot-like behavior detected. Retrying...\n\n", attempt);
            attempt++;
            Sleep(500); // Brief pause before next attempt
        }
    }

    printf("Human activity verified. Continuing execution...\n\n");
}

int main() {
    // Call MouseActivity - it will loop until human behavior is detected
    MouseActivity();

    // Execution continues here after human activity is detected
    printf("=== Main Program Continues ===\n");
    printf("Human verification successful!\n");
    printf("Application can now proceed with normal execution...\n");

    printf("\nPress Enter to exit...");
    getchar();

    return 0;
}