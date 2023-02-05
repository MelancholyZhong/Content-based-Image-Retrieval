CC = g++
CFLAGS = -g -Wall -std=c++11 
CAM_PLIST_FLAG = -sectcreate __TEXT __info_plist Info.plist
PRODUCTS = imgDisplay vidDisplay


OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

imgDisplay : imgDisplay.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) 

vidDisplay : vidDisplay.cpp filters.cpp 
	$(CC) $(CFLAGS) $(CAM_PLIST_FLAG) -o $@ $^ $(LIBS) 

.PHONY: clean

clean : 
	rm -r -f *.o *~ $(PRODUCTS) *.dSYM

