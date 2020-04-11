GCC=gcc
CFLAGS=-Wall -ansi -O2

OBJ_UTIL=bin/util.lang.o \
		 bin/util.contract.o \
		 bin/util.func.o \
		 bin/util.list.o \
		 bin/util.tree.o \
		 bin/util.str.o \
		 bin/util.error.o \
		 bin/util.misc.o \
		 bin/util.matrix.o

OBJ_UI=bin/ui.transform.o \
	   bin/ui.point.o \
	   bin/ui.scene.o \
	   bin/ui.drawables.drawable.o \
	   bin/ui.drawables.ellipse.o \
	   bin/ui.drawables.sprite.o

OBJ_GAME=bin/game.entity.o

OBJ_CHALLENGE=bin/game.o \
			  bin/interface.o \
			  bin/acquisition.o \
			  bin/plugins.o \
			  bin/main.o

OBJ=$(OBJ_UTIL) $(OBJ_UI) $(OBJ_GAME) $(OBJ_CHALLENGE)

HEADERS_UTIL=includes/util/lang.h \
			 includes/util/contract.h \
			 includes/util/func.h \
			 includes/util/list.h \
			 includes/util/tree.h \
			 includes/util/str.h \
			 includes/util/error.h \
			 includes/util/misc.h \
			 includes/util/matrix.h

HEADERS_UI=includes/ui/transform.h \
		   includes/ui/point.h \
		   includes/ui/scene.h \
		   includes/ui/drawables/drawable.h \
		   includes/ui/drawables/shapes.h \
		   includes/ui/drawables/ellipse.h \
		   includes/ui/drawables/sprite.h

HEADERS_GAME=includes/game/entity.h

HEADERS_CHALLENGE=includes/game.h \
				  includes/interface.h \
				  includes/acquisition.h \
				  includes/plugins.h

HEADERS=$(HEADERS_UTIL) $(HEADERS_UI) $(HEADERS_GAME) $(HEADERS_CHALLENGE)

PLUGINS=plugins/full_dummy.so \
		plugins/rand_all.so \
		plugins/rand_move.so \
		plugins/rand_shot.so \
		plugins/circle_rand_shot.so \
		plugins/rand_move_info.so

EXECUTABLE=starshipcode
all: $(EXECUTABLE) $(PLUGINS)

$(EXECUTABLE): $(OBJ)
	$(GCC) -o $(EXECUTABLE) $(OBJ) $(CFLAGS) -lMLV -lm -ldl

bin/util.lang.o: src/util/lang.c $(HEADERS)
	$(GCC) -c src/util/lang.c -o bin/util.lang.o $(CFLAGS) -lMLV -ldl

bin/util.contract.o: src/util/contract.c $(HEADERS)
	$(GCC) -c src/util/contract.c -o bin/util.contract.o $(CFLAGS) -lMLV -ldl

bin/util.func.o: src/util/func.c $(HEADERS)
	$(GCC) -c src/util/func.c -o bin/util.func.o $(CFLAGS) -lMLV -ldl

bin/util.list.o: src/util/list.c $(HEADERS)
	$(GCC) -c src/util/list.c -o bin/util.list.o $(CFLAGS) -lMLV -ldl

bin/util.tree.o: src/util/tree.c $(HEADERS)
	$(GCC) -c src/util/tree.c -o bin/util.tree.o $(CFLAGS) -lMLV -ldl

bin/util.str.o: src/util/str.c $(HEADERS)
	$(GCC) -c src/util/str.c -o bin/util.str.o $(CFLAGS) -lMLV -ldl

bin/util.error.o: src/util/error.c $(HEADERS)
	$(GCC) -c src/util/error.c -o bin/util.error.o $(CFLAGS) -lMLV -ldl

bin/util.misc.o: src/util/misc.c $(HEADERS)
	$(GCC) -c src/util/misc.c -o bin/util.misc.o $(CFLAGS) -lMLV -ldl

bin/util.matrix.o: src/util/matrix.c $(HEADERS)
	$(GCC) -c src/util/matrix.c -o bin/util.matrix.o $(CFLAGS) -lMLV -ldl

bin/ui.transform.o: src/ui/transform.c $(HEADERS)
	$(GCC) -c src/ui/transform.c -o bin/ui.transform.o $(CFLAGS) -lMLV -ldl

