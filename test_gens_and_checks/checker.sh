#!/bin/bash

# Check if two algorithm names are provided
if [ $# -ne 2 ]; then
    echo "Usage: $0 <algorithm_name_1> <algorithm_name_2>"
    echo "Valid algorithm names: jen_schmidt, tarjan_dfs, tarjan_vishkin_dsu, tarjan_vishkin_no_dsu"
    exit 1
fi

# Validate algorithm names
case $1 in
    jen_schmidt|tarjan_dfs|tarjan_vishkin_dsu|tarjan_vishkin_no_dsu)
        # Valid algorithm
        ;;
    *)
        echo "Invalid algorithm name: $1"
        echo "Valid algorithm names: jen_schmidt, tarjan_dfs, tarjan_vishkin_dsu, tarjan_vishkin_no_dsu"
        exit 1
        ;;
esac
case $2 in
    jen_schmidt|tarjan_dfs|tarjan_vishkin_dsu|tarjan_vishkin_no_dsu)
        # Valid algorithm
        ;;
    *)
        echo "Invalid algorithm name: $2"
        echo "Valid algorithm names: jen_schmidt, tarjan_dfs, tarjan_vishkin_dsu, tarjan_vishkin_no_dsu"
        exit 1
        ;;
esac


FIRST_DIR="../outputs/$1"
SECOND_DIR="../outputs/$2"
TEST_DIR="../tests"

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

for file in "$FIRST_DIR"/*; do
    basename=$(basename "$file")
    if [ -f "$SECOND_DIR/$basename" ]; then
        echo "Comparing $basename:"
        
        # Get first line of each file
        line1=$(head -n 1 "$FIRST_DIR/$basename")
        line2=$(head -n 1 "$SECOND_DIR/$basename")
        
        # Compare first lines
        if [ "$line1" = "$line2" ]; then
            echo -e "${GREEN}ACCEPT${NC}"
        else
            echo -e "${RED}REJECT${NC}"
        fi
        
        echo "-----------------------"
    fi
done