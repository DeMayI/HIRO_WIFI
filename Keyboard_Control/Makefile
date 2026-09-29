# This is a very fancy makefile. Don't touch it. It works like magic, even if
# you add more .cpp files and .hpp files to this project. It also
# auto-generates dependency lists as a side effect of compilation, so you'll
# basically never need to run `make clean`. The `clean` target is still there
# in case you want to use it, though.

CXX = g++ -g
DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d

# libraries
SQLITE_FLAGS = -lsqlite3
ALLEGRO_CFLAGS = $(shell pkg-config --cflags allegro-5 allegro_image-5 \
                                            allegro_font-5 allegro_primitives-5 \
                                            allegro_ttf-5)
ALLEGRO_LIBS   = $(shell pkg-config --libs   allegro-5 allegro_image-5 \
                                            allegro_font-5 allegro_primitives-5 \
                                            allegro_ttf-5)

EXE = HIRO_Keyboard_Client

SRC_DIR = .
SRC = $(shell find $(SRC_DIR) -regex ".*\.cpp" -not -path "./depreciatedCode/*")

OBJ_DIR = .obj
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

DEP_DIR = .deps
DEP = $(patsubst $(SRC_DIR)/%.cpp,$(DEP_DIR)/%.d,$(SRC))

DATA_DIR   = data
DATA_FILES = $(wildcard $(DATA_DIR)/*)

# executable depends on objects; data files are order-only so they trigger rebuilds but
# are not passed to the linker
$(EXE): $(OBJ) | $(DATA_FILES)
	$(CXX) $(ALLEGRO_CFLAGS) -o $@ $(OBJ) $(SQLITE_FLAGS) $(ALLEGRO_LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d \
                 $(OBJ_DIR)/%.o.sentinel $(DEP_DIR)/%.d.sentinel
	$(CXX) $(ALLEGRO_CFLAGS) $(DEP_FLAGS) -I . -c -o $@ $<


$(OBJ_DIR)/%.sentinel:
	@mkdir -p ${@D}
	@touch $@

$(DEP_DIR)/%.sentinel:
	@mkdir -p ${@D}
	@touch $@

$(DEP):

include $(wildcard $(DEP))

clean:
	rm -rf $(OBJ_DIR) $(DEP_DIR) $(EXE)

.PHONY: clean insert_admin


