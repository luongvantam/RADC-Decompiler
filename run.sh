#!/bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' 
BOLD='\033[1m'

mkdir -p hex_ropchain
mkdir -p asm_ropchain

# Check if radc executable exists
if [ ! -f "radc" ]; then
    printf "${YELLOW}Executable 'radc' not found. Trying to build...${NC}\n"
    sh ./build.sh
    if [ $? -ne 0 ]; then
        printf "${RED}[Error] Failed to build radc! Please compile it manually.${NC}\n"
        exit 1
    fi
fi

while true; do
    clear
    printf "${BLUE}╔════════════════════════════════════════╗${NC}\n" 
    printf "${BLUE}║      ${BOLD}radc - ROP Asm Decompiler         ${BLUE}║${NC}\n"
    printf "${BLUE}╚════════════════════════════════════════╝${NC}\n"
    printf "${YELLOW}Tip: Enter 'q' or 'exit' to quit.${NC}\n\n"

    printf "Enter filename to decompile (e.g. test.txt): "
    read filename

    if [ -z "$filename" ] || [ "$filename" = "q" ] || [ "$filename" = "exit" ]; then
        printf "\n${GREEN}[*] Exiting program. Goodbye!${NC}\n"
        break
    fi

    # Output filename = input basename (no extension) + .asm
    base=$(basename "$filename")
    out_filename="${base%.*}.asm"

    input_path=""
    if [ -f "hex_ropchain/$filename" ]; then
        input_path="hex_ropchain/$filename"
    elif [ -f "$filename" ]; then
        input_path="$filename"
    else
        printf "${RED}[Error] File not found: '$filename'${NC}\n"
        printf "Please place your hex payload file into the ${YELLOW}hex_ropchain/${NC} directory or the current directory.\n"
        printf "Press Enter to try again...\n"
        read dummy
        continue
    fi
    
    output_path="asm_ropchain/$out_filename"
    model_name="991cnx_vf"

    printf "\n${BLUE}[*] Processing and decompiling '${input_path}'...${NC}\n"
    
    ./radc --no-recompile "$model_name" "$input_path" "$output_path"

    if [ $? -eq 0 ]; then
        printf "\n${GREEN}✔ Decompilation completed successfully!${NC}\n"
        printf "➡ Output saved to: ${BOLD}${GREEN}$output_path${NC}\n"
    else
        printf "\n${RED}✘ An error occurred while executing radc.${NC}\n"
        printf "Note: Make sure your file is not empty and contains valid hex bytes.\n"
    fi

    printf "\nPress Enter to continue...\n"
    read dummy
done