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
    "10 4" "10 8" "10 12" "10 16" "10 20" "10 40"
    "50 20" "50 50" "50 70" "50 100" "50 150"
    "100 50" "100 100" "100 200" "100 400"
    "500 200" "500 500" "500 700" "500 1000" "500 1500"
    "1000 500" "1000 1000" "1000 2000" "1000 4000"
    "5000 2000" "5000 5000" "5000 7000" "5000 10000" "5000 15000"
    "10000 5000" "10000 10000" "10000 20000" "10000 40000"
    "100000 50000" "100000 100000" "100000 200000" "100000 400000"
    "1000000 500000" "1000000 1000000" "1000000 2000000" "1000000 4000000"
    "10000000 5000000" "10000000 10000000" "10000000 20000000" "10000000 40000000"
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