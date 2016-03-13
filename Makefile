SHORT_TASKS_DIRS = $(wildcard task-s*:%=build-%)
LONG_TASKS_DIRS = $(wildcard task-l*:%=build-%)
CLEAN_DIRS = $(SHORT_TASKS_DIRS:%=clean-%)

all: short_tasks long_tasks

short_tasks: $(SHORT_TASKS_DIRS)

long_tasks: $(LONG_TASKS_DIRS)

$(SHORT_TASKS_DIRS):
	$(MAKE) -C $(@:build-%=%)

$(LONG_TASKS_DIRS):
	$(MAKE) -C $(@:build-%=%)

clean: $(CLEAN_DIRS)
$(CLEAN_DIRS):
	$(MAKE) clean -C  $(@:clean-%=%)

.PHONY: all short_tasks long_tasks $(SHORT_TASKS_DIRS) $(LONG_TASKS_DIRS) $(CLEAN_DIRS) clean
