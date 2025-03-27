
# Nom de l'exécutable
TARGET = orchestrator



# Détection de l'OS
ifeq ($(OS),Windows_NT)
    TARGET_OS := windows
else
    TARGET_OS := linux
endif

# Définition du compilateur
ifeq ($(TARGET_OS),windows)
    CXX = g++
    STD = -std=c++17
    # cd raylib/src
    # make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=STATIC
    # RM = del /Q /F
    RM =  rm -rf
    LDFLAGS = -Lraylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
else
    CXX = clang++
    STD = -std=c++17
    RM = rm -rf
    LDFLAGS = -L./raylib/src -lraylib -ldl -lpthread -lX11
endif


# Options de compilation
#CXXFLAGS = $(STD) -Wall -Wextra -Werror -O2 -I../raylib/src
###CXXFLAGS = $(STD)  -Wextra -Werror -O2 -I../raylib/src
CXXFLAGS = $(STD)  -O2 -I../raylib/src

# Répertoire des fichiers objets
OBJ_DIR = obj

# Fichiers source
SRC =	src/main.cpp \
		src/Car.cpp \
		src/Game.cpp \
		src/Game_draw.cpp \
		src/Game_open_save_map.cpp \
		src/Game_ui.cpp \
		src/Game_update.cpp \
		src/Lane.cpp \
		src/Node.cpp \
		src/Road.cpp \
		src/utils_config.cpp \
		src/utils_draw.cpp \
		src/utils_files.cpp \
		src/utils_maths.cpp \

# Gestion des fichiers objets
ifeq ($(TARGET_OS),windows)
    OBJ = $(SRC:.cpp=.o)
else
    OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))
endif




# Compilation des fichiers objets
ifeq ($(TARGET_OS),windows)
    %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
else
    $(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/src
	$(CXX) $(CXXFLAGS) -c $< -o $@
endif


#RAYLIB_LIB = raylib/src/libraylib.a
#$(RAYLIB_LIB):
#	@echo "Compiling Raylib................"
#	@$(MAKE) -C raylib/src PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=STATIC



# Règle par défaut : compile tout
all: $(TARGET)
	@echo " --------------------------------"
	@echo "|  Compilation completed         |"
	@echo "|  Execute :                     |"
	@echo "|   ./orchestrator     (linux)   |"
	@echo "|   .\orchestrator.exe (windows) |"
	@echo " --------------------------------"


# Compilation de l'exécutable
$(TARGET): $(OBJ)
#	$(MAKE) $(RAYLIB_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Nettoyage
clean:


ifeq ($(TARGET_OS),windows)
	@$(RM) ./src/*.o
	#	@$(RM) $(OBJ_DIR)/src/*.o
	#	@$(RM) $(OBJ_DIR)
else
	@rm -f $(OBJ)
endif
	@echo "Object files deleted."

fclean: clean
	@$(RM) $(TARGET)
	@echo "Executable deleted."

re: fclean all

# Cibles Make
.PHONY: all clean fclean re