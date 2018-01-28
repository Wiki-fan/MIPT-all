ASTYLEFLAGS = --style=ansi

SHORT_TASKS_DIRS = $(wildcard task-s*)
LONG_TASKS_DIRS =$(wildcard task-l*)
SHORT_TASKS = $(SHORT_TASKS_DIRS:%=build-%)
LONG_TASKS = $(LONG_TASKS_DIRS:%=build-%)
CLEAN_DIRS = $(SHORT_TASKS_DIRS:%=clean-%) $(LONG_TASKS_DIRS:%=clean-%) $(CONSPECTS_DIR:%=clean-%) 
CONSPECTS_DIR = conspects-tex
SRC = $(wildcard */*.c *.c */*.h *.h)

.PHONY:  all clean $(CLEAN_DIRS) short_tasks $(SHORT_TASKS) long_tasks $(LONG_TASKS) conspects $(CONSPECTS_DIR)

all: short_tasks long_tasks conspects

short_tasks: $(SHORT_TASKS)

long_tasks: $(LONG_TASKS)

$(CLEAN_DIRS):
	$(MAKE) clean -C  $(@:clean-%=%)

$(SHORT_TASKS):
	$(MAKE) -C $(@:build-%=%)

$(LONG_TASKS):
	$(MAKE) -C $(@:build-%=%)

style: $(SRC)
	astyle $(ASTYLEFLAGS) $^

conspects: $(CONSPECTS_DIR)
	$(MAKE) -C $(CONSPECTS_DIR)

clean: $(CLEAN_DIRS)
