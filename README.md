# lopen (WIP)

Like the MacOS `open` command, but for Linux.

Takes a [mailcap](http://manpages.ubuntu.com/manpages/xenial/man5/mailcap.5.html) format configuration, and opens file with the appropriate application.


## Configuration

`${LOPEN_BROWSER}` is used to open URLs

`${SUPPORTED_SCHEMES}` are schemes that are sent to `${BROWSER}`

`${MAILCAP_FILE}` is where you define the app/mimetype association



## Building

1. Run `make`
2. Drop `target/lopen` into `/usr/local/bin/` or somewhere else in your `${PATH}`.
