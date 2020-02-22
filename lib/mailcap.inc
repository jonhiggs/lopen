# these functions expect a mimetype as input

mailcap_cmd() {
  awk -v M="$1" '
      BEGIN {
        FS=";"
      }

      (M~pattern) { wildcard_result=$2 }
      (M==$1)     { exact_result=$2 }

      END {
        if (exact_result) result=exact_result
        else result=wildcard_result
        gsub(/^ +/, "", result)
        print result
      }
    ' "${LOPEN_MAILCAP_FILE}"
}

mailcap_flags() {
  awk -F\; -v M="$1" \
    '($1~M) {gsub(/^ +/, "", $3); print $3}' \
    < "${LOPEN_MAILCAP_FILE}"
}