bin/ui.point.o: src/ui/point.c $(HEADERS)
	$(GCC) -c src/ui/point.c -o bin/ui.point.o $(CFLAGS) -lMLV -ldl

bin/ui.scene.o: src/ui/scene.c $(HEADERS)
	$(GCC) -c src/ui/scene.c -o bin/ui.scene.o $(CFLAGS) -lMLV -ldl

bin/ui.drawables.drawable.o: src/ui/drawables/drawable.c $(HEADERS)
	$(GCC) -c src/ui/drawables/drawable.c -o bin/ui.drawables.drawable.o $(CFLAGS) -lMLV -ldl

bin/ui.drawables.ellipse.o: src/ui/drawables/ellipse.c $(HEADERS)
	$(GCC) -c src/ui/drawables/ellipse.c -o bin/ui.drawables.ellipse.o $(CFLAGS) -lMLV -ldl -lm

bin/ui.drawables.sprite.o: src/ui/drawables/sprite.c $(HEADERS)
	$(GCC) -c src/ui/drawables/sprite.c -o bin/ui.drawables.sprite.o $(CFLAGS) -lMLV -ldl -lm

bin/game.entity.o:src/game/entity.c $(HEADERS)
	$(GCC) -c src/game/entity.c -o bin/game.entity.o $(CFLAGS) -lMLV -ldl

bin/game.o: src/game.c includes/game.h
	$(GCC) -c src/game.c -o bin/game.o $(CFLAGS) -lm

bin/interface.o: src/interface.c includes/interface.h includes/game.h
	$(GCC) -c src/interface.c -o bin/interface.o $(CFLAGS) -lMLV -lm

bin/acquisition.o: src/acquisition.c includes/acquisition.h
	$(GCC) -fPIC -c src/acquisition.c -o bin/acquisition.o $(CFLAGS)

bin/plugins.o: src/plugins.c includes/plugins.h
	$(GCC) -c src/plugins.c -o bin/plugins.o $(CFLAGS) -lrt

bin/main.o: src/main.c $(HEADERS)
	$(GCC) -c src/main.c -o bin/main.o $(CFLAGS) -lMLV -ldl

#############################
#     Plugins examples      #
#############################
plugins/full_dummy.so: plugins/full_dummy.c bin/acquisition.o
	$(GCC) -fPIC -c plugins/full_dummy.c -o plugins/full_dummy.o $(CFLAGS)
	$(GCC) -shared -o plugins/full_dummy.so plugins/full_dummy.o bin/acquisition.o $(CFLAGS)

plugins/rand_all.so: plugins/rand_all.c bin/acquisition.o
	$(GCC) -fPIC -c plugins/rand_all.c -o plugins/rand_all.o $(CFLAGS)
	$(GCC) -shared -o plugins/rand_all.so plugins/rand_all.o bin/acquisition.o $(CFLAGS)

plugins/rand_move.so: plugins/rand_move.c bin/acquisition.o
	$(GCC) -fPIC -c plugins/rand_move.c -o plugins/rand_move.o $(CFLAGS)
	$(GCC) -shared -o plugins/rand_move.so plugins/rand_move.o bin/acquisition.o $(CFLAGS)

plugins/rand_shot.so: plugins/rand_shot.c bin/acquisition.o
	$(GCC) -fPIC -c plugins/rand_shot.c -o plugins/rand_shot.o $(CFLAGS)
	$(GCC) -shared -o plugins/rand_shot.so plugins/rand_shot.o bin/acquisition.o $(CFLAGS)

plugins/circle_rand_shot.so: plugins/circle_rand_shot.c bin/acquisition.o
	$(GCC) -fPIC -c plugins/circle_rand_shot.c -o plugins/circle_rand_shot.o $(CFLAGS)
	$(GCC) -shared -o plugins/circle_rand_shot.so plugins/circle_rand_shot.o bin/acquisition.o $(CFLAGS)

plugins/rand_move_info.so: plugins/rand_move_info.c bin/acquisition.o
	$(GCC) -fPIC -c plugins/rand_move_info.c -o plugins/rand_move_info.o $(CFLAGS)
	$(GCC) -shared -o plugins/rand_move_info.so plugins/rand_move_info.o bin/acquisition.o $(CFLAGS)

clean:
	rm -f bin/*.o
	rm -f plugins/*.o
	rm -f plugins/*.so
	rm -f starshipcode
	rm -f */*~
	rm -f *~

rebuild:
	make clean
	make
