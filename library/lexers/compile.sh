#!/bin/bash

PWD=`dirname $0`

# check existing of the binary
REFLEX=$PWD/../RE-flex/bin/reflex

if [[ ! -f $REFLEX ]]
then
    (cd $PWD/../RE-flex ; ./clean.sh ; ./build.sh)
fi

# generate lexers
$REFLEX $PWD/json.l \
  −−outfile=$PWD/../src/serialization/json/lexer_json.yy.cpp \
  −−header-file=$PWD/../src/serialization/json/lexer_json.yy.h

$REFLEX $PWD/yaml.l \
  −−outfile=$PWD/../src/serialization/yaml/lexer_yaml.yy.cpp \
  −−header-file=$PWD/../src/serialization/yaml/lexer_yaml.yy.h

# declare success
COLOR='\033[0;36m' #Cyan

if command -v tput &> /dev/null
then
    BOLD=$(tput bold)
    NORM=$(tput sgr0)
fi

echo -e ${COLOR}${BOLD}"Lexer files were generated successfully!"${NORM}
