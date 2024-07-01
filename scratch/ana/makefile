
.PHONY: all clean

src = $(wildcard source/*.cpp)
# src = source/sbt.cpp source/hime_veto.cpp source/kyoto.cpp
ana : ana.cpp $(src)
	g++ $^ -o ana.exe `root-config --libs --cflags` -I./include

clean:
	rm -f ana.exe