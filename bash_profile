# 开启ls的颜色设置
export CLICOLOR=1
export LSCOLORS=gxbxDxdxfxegedabagacad
export GREP_OPTIONS='--color=always'

export DYLD_LIBRARY_PATH="$DYLD_LIBRARY_PATH:/usr/local/mysql/lib/"

export LDFLAGS="-L/usr/local/opt/libffi/lib"

alias ll="ls -l"
alias find="mdfind"
alias cat="bat"
alias toilet="toilet -f mono12 -F gay"
alias diff="colordiff"

# 每次开启终端时运行powerline.sh
. /Library/Python/2.7/site-packages/powerline/bindings/bash/powerline.sh
