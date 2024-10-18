.POSIX:

CXX := g++
CXX_FLAGS := -std=c++17 -g

LEAK := valgrind --tool=memcheck --trace-children=yes --track-origins=yes --leak-check=full --show-leak-kinds=all
OPEN := xdg-open
TAR := tar
APP_NAME := 3DViewer
APP_FILE := $(APP_NAME)
PATH_INSTALL = $(HOME)/$(APP_NAME)

ifeq ($(shell uname -s), Darwin)
	LEAK := leaks -atExit --
	OPEN := open
	APP_FILE := $(APP_NAME).app
endif

all: clean src/$(APP_FILE)

clean:
	rm -f src/moc_*.cpp src/moc_*.h src/*.o
	rm -f src/Makefile src/.qmake.stash src/$(APP_FILE)
	rm -f $(APP_NAME).tar.gz
	rm -f *.gcda *.gcno res.info
	rm -f *.o *.a tests
	rm -rf report
	rm -rf docs

src/$(APP_FILE):
	cd src && qmake6 && make -j

install: src/$(APP_FILE)
	mkdir -p "$(PATH_INSTALL)"
	mv -f $< "$(PATH_INSTALL)/"

uninstall:
	rm -f "$(PATH_INSTALL)/$(APP_FILE)"
	rm -f "$(PATH_INSTALL)/settings.ini"
	rm -df "$(PATH_INSTALL)"

TAR_OPT := --numeric-owner --mtime='2024-01-01T00:00Z' --owner=0 --group=0
define TAR_DEF
	$(info # tar $@)
	$(TAR) -c $(TAR_OPT)$(if $(1), --transform='s;^;$(1)/;') -af $@ $^
endef

dist: clean $(APP_NAME).tar.gz

$(APP_NAME).tar.gz: src src/back src/front libs models test test/models Doxyfile Makefile LICENSE README.md .clang-format
	$(call TAR_DEF,$(APP_NAME))

html:
	doxygen
	$(OPEN) ./docs/html/index.html

tests: test/test.cc src/back/Model.cc src/back/Controller.cc src/back/AffineApplier.cc src/back/ObjectFile.cc
	$(CXX) $(CXX_FLAGS) --coverage $^ -o tests `pkg-config --cflags --libs gtest`

gcov_report: tests
	./tests
	lcov --no-external --ignore-errors inconsistent,usage -t "result" -o res.info -c -d .
	genhtml -o report res.info
	$(OPEN) report/index.html

clang-format: src/main.cc src/back/* src/front/* test/*.cc
	make clean
	-clang-format --verbose -n $^
	-clang-format --verbose -i $^

leaks: tests
	CK_FORK=no $(LEAK) ./tests

.PHONY: all install uninstall clean html dist tests gcov_report clang-format leaks