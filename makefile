openck:
	clang++\
 main.cpp\
 ./simulator/trait.cpp\
 ./simulator/religion.cpp\
 ./simulator/title.cpp\
 ./simulator/map.cpp\
 ./simulator/army.cpp\
 ./simulator/scripted_trigger.cpp\
 ./parser/prdx_parser.cpp\
 ./scripting/condition.cpp\
 -std=c++26 -g -I . -o ./bin/openck.exe