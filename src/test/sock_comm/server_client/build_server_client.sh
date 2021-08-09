#!/bin/bash

set -x
gcc server.c -o myserver
gcc client.c -o myclient

