#!/bin/bash

set -e

doxygen

scp ~/Desktop/workspace/multiTrace\ vision/doc/html/* gmerritt@chester:~/public_html/cs280/doc/
