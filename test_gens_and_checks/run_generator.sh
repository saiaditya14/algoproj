#!/bin/bash

# Compile the generator program
echo "Compiling generator.cpp..."
g++-14 -std=c++17 -o generator generator.cpp

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Array of n,m pairs to test
declare -a pairs=(
    "10 10" "10 20" "10 30"
    "100 100" "100 200" "100 500" "100 1000" "100 2000"
    "1000 1000" "1000 2000" "1000 5000" "1000 10000" "1000 20000" "1000 40000"
    "10000 10000" "10000 20000" "10000 50000" "10000 100000" "10000 200000" "10000 400000"
    "100000 100000" "100000 200000" "100000 500000" "100000 1000000" "100000 2000000" "100000 5000000"
    "1000000 1000000" "1000000 2000000" "1000000 5000000" "1000000 10000000"
)

mkdir -p ../tests

# Run generator with each pair of n and m
for pair in "${pairs[@]}"; do
    read -r n m <<< "$pair"
    echo "Running generator with n=$n, m=$m"
    
    # Run the generator and save output to a file
    ./generator $n $m
    
    if [ $? -eq 0 ]; then
        echo "  Success! Output saved to ../tests/test_${n}_${m}.txt"
    else
        echo "  Error running generator with n=$n, m=$m"
    fi
done

echo "All tests completed!"