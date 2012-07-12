STATIC_LIB=libi2c_wrapper.a
SHARED_LIB=

TARGETS = $(STATIC_LIB)

SOURCES = i2c_wrapper.c i2cbusses.c check_funcs.c


CC=gcc

OBJECTS = $(SOURCES:.c=.o)

INCLUDE_FILES = i2cbusses.h i2c_wrapper.h check_funcs.h
INCLUDES = -I../include/


CFLAGS= -fPIC -Wall 

LDFLAGS = -L../lib
LDLIBS= -li2c_wrapper

#all: deps.d $(TARGETS) accel_test battery_test
all: deps.d $(TARGETS) 

$(STATIC_LIB): $(OBJECTS)
	ar rcs $@ $^
	mv $(STATIC_LIB) ../lib

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

deps.d: $(SOURCES)
	$(CC) -MM $(INCLUDES) $(SOURCES) -MF $@

accel_test:
	$(CC) $(INCLUDES) accel_test.c  $(LDFLAGS) $(LDLIBS) -o accel_test
	mv accel_test ../bin

battery_test:
	$(CC) $(INCLUDES) battery_test.c  $(LDFLAGS) $(LDLIBS) -o battery_test
	mv battery_test ../bin

clean:
	rm -rf $(OBJECTS) deps.d
	rm -rf $(TARGETS) *.a *.o ../lib/*.a 
	rm -f ../bin/battery_test
	rm -f ../bin/accel_test

-include deps.d
