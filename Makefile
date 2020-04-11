GCC=gcc
CFLAGS=-Wall -ansi -O2 -Iincludes

OBJ_MLVE_UTIL=bin/MLVEngine/util.lang.o \
		 bin/MLVEngine/util.contract.o \
		 bin/MLVEngine/util.func.o \
		 bin/MLVEngine/util.list.o \
		 bin/MLVEngine/util.tree.o \
		 bin/MLVEngine/util.str.o \
		 bin/MLVEngine/util.error.o \
		 bin/MLVEngine/util.misc.o \
		 bin/MLVEngine/util.matrix.o

OBJ_MLVE_UI=bin/MLVEngine/ui.transform.o \
	   bin/MLVEngine/ui.point.o \
	   bin/MLVEngine/ui.scene.o \
	   bin/MLVEngine/ui.drawables.drawable.o \
	   bin/MLVEngine/ui.drawables.ellipse.o \
	   bin/MLVEngine/ui.drawables.sprite.o

OBJ_MLVE_GAME=bin/MLVEngine/game.entity.o

OBJ_MLVE=$(OBJ_MLVE_UTIL) $(OBJ_MLVE_UI) $(OBJ_MLVE_GAME)

HEADERS_MLVE_UTIL=includes/MLVEngine/util/lang.h \
				  includes/MLVEngine/util/contract.h \
				  includes/MLVEngine/util/func.h \
			 	  includes/MLVEngine/util/list.h \
				  includes/MLVEngine/util/tree.h \
			 	  includes/MLVEngine/util/str.h \
			 	  includes/MLVEngine/util/error.h \
			 	  includes/MLVEngine/util/misc.h \
			 	  includes/MLVEngine/util/matrix.h

HEADERS_MLVE_UI=includes/MLVEngine/ui/transform.h \
		   		includes/MLVEngine/ui/point.h \
		   		includes/MLVEngine/ui/scene.h \
		   		includes/MLVEngine/ui/drawables/drawable.h \
		   		includes/MLVEngine/ui/drawables/shapes.h \
		   		includes/MLVEngine/ui/drawables/ellipse.h \
		   		includes/MLVEngine/ui/drawables/sprite.h

HEADERS_MLVE_GAME=includes/MLVEngine/game/entity.h

HEADERS_MLVE=$(HEADERS_MLVE_UTIL) $(HEADERS_MLVE_UI) $(HEADERS_MLVE_GAME)

OBJ_CHALLENGE=bin/game.o \
			  bin/interface.o \
			  bin/acquisition.o \
			  bin/plugins.o \
			  bin/main.o

OBJ=$(OBJ_MLVE) $(OBJ_CHALLENGE)

HEADERS_CHALLENGE=includes/game.h \
				  includes/interface.h \
				  includes/acquisition.h \
				  includes/plugins.h

HEADERS=$(HEADERS_MLVE) $(HEADERS_CHALLENGE)

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

bin/MLVEngine/util.lang.o: src/MLVEngine/util/lang.c $(HEADERS)
	$(GCC) -c src/MLVEngine/util/lang.c -o bin/MLVEngine/util.lang.o $(CFLAGS) -lMLV -ldl

bin/MLVEngine/util.contract.o: src/MLVEngine/util/contract.c $(HEADERS)
	$(GCC) -c src/MLVEngine/util/contract.c -o bin/MLVEngine/util.contract.o $(CFLAGS) -lMLV -ldl

bin/MLVEngine/util.func.o: src/MLVEngine/util/func.c $(HEADERS)
	$(GCC) -c src/MLVEngine/util/func.c -o bin/MLVEngine/util.func.o $(CFLAGS) -lMLV -ldl

bin/MLVEngine/util.list.o: src/MLVEngine/util/list.c $(HEADERS)
	$(GCC) -c src/MLVEngine/util/list.c -o bin/MLVEngine/util.list.o $(CFLAGS) -lMLV -ldl

bin/MLVEngine/util.tree.o: src/MLVEngine/util/tree.c $(HEADERS)
	$(GCC) -c src/MLVEngine/util/tree.c -o bin/MLVEngine/util.tree.o $(CFLAGS) -lMLV -ldl

bin/MLVEngine/util.str.o: src/MLVEngine/util/str.c $(HEADERS)
	$(GCC) -c src/MLVEngine/util/str.c -o bin/MLVEngine/util.str.o $(CFLAGS) -lMLV -ldl

bin/MLVEngine/util.error.o: src/MLVEngine/util/error.c $(HEADERS)
	$(GCC) -c src/MLVEngine/util/error.c -o bin/MLVEngine/util.error.o $(CFLAGS) -lMLV -ldl

bin/MLVEngine/util.misc.o: src/MLVEngine/util/misc.c $(HEADERS)
	$(GCC) -c src/MLVEngine/util/misc.c -o bin/MLVEngine/util.misc.o $(CFLAGS) -lMLV -ldl

bin/MLVEngine/util.matrix.o: src/MLVEngine/util/matrix.c $(HEADERS)
	$(GCC) -c src/MLVEngine/util/matrix.c -o bin/MLVEngine/util.matrix.o $(CFLAGS) -lMLV -ldl

bin/MLVEngine/ui.transform.o: src/MLVEngine/ui/transform.c $(HEADERS)
	$(GCC) -c src/MLVEngine/ui/transform.c -o bin/MLVEngine/ui.transform.o $(CFLAGS) -lMLV -ldl

bin/MLVEngine/ui.point.o: src/MLVEngine/ui/point.c $(HEADERS)
	$(GCC) -c src/MLVEngine/ui/point.c -o bin/MLVEngine/ui.point.o $(CFLAGS) -lMLV -ldl

bin/MLVEngine/ui.scene.o: src/MLVEngine/ui/scene.c $(HEADERS)
	$(GCC) -c src/MLVEngine/ui/scene.c -o bin/MLVEngine/ui.scene.o $(CFLAGS) -lMLV -ldl

bin/MLVEngine/ui.drawables.drawable.o: src/MLVEngine/ui/drawables/drawable.c $(HEADERS)
	$(GCC) -c src/MLVEngine/ui/drawables/drawable.c -o bin/MLVEngine/ui.drawables.drawable.o $(CFLAGS) -lMLV -ldl

bin/MLVEngine/ui.drawables.ellipse.o: src/MLVEngine/ui/drawables/ellipse.c $(HEADERS)
	$(GCC) -c src/MLVEngine/ui/drawables/ellipse.c -o bin/MLVEngine/ui.drawables.ellipse.o $(CFLAGS) -lMLV -ldl -lm

bin/MLVEngine/ui.drawables.sprite.o: src/MLVEngine/ui/drawables/sprite.c $(HEADERS)
	$(GCC) -c src/MLVEngine/ui/drawables/sprite.c -o bin/MLVEngine/ui.drawables.sprite.o $(CFLAGS) -lMLV -ldl -lm

bin/MLVEngine/game.entity.o:src/MLVEngine/game/entity.c $(HEADERS)
	$(GCC) -c src/MLVEngine/game/entity.c -o bin/MLVEngine/game.entity.o $(CFLAGS) -lMLV -ldl

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
