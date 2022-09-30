# Template generated code from trgen 0.17.0
default: setup
	rm -rf build; mkdir build; cd build; cmake .. ; $(MAKE)
setup:
	if [ -f transformGrammar.py ]; then python3 transformGrammar.py ; fi
run:
	cd build; trwdog ./Test $(RUNARGS)
clean:
	rm -rf build /tmp/antlr4_runtime
test:
	bash test.sh
