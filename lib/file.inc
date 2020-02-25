# these functions expect a file as input
LIB_DIR="$(dirname "${BASH_SOURCE[0]}")"

source "${LIB_DIR}/realpath.inc"

# shellcheck source=./lib/mailcap.inc
source "${LIB_DIR}/mailcap.inc"

file_cmd() {
  f="$(realpath "$1")"
  m=$(file_mimetype "$f")
  c="$(mailcap_cmd "$m")"
  c="$(echo "$c" | awk -v f="$f" '{gsub(/%s/,f,$0)}; {print}')"
  c="$(echo "$c" | awk -v m="$m" '{gsub(/%t/,m,$0)}; {print}')"
  [[ $(mailcap_flags "$m") =~ copiousoutput ]] && c+="|${LOPEN_PAGER}"
  echo "$c"
}

file_mimetype() {
  for f in ${LOPEN_MIMETYPES} "${HOME}/.mime.types" "/etc/mimetypes"; do
    [[ -f "$f" ]] && cat $f
  done | awk -v ext="${1##*.}" \
      '/^[^#]/ {
        for (i=2; i<=NF; ++i) {
          if ($i==ext) { print $1; exit 0 }
        }
      }'
}
