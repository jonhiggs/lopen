target/lopen: lib/functions.inc bin/lopen .FORCE
	echo '#!/usr/bin/env bash' > $@
	cat $(word 1,$^) >> $@
	cat $(word 2,$^) | awk '/### END FRONTMATTER ###/ { n=NF }; (n>NF+1)' >> $@
	shellcheck $@

.PHONY: test
test:
	./test/test

.FORCE:
