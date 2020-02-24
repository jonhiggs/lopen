help() {
  echo "$(basename "$0") [options] <file>"
  echo "  -C   dump configuration"
  echo "  -c   show command"
  echo "  -h   help"
  echo "  -m   show mimetype"
  echo "  -n   dry run"
  echo "  -v   verbose"
  echo "  -V   version"
}

version() {
  echo "lopen version ${VERSION}"
  echo "https://www.github.com/jonhiggs/lopen"
}

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
  cat "${LOPEN_MAILCAP_FILE}"
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
