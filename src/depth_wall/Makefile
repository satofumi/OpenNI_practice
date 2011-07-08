TARGET = moving_sphere

CXXFLAGS = -g -O0 -Wall -W $(INCLUDES)
INCLUDES = -I/usr/include/ni
LDFLAGS =
LDLIBS = -lOpenNI -lcv -lhighgui


all : $(TARGET)

clean :
	$(RM) $(TARGET) *.o

.PHONY : all clean
