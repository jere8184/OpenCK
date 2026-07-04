openck:
	clang++\
 main.cpp\
 ./simulator/trait.cpp\
 ./simulator/religion.cpp\
 ./simulator/title.cpp\
 ./simulator/map.cpp\
 ./simulator/army.cpp\
 ./parser/prdx_parser.cpp\
 ./scripting/condition.cpp\
 -std=c++23 -g -I . -o ./bin/openck.exe