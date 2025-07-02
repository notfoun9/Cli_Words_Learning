#!/bin/bash
mkdir ~/.config/words
touch ~/.config/words/config.json

g++ -std=c++20 -O3 main.cpp -o words
chmod +x ./words
echo "Enter password to place the executable in /usr/local/bin:"
sudo mv words /usr/local/bin

echo "Configuration will start after the first execution"

