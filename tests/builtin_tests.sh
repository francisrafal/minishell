#!/bin/bash

diff -y <(export | cut -d' ' -f 3- | tr -d '"') <(env | sort -n)
