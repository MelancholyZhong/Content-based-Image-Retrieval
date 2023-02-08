CC = g++
CFLAGS = -g -Wall -std=c++11 
CAM_PLIST_FLAG = -sectcreate __TEXT __info_plist Info.plist
PRODUCTS = driver match test.csv


OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

driver : driver.cpp histograms.cpp distances.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

match : main.cpp matchingAlgo.cpp fileHelper.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean : 
	rm -r -f *.o *~ $(PRODUCTS) *.dSYM

