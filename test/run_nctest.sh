#!/bin/bash

# Check if the user provided a number of executions
if [ -z "$1" ]; then
    echo "Usage: $0 <number_of_executions>"
    exit 1
fi

N=$1  # Number of times to run test.sh

# Detect terminal emulator
TERMINAL=""
if command -v gnome-terminal >/dev/null; then
    TERMINAL="gnome-terminal --"
elif command -v konsole >/dev/null; then
    TERMINAL="konsole -e"
elif command -v xfce4-terminal >/dev/null; then
    TERMINAL="xfce4-terminal -e"
elif command -v xterm >/dev/null; then
    TERMINAL="xterm -e"
elif command -v lxterminal >/dev/null; then
    TERMINAL="lxterminal -e"
else
    echo "No supported terminal found! Try installing gnome-terminal, konsole, or xterm."
    exit 1
fi

# Loop to start test.sh instances in separate terminal windows
for ((i = 1; i <= N; i++)); do

    echo "Opening terminal for nctest.sh with arguments: $i"
    
    # Open test.sh in a new terminal window
    $TERMINAL bash -c "bash nctest.sh $i; exec bash" &
done

echo "All test.sh instances have been started in new windows."
