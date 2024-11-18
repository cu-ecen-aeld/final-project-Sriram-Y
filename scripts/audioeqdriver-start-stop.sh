#!/bin/sh

if [ "$1" = "start" ]; then
    echo "starting audio eq driver!"
    start-stop-daemon -S -n audioeqdriver -a /usr/bin/audioeqdriver -- -d
    
    if [ $? -eq 0 ]; then
        echo "audio eq driver started successfully!"
    else
        echo "Failed to start audio eq driver."
    fi
elif [ "$1" = "stop" ]; then
    echo "stopping audio eq driver!"
    start-stop-daemon -K -n audioeqdriver --signal TERM
else
    echo "Usage: $0 command <start|stop>"
    exit 1
fi
