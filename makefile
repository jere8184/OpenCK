openck:
	g++\
 Main.cpp\
 ./Simulator/Trait.cpp\
 ./Simulator/Religion.cpp\
 ./Simulator/Title.cpp\
 ./Simulator/Map.cpp\
 ./Simulator/Army.cpp\
 ./Simulator/ScriptedTrigger.cpp\
 ./Parser/PrdxParser.cpp\
 ./Scripting/Condition.cpp\
 -std=c++26 -freflection -g -I . -o ./bin/openck.exe