#!/usr/bin/bash

mkdir -p build && cd build && cmake .. && ln compile_commands.json ../compile_commands.json
