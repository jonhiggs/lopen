VERSION := 0.0.4

target/lopen: lib/functions.inc bin/lopen .FORCE
	echo '#!/usr/bin/env bash' >$@
	cat $(word 1,$^) >>$@
	echo VERSION=${VERSION} >>$@
	cat $(word 2,$^) | awk '/### END FRONTMATTER ###/ { n=NR }; (n&&n<NR)' >>$@
	chmod +x ${@}
	shellcheck $@ -x -a

.PHONY: test
test:
	./test/test
	./test/test_file
	./test/test_mailcap
	./test/test_integration
	shellcheck ./bin/lopen -x -a

.FORCE:
