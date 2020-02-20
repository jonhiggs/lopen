target/lopen: lib/functions.inc bin/lopen .FORCE
	echo '#!/usr/bin/env bash' >$@
	cat $(word 1,$^) >>$@
	cat $(word 2,$^) | awk '/### END FRONTMATTER ###/ { n=NR }; (n&&n<NR)' >>$@
	shellcheck $@

.PHONY: test
test:
	./test/test

.FORCE:
