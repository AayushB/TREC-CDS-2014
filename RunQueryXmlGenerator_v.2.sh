#!/bin/bash
filename=$1
resultfile=$2
metamap13 -cy $filename ./meta.tmp


awk -v RS='' -F'\n' -v re='^Meta Mapping \\(' '
    # Only process non-empty records:
    # those that have at least 1 "Meta Mapping" line.
  $2 ~ re { 
    print $1 # print the "Phrase: " line
    print $2 # print the 1st "Meta Mapping" line.
      # Print the remaining lines, if any, up to but not including
      # the next "Meta Mapping" line.
    for (i=3;i<=NF;++i) {
      if ($i ~ re) break # next "Meta Mapping" found; ignore and terminate block.
      print $i
    }
    print "" # print empty line between output blocks
  }
' ./meta.tmp|sed '/Meta Mapping/d' |sed '/Processing/d'|  sed -e 's/^[ \t]*//' | sed 's/   / /'| cut -d" " -f2-1000| sed 's/^$/@/'| sed -e 's|["'\'']| " |g' | sed -e 's|[('\'']| ( |g' |  sed -e 's|[)'\'']| ) |g'| sed -e 's|[['\'']| [ |g'  |  sed -e 's|[]'\'']| ] |g' |sed -e 's/^[ \t]*//' >./a.tmp
mv ./a.tmp ./meta.tmp

metamap13 --negex $filename ./negationMetaMap.tmp
cat ./negationMetaMap.tmp |grep "Negated  Concept"|cut -f2-10 -d:| tr ',' '\n'| sed -e 's/^[ \t]*//'| cut -f2 -d: > ./negation.tmp
rm ./negationMetaMap.tmp



echo "<parameters>
 	<index>/home/aayush/indexes/pmc-text-index-with-fields</index>
   	 <count>1000</count>
   	 <trecFormat>true</trecFormat>
    	<query>
		<type>indri</type>
        	<number>MT1</number>
        	<text>
		">$resultfile
~/scripts/StructuredQuery/Query >>$resultfile

echo "     		</text>

    	</query>
</parameters>
">>$resultfile


rm ./meta.tmp
rm ./negation.tmp
 
