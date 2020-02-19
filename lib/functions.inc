input_type() {
  if [[ -f "$1" ]]; then
    echo "file"
  elif [[ "$1" =~ ^[a-z]+:\/\/ ]]; then
    echo "url"
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

get_command() {
  echo "${mailcap}" | awk -F: -v M='text/markdown' '($1~M) {print $2 }'
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
