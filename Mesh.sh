#!/bin/bash
concept=$1
search="^MH = "$concept"$"
cat ~/scripts/StructuredQuery/d2014.bin |grep -A5 "$search"|grep -v ","|grep ENTRY | head -2 |grep "|" | cut -d"=" -f2 | cut -d"|" -f1|sed 's/^.//'
