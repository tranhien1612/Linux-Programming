# CXX = g++ #CC=gcc
# CXXFLAGS = -g -Wall -Wl,-rpath=/home/maj/ffmpeg-3x64/lib #CXXFLAGS = -g -Wall
# INCLAGS = -I/home/maj/ffmpeg-3x64/include #-I/home/origin/include

# LDFLAGS = -L/home/maj/ffmpeg-3x64/lib #-L./aarch64
# LDLIBS = -libswscale -libavcodec -libavutil -libavformat

OPENCV_LIBS = $(shell pkg-config --cflags --libs opencv4) #`pkg-config --cflags --libs opencv4`
FFMPEG_LIBS = $(shell pkg-config --cflags --libs libavformat libavutil libavcodec libswscale libavdevice)


main:
	g++ -o main main.cpp $(OPENCV_LIBS) $(FFMPEG_LIBS)

clean:
	rm -f main