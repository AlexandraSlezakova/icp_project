FILES = Makefile \
	README.txt \
	src/Makefile \
	src/Functions.cpp \
	src/Functions.h \
	src/main.cpp \
	src/main.h \
	src/mainwindow.ui \
	src/src.pro \
	src/ui_mainwindow.h
	
DELETE = src/Bus.o \
	src/BusRouteMap.o \
	src/Coordinates.o \
	src/Functions.o \
	src/Garage.o \
	src/main.o \
	src/MainWindow.o \
	src/moc_MainWindow.cpp \
	src/moc_MainWindow.o \
	src/moc_predefs.h \
	src/moc_Scene.cpp \
	src/moc_Scene.o \
	src/Scene.o \
	src/Square.o \
	src/icp \
	src/Stop.o \
	src/Street.o \
	src/StreetMap.o \
	src/Timer.o \
	
build:
	cd src && make


run:
	cd src && make
	cd src && ./icp
	
doxygen:
	doxygen doc/Doxyfile

pack:
	zip xvadur04-xsleza20 Makefile README.txt -r src/Bus/* -r src/View/* $(FILES) -r examples/* -r images/*
	
clean:
	rm -rf doc/latex/*
	rm -rf doc/html/*
	rm -rf doc/latex
	rm -rf doc/html
	rm -rf $(DELETE)
	rm -rf xvadur04-xsleza20.zip
