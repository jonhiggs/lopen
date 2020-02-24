# these functions expect a file as input
LIB_DIR="$(dirname "${BASH_SOURCE[0]}")"

# shellcheck source=./lib/mailcap.inc
source "${LIB_DIR}/mailcap.inc"

uri_cmd() {
  m=$(file_mimetype "$1")
  c="$(mailcap_cmd "$m")"
  c="$(echo "$c" | awk -v f="$1" '{gsub(/%s/,f,$0)}; {print}')"
  c="$(echo "$c" | awk -v m="$m" '{gsub(/%t/,m,$0)}; {print}')"
  [[ $(mailcap_flags "$m") =~ copiousoutput ]] && c+="|${LOPEN_PAGER}"
  echo "$c"
}

_uri_valid_scheme() {
  for s in "${LOPEN_SCHEMES[@]}"; do
    [[ $s == "$1" ]] && return 0
  done
  return 1
}

_uri_scheme() {
  if [[ $1 =~ ^[a-z]+:// ]]; then
    echo "$1" | cut -d: -f1
  elif [[ ! $1 =~ ^[a-z]+:// ]]; then
    echo "http"
  fi
}
