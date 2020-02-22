# these functions expect a file as input

#unwrap_file() {
#  local f
#  f=$1
#  while [[ -S "$f" ]]; do f=$(readlink "$f"); done
#  [[ -f $f ]] || return 1
#  echo "$f"
#}

file_mimetype() {
  local mimetype
  local extension

  forced_mimetype "$1" && return 0
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
