#!/bin/bash
afl-fuzz -i corpus -o results -m none -x json.dict ./harness
 
