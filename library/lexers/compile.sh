#!/bin/bash

PWD=`dirname $0`

$PWD/../RE-flex/bin/reflex $PWD/json.l \
  −−outfile=$PWD/../src/serialization/json/lexer_json.yy.cpp \
  −−header-file=$PWD/../src/serialization/json/lexer_json.yy.h

$PWD/../RE-flex/bin/reflex $PWD/yaml.l \
  −−outfile=$PWD/../src/serialization/yaml/lexer_yaml.yy.cpp \
  −−header-file=$PWD/../src/serialization/yaml/lexer_yaml.yy.h
