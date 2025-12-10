# Mouse Behavior Detection System

A trigonometry-based system that detects human vs. bot behavior by analyzing mouse movement patterns. This implementation is based on anti-sandbox technique used in LummaStealer v4.0.

The blogpost by Outpost42 goes into more detail about this technique: https://outpost24.com/blog/lummac2-anti-sandbox-technique-trigonometry-human-detection/

## Overview

This system uses geometric analysis of mouse movements to distinguish between human users and automated bots. It captures mouse positions at regular intervals and calculates angles between movement vectors to identify characteristic human behavior patterns.

## How It Works

### 1. Data Collection
- Captures **5 consecutive mouse positions**
- Each position is captured with a **50-millisecond interval**
- Records X, Y coordinates and timestamp for each position

BOOL GetCursorPos(
  [out] LPPOINT lpPoint
);

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

Humans naturally make quick direction changes and curved movements, resulting in sharp angles. Bots typically move in straight lines with minimal direction changes.

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
gcc -o mouse_detector mouse_detector.c -lm
```


**Usage:**
1. Run the executable
2. Move your mouse around the screen
3. The program will capture 5 positions automatically
4. View the analysis results in the console

## Example Output

```
=== Mouse Behavior Detection System ===
Capturing 5 positions with 50 ms intervals...

Position 0: (523, 412) at 0 ms
Position 1: (589, 445) at 52 ms
Position 2: (634, 502) at 103 ms
Position 3: (651, 578) at 154 ms
Position 4: (643, 641) at 206 ms

=== Vector Analysis ===
Vector V0: dx=66.00, dy=33.00, magnitude=73.82
Vector V1: dx=45.00, dy=57.00, magnitude=72.60
Vector V2: dx=17.00, dy=76.00, magnitude=77.88
Vector V3: dx=-8.00, dy=63.00, magnitude=63.51

=== Angle Analysis ===
Angle between V0 and V1: 35.42 degrees [SHARP ANGLE - Human-like behavior detected!]
Angle between V1 and V2: 42.18 degrees [SHARP ANGLE - Human-like behavior detected!]
Angle between V2 and V3: 78.93 degrees

=== Detection Result ===
HUMAN BEHAVIOR DETECTED
At least one angle is less than 45 degrees (sharp turn)
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
