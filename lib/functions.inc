BROWSER="$(command -v firefox)"
SUPPORTED_SCHEMES=(http https ftp)

input_type() {
  if [[ -f "$1" ]]; then
    echo "file"
  elif [[ "$1" =~ ^[a-z]+:\/\/ ]]; then
    for s in ${SUPPORTED_SCHEMES[@]}; do
      [[ $s == $(echo $1 | awk -F'://' '{print $1}') ]] && echo $s && return 0
    done
    echo "unknown protocol" >&2
    return 1
  else
    return 1
  fi
}

mimetype() {
  mimetype=$(file --mime-type "$1" | awk -F: '{gsub(/^ +/,"", $2);print $2}')
  extension=${1##*.}

  case ${mimetype} in
    "text/plain")
      case ${extension} in
        md|markdown|mkd) mimetype='text/markdown' ;;
      esac
      ;;
    "inode/symlink") mimetype=$(mimetype "$(readlink "$1")") ;;
  esac

  echo "${mimetype}"
}

mailcap_cmd() {
  cat ${MAILCAP_FILE} | awk -F\; -v M="$1" '($1~M) { gsub(/^ +/, "", $2); print $2 }'
}

cmd() {
  local t
  t=$(input_type $1 2> /dev/null)
  [[ -z $t ]] && return 1
  if [[ $t == "file" ]]; then
    mailcap_cmd $(mimetype $1) \
      | awk -v file=$1 '{gsub(/%s/,file,$0)}; {print}'
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

open_url() {
  local p
  p=$(echo "$1" | awk -F: '{ print $1 }') in
  case $p in
    http|https) ${BROWSER} "$1" ;;
    *) echo "Unsupported protocol '$p'." ;;
  esac
}

open_file() {
  local file
  local mimetype

  file=$(unwrap_file "$1")
  mimetype=$(mimetype "${file}")

  case ${mimetype} in
    'text/markdown') get_command ${mimetype} ;;
    *) echo "Unsupported mimetype '${mimetype}'." ;;
  esac
}
