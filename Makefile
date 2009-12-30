:#========================================================================
# build configuration

CPP = g++
CPPFLAGS = -c -w -O2 -g `pkg-config gtkmm-2.4 --cflags`

CPPLNFLAGS =  

INCLUDEDIRS =  -I/usr/include/gtkmm-2.4
LIBDIRS =  -L /usr/lib/lib32 

BINARIES = aoide

OBJECTS = aboutdialog.o debuglog.o directorytree.o main_window.o mplayer.o playlist.o main_window_glade.o aoide.o
 
LIBRARIES = `pkg-config gtkmm-2.4--libs` \
-lgtkmm-2.4 -lgiomm-2.4 -lgdkmm-2.4 -latkmm-1.6 -lgtk-x11-2.0 -lpangomm-1.4 -lcairomm-1.0 -lglibmm-2.4 -lsigc-2.0 -lgdk-x11-2.0 -latk-1.0 -lpangoft2-1.0 -lgdk_pixbuf-2.0 -lm -lpangocairo-1.0 -lgio-2.0 -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lglib-2.0  -lX11 -pthread -lboost_thread \
 -L"/usr/lib" $(PWD)/irrklang_lib/libIrrKlang.so
#
#========================================================================
all:: $(BINARIES)
%.o: %.cpp; $(CPP) $(CPPFLAGS) $(INCLUDEDIRS) $< -o $@

#------------------------------------------------------------------------
# create executable
#
aoide: $(OBJECTS)
	$(CPP) $(CPPLNFLAGS) $(LIBDIRS) $(LIBRARIES) $(OBJECTS) \
	-o $(BINARIES)


#
run:
	./aoide
clean:
	rm *.o
	rm aoide
