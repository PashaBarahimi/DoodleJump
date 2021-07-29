PATH_SRC   := src
PATH_LIB   := lib
PATH_BUILD := build

ifeq ($(OS),Windows_NT)
	PATH_INC   := $(PATH_SRC)\include
	PATH_BIN   := $(PATH_BUILD)\bin
	PATH_OBJ   := $(PATH_BUILD)\obj
else
	PATH_INC   := $(PATH_SRC)/include
	PATH_BIN   := $(PATH_BUILD)/bin
	PATH_OBJ   := $(PATH_BUILD)/obj
endif

include vars.mk

LDFLAGS =

ifeq ($(OS),Windows_NT)
	CPPFLAGS += -I "$(PATH_INC)"
	LDFLAGS += -L "$(PATH_LIB)"
endif

LDLIBS  += -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_mixer

ifeq ($(OS),Windows_NT)
	OUTPUT = DoodleJump.exe
else
	OUTPUT = DoodleJump.out
endif

#--------------------------------------

vpath %.hpp $(PATH_INC)
vpath %.cpp $(PATH_SRC)

OBJECTS := breakable_platform.o doodle_jump.o doodler.o enemy.o game_board.o game_play.o menu.o movable_enemy.o movable_platform.o normal_enemy.o normal_platform.o pistol.o platform.o rsdl.o spaceship.o spring.o

#--------------------------------------

all: $(PATH_BUILD) $(PATH_BIN) $(PATH_OBJ) $(PATH_BIN)/$(OUTPUT) copy-output

$(PATH_BIN)/$(OUTPUT): $(addprefix $(PATH_OBJ)/, $(OBJECTS))
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

copy-output: $(PATH_BIN)/$(OUTPUT)
	$(COPY) $(PATH_BIN)/$(OUTPUT) .

$(PATH_OBJ)/breakable_platform.o: breakable_platform.cpp breakable_platform.hpp platform.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/doodle_jump.o: doodle_jump.cpp menu.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/doodler.o: doodler.cpp doodler.hpp rsdl.hpp game_board.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/enemy.o: enemy.cpp enemy.hpp rsdl.hpp doodler.hpp game_board.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/game_board.o: game_board.cpp game_board.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/game_play.o: game_play.cpp game_play.hpp breakable_platform.hpp doodler.hpp enemy.hpp game_board.hpp movable_enemy.hpp movable_platform.hpp normal_enemy.hpp normal_platform.hpp pistol.hpp platform.hpp rsdl.hpp spring.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/menu.o: menu.cpp menu.hpp game_play.hpp platform.hpp  normal_platform.hpp spaceship.hpp doodler.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/movable_enemy.o: movable_enemy.cpp movable_enemy.hpp enemy.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/movable_platform.o: movable_platform.cpp movable_platform.hpp platform.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/normal_enemy.o: normal_enemy.cpp normal_enemy.hpp enemy.hpp 
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/normal_platform.o: normal_platform.cpp normal_platform.hpp platform.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/pistol.o: pistol.cpp pistol.hpp rsdl.hpp game_board.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/platform.o: platform.cpp platform.hpp rsdl.hpp doodler.hpp game_board.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/rsdl.o: rsdl.cpp rsdl.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/spaceship.o: spaceship.cpp spaceship.hpp rsdl.hpp game_board.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PATH_OBJ)/spring.o: spring.cpp spring.hpp rsdl.hpp doodler.hpp game_board.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

#--------------------------------------

$(PATH_BUILD): ; $(MKDIR) $@
$(PATH_BIN): ; $(MKDIR) $@
$(PATH_OBJ): ; $(MKDIR) $@

.PHONY: all clean run

clean:
	$(RMDIR) $(PATH_BUILD) > $(NULL_DEVICE)
	$(RM) $(OUTPUT) > $(NULL_DEVICE)

run:
	./$(OUTPUT)
