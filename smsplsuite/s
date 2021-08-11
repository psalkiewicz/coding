#!/bin/sh
PROMPT_COMMAND="echo -n 'C:'; pwd | tr '/' '\\\\' | tr [:lower:] [:upper:] | tr -d '\n'; echo -n '> '"
PS1=""
uname() {
  case $1 in
    -a) echo "Windows 2000 `/bin/uname -n` 5.00.2195 #1 `date` `/bin/uname -m`" ;;
    -r) echo "5.00.2195" ;;
    -s) echo "Windows 2000" ;;
    *) `/bin/uname $*` ;;
  esac
}
