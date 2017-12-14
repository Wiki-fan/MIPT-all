#!/bin/bash

hdfs fsck -blocks $1 2>/dev/null | python scr.py

