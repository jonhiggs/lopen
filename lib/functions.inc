dump_config() {
  echo "ENVIRONMENT:"
  ( echo "LOPEN_BROWSER:	${LOPEN_BROWSER}"
    echo "LOPEN_CONFIG:	${LOPEN_CONFIG}"
    echo "LOPEN_CONFIG_DIR:	${LOPEN_CONFIG_DIR}"
    echo "LOPEN_DRY:	${LOPEN_DRY}"
    echo "LOPEN_MAILCAP_FILE:	${LOPEN_MAILCAP_FILE}"
    echo "LOPEN_SCHEMES:	${LOPEN_SCHEMES[*]}"
    echo "LOPEN_SHOW_COMMAND:	${LOPEN_SHOW_COMMAND}"
    echo "LOPEN_SHOW_MIMETYPE:	${LOPEN_SHOW_MIMETYPE}"
  ) | column -t -s"	"

  echo -e "\nMAILCAP FILE:"
  cat ${LOPEN_MAILCAP_FILE}
}

input_type() {
  if [[ -f "$1" ]]; then
    echo "file"
    return 0
  elif [[ "$1" =~ ^[a-z]+:\/\/ ]]; then
    for s in "${LOPEN_SCHEMES[@]}"; do
      if [[ "$s" == $(echo "$1" | awk -F'://' '{print $1}') ]]; then
        echo "$s"
        return 0
      fi
    done
    echo "unknown protocol" >&2
  fi
  return 1
}

forced_mimetype() {
  t=$(awk -v E=".${1##*.}" '($1==E) {print $2}' < "${LOPEN_FILETYPES}")
  [[ -z "$t" ]] && return 1
  echo "$t"
}

mimetype() {
  forced_mimetype "$1" && return 0
  mimetype=$(file --mime-type "$1" | awk -F: '{gsub(/^ +/,"", $2);print $2}')
  extension=${1##*.}

  case ${mimetype} in
    "text/plain")
      case ${extension} in
        md|markdown|mkd) mimetype='text/markdown' ;;
      esac
      ;;
    "inode/symlink") mimetype=$(mimetype "$(readlink -f "$1")") ;;
  esac

  echo "${mimetype}"
}

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

cmd() {
  local t
  t=$(input_type "$1" 2> /dev/null)
  [[ -z $t ]] && return 1
  if [[ $t == "file" ]]; then
    mailcap_cmd "$(mimetype "$1")" \
      | awk -v file="$1" '{gsub(/%s/,file,$0)}; {print}'
  else
    echo "${BROWSER} $1"
  fi
}

unwrap_file() {
  local f
  f=$1
  while [[ -S "$f" ]]; do f=$(readlink "$f"); done
  [[ -f $f ]] || return 1
  echo "$f"
}
