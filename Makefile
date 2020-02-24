VERSION := 0.0.4
libs = $(wildcard ./lib/*.inc)

target/lopen: bin/lopen $(libs) .FORCE
	echo '#!/usr/bin/env bash' >$@
	cat $(libs) >>$@
	echo VERSION=${VERSION} >>$@
	cat $< | awk '/### END FRONTMATTER ###/ { n=NR }; (n&&n<NR)' >>$@
	chmod +x $@
	shellcheck $@ -x -a

.PHONY: test
test:
	./test/test
	./test/test_file
	./test/test_mailcap
	./test/test_integration
	shellcheck ./bin/lopen -x -a

.FORCE:
