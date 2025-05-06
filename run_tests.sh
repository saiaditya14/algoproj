#!/bin/bash
# filepath: /Users/aryan12/Desktop/IIITH/IAE/algoproj/run_tests.sh

# Check if algorithm name is provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <algorithm_name>"
    echo "Valid algorithm names: jen_schmidt, tarjan_dfs, tarjan_vishkin_dsu, tarjan_vishkin_no_dsu"
    exit 1
fi

ALGO=$1

# Validate algorithm name
case $ALGO in
    jen_schmidt|tarjan_dfs|tarjan_vishkin_dsu|tarjan_vishkin_no_dsu)
        # Valid algorithm
        ;;
    *)
        echo "Invalid algorithm name: $ALGO"
        echo "Valid algorithm names: jen_schmidt, tarjan_dfs, tarjan_vishkin_dsu, tarjan_vishkin_no_dsu"
        exit 1
        ;;
esac

# Path to the source file
SRC_FILE="src/${ALGO}.cpp"

# Check if source file exists
if [ ! -f "$SRC_FILE" ]; then
    echo "Source file not found: $SRC_FILE"
    exit 1
fi

# Compile the source file
echo "Compiling $SRC_FILE..."
g++-14 -std=c++17 -O2 -o algo_executable "$SRC_FILE"
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Create output directory if it doesn't exist
OUTPUT_DIR="outputs/${ALGO}"
mkdir -p "$OUTPUT_DIR"

# Find and process all test files
echo "Running $ALGO on test files..."
for TEST_FILE in tests/test_*.in; do
    # Extract the test file name
    TEST_NAME=$(basename "$TEST_FILE" .in)
    
    # Extract x and y values from the test name (assuming format test_x_y.in)
    X=$(echo "$TEST_NAME" | cut -d'_' -f2)
    Y=$(echo "$TEST_NAME" | cut -d'_' -f3)
    
    OUTPUT_FILE="${OUTPUT_DIR}/${TEST_NAME}.out"
    
    echo "  Processing $TEST_NAME..."
    
    # Run the algorithm and save the output
    ./algo_executable < "$TEST_FILE" > "$OUTPUT_FILE"
done

echo "All tests processed for $ALGO"
echo "Results saved to $OUTPUT_DIR/"

# Clean up the executable
rm algo_executable