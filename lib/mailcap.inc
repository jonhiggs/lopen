# these functions expect a mimetype as input

mailcap_cmd() {
  awk -F\; -v M="$1" \
    '($1~M) {gsub(/^ +/, "", $2); print $2}' \
    < "${LOPEN_MAILCAP_FILE}"
}

mailcap_flags() {
  awk -F\; -v M="$1" \
    '($1~M) {gsub(/^ +/, "", $3); print $3}' \
    < "${LOPEN_MAILCAP_FILE}"
}


