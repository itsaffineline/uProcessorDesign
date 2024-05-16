#!/bin/bash

while true; do
    read -n 1 char
    if [[ -z "$char" ]]; then
        echo "something's very wrong"
        break
    fi
    char=$(echo "$char" | tr '[:lower:]' '[:upper:]')
    curl -s "http://192.168.1.12/${char}/" > /dev/null
    echo
done
