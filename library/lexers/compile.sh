#!/bin/bash

PWD=`dirname $0`

$PWD/../RE-flex/bin/reflex $PWD/json.l −−outfile=$PWD/../src/serialization/parsing/lexers/lexer_json.yy.h
$PWD/../RE-flex/bin/reflex $PWD/yaml.l −−outfile=$PWD/../src/serialization/parsing/lexers/lexer_yaml.yy.h