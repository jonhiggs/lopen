# these functions expect a file as input
source $(dirname "${BASH_SOURCE[0]}")/mailcap.inc

uri_cmd() {
  m=$(file_mimetype "$1")
  c="$(mailcap_cmd "$m")"
  c="$(echo "$c" | awk -v f="$1" '{gsub(/%s/,f,$0)}; {print}')"
  c="$(echo "$c" | awk -v m="$m" '{gsub(/%t/,m,$0)}; {print}')"
  [[ $(mailcap_flags "$m") =~ copiousoutput ]] && c+="|${LOPEN_PAGER}"
  echo "$c"
}

_uri_valid_scheme() {
  for s in ${LOPEN_SCHEMES[@]}; do
    [[ $s == "$1" ]] && return 0
  done
  return 1
}

_uri_scheme() {
  if [[ $1 =~ ^[a-z]+:// ]]; then
    echo $1 | cut -d: -f1
  elif [[ ! $1 =~ ^[a-z]+:// ]]; then
    echo "http"
  fi
}

file_mimetype() {
  local mimetype
  local extension

  _file_forced_mimetype "$1" && return 0
  mimetype=$(file --mime-type "$1" | awk -F: '{gsub(/^ +/,"", $2);print $2}')
  extension=${1##*.}

  # TODO: is own function name available in a variable?
  case ${mimetype} in
    "text/plain")
      case ${extension} in
        md|markdown|mkd) mimetype='text/markdown' ;;
      esac
      ;;
    "inode/symlink") mimetype=$(file_mimetype "$(readlink -f "$1")") ;;
  esac

  echo "${mimetype}"
}

_file_forced_mimetype() {
  t=$(awk -v E=".${1##*.}" '($1==E) {print $2}' < "${LOPEN_FILETYPES}")
  [[ -z "$t" ]] && return 1
  echo "$t"
}
