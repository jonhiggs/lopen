# these functions expect a file as input
source $(dirname "${BASH_SOURCE[0]}")/mailcap.inc

file_cmd() {
  f="$(_file_expand $1)"
  m=$(file_mimetype "$f")
  c="$(mailcap_cmd "$m")"
  c="$(echo "$c" | awk -v f="$f" '{gsub(/%s/,f,$0)}; {print}')"
  c="$(echo "$c" | awk -v m="$m" '{gsub(/%t/,m,$0)}; {print}')"
  [[ $(mailcap_flags "$m") =~ copiousoutput ]] && c+="|${LOPEN_PAGER}"
  echo "$c"
}

file_mimetype() {
  # FIXME: deal with missing mimetype files
  cat ${LOPEN_MIMETYPES} ${HOME}/.mime.types /etc/mime.types \
    | awk -v ext="${1##*.}" \
      '/^[^#]/ {
        for (i=2; i<=NF; ++i) {
          if ($i==ext) { print $1; exit 0 }
        }
      }'
}

_file_expand() {
  if [[ -L $1 ]]; then
    _file_expand $(readlink -f $1)
  else
    readlink -f $1
  fi
}
