# Mouse Behavior Detection System

A trigonometry-based system that detects human vs. bot behavior by analyzing mouse movement patterns. This implementation is based on anti-sandbox technique used in LummaStealer v4.0.

The blogpost by Outpost42 goes into more detail about this technique: https://outpost24.com/blog/lummac2-anti-sandbox-technique-trigonometry-human-detection/

## Overview

This code has a MouseActivity() function that will loop until human behaviour is detected. This is caluclated by capturing 5 mouse positions and calculating the degrees of the 4 vectors between them.

## How It Works

### 1. Data Collection
- Captures **5 consecutive mouse positions** using GetCursorPos
- Each position is captured with a **50-millisecond interval**
- Records X, Y coordinates and timestamp for each position

```
BOOL GetCursorPos(
  [out] LPPOINT lpPoint
);
```

### 2. Vector Analysis
The system creates **4 vectors** between the 5 captured points:
- Vector V0: P0 → P1
- Vector V1: P1 → P2
- Vector V2: P2 → P3
- Vector V3: P3 → P4

Each vector's magnitude is calculated using the Euclidean distance formula:
```
magnitude = √(Δx² + Δy²)
```

### 3. Angle Calculation
The system calculates **3 angles** between consecutive vectors using:

1. **Dot Product**: `v1.x × v2.x + v1.y × v2.y`
2. **Angle Formula**: `arccos(dot_product / (mag1 × mag2))`
3. **Convert to Degrees**: `angle_radians × (180 / π)`

### 4. Human Detection
The detection logic is simple:
- **Human Behavior**: At least one angle < 45° (sharp turn)
- **Bot Behavior**: All angles ≈ 180° (straight-line movement)


## Mathematics

### Euclidean Distance
```
distance = √((x₂ - x₁)² + (y₂ - y₁)²)
```

### Dot Product
```
v1 · v2 = v1.x × v2.x + v1.y × v2.y
```

### Angle Between Vectors
```
θ = arccos((v1 · v2) / (|v1| × |v2|))
angle_degrees = θ × (180 / π)
```

## Compilation & Usage

### C Version (Windows)

**Compile:**
```bash
gcc -o mouse_detector main.c -lm
```

**Usage:**
1. Run the executable
2. Move your mouse around the screen
3. The program will capture 5 positions automatically
4. View the analysis results in the console

## Example Output

```
=== Mouse Behavior Detection System ===
Waiting for human activity...

[Attempt 1] Capturing 5 mouse positions...
[Attempt 1] Positions captured. Analyzing...
[Attempt 1] HUMAN ACTIVITY DETECTED!

=== Vector Analysis ===
Vector V0: dx=0.00, dy=-1.00, magnitude=1.00
Vector V1: dx=-1.00, dy=-21.00, magnitude=21.02
Vector V2: dx=-2.00, dy=-30.00, magnitude=30.07
Vector V3: dx=-4.00, dy=-18.00, magnitude=18.44

=== Angle Analysis ===
Angle between V0 and V1: 2.73 degrees [SHARP ANGLE]
Angle between V1 and V2: 1.09 degrees [SHARP ANGLE]
Angle between V2 and V3: 8.71 degrees [SHARP ANGLE]

=== Detection Result ===
HUMAN BEHAVIOR CONFIRMED
At least one angle is less than 45 degrees (sharp turn)

Human activity verified. Continuing execution...

=== Main Program Continues ===
Human verification successful!
Application can now proceed with normal execution...
```

## Configuration

You can modify these constants to adjust detection parameters:

```c
#define REQUIRED_POSITIONS 5        // Number of positions to capture
#define CAPTURE_INTERVAL_MS 50      // Time between captures (milliseconds)
#define THRESHOLD_DEGREES 45.0      // Angle threshold for human detection
```

## Limitations

- May produce false negatives if user moves mouse in straight lines
- Windows-specific API in C version (uses `GetCursorPos()`)
- Short capture window (250ms total) requires immediate mouse movement

## License

This code is provided for educational and security research purposes.
