SHORT_TASKS = $(wildcard task-s*)

.PHONY: short_tasks

short_tasks:
	$(MAKE) -C $(SHORT_TASKS)
