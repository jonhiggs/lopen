VERSION := 0.0.0

target/lopen: lib/functions.inc bin/lopen .FORCE
	echo '#!/usr/bin/env bash' >$@
	cat $(word 1,$^) >>$@
	echo VERSION=${VERSION} >>$@
	cat $(word 2,$^) | awk '/### END FRONTMATTER ###/ { n=NR }; (n&&n<NR)' >>$@
	chmod +x ${@}
	shellcheck $@

.PHONY: test
test:
	./test/test

.FORCE:
