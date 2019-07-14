alias ..="cd .."
alias ...="cd ../.."
mcd() { mkdir "$1"; cd "$1" || return; }
alias ls='ls -G'
export LSCOLORS=gxbxDxdxfxegedabagacad
alias la="ls -A"
alias ll="ls -Gl"
alias lh="ls -Glh"
alias grep="grep --color=auto"
alias diff="colordiff"
alias cpp="c++ -std=c++11"
alias py3="python3"
alias hisg="history | grep"
alias tree="tree -L"
alias vibashprof="vim ~/.bash_profile"
alias vibashrc="vim ~/.bashrc"
alias vivimrc="vim ~/.vimrc"
alias srcbashprof="source ~/.bash_profile"
alias toilet="toilet -f mono12 -F gay"
# 连接本地端口
alias teloh="telnet localhost"
# 查看指定端口状态
alias wport="netstat -nap tcp | grep"
# 查看正在监听的所有端口
alias wlport="netstat -nap tcp | grep 'LISTEN'"

# 用mv替换rm，将待删除的文件移到Trash中去
alias rm="trash"
trash() 
{ 
    for arg in "$@" 
    do
        if [ -d "$arg" ]; then
            dir="$HOME/.Trash/$arg"
            if [ -d "$dir" ]; then
                read -p "overwrite $dir ? (y/n [n]) " ans;
                if [ "$ans" == "y" ]; then
                    echo "overwriten $dir";
                    /bin/rm -fr "$dir";
                else
                    continue;
                fi
            fi
        fi
        mv -i "$arg" ~/.Trash/;
    done
}
# 列出Trash中的所有文件
alias rlist="trashlist"
trashlist() { ls -a ~/.Trash/; }
# 从Trash中找回被删除的文件
alias rretrieve="trashretrieve"
trashretrieve() 
{ 
    if [ "$#" -eq 1 ]; then
        mv ~/.Trash/"$1" "$(pwd)";
    else
        echo "usage: [rretrieve file]";
    fi
}
# 清空Trash
alias rclear="trashclear"
trashclear() 
{ 
    /bin/rm -fr ~/.Trash/*;
    # 删除隐藏文件
    files=$(ls -A ~/.Trash);
    for arg in $files
    do
        if [[ "$arg" == \.* ]]; then
            /bin/rm -fr "$HOME/.Trash/$arg";
        fi
    done
}

# 解压包
extract() {
    if [ -f "$1" ] ; then
      case "$1" in
        *.tar.bz2)   tar xjf "$1"     ;;
        *.tar.gz)    tar xzf "$1"     ;;
        *.bz2)       bunzip2 "$1"     ;;
        *.rar)       unrar e "$1"     ;;
        *.gz)        gunzip "$1"      ;;
        *.tar)       tar xf "$1"      ;;
        *.tbz2)      tar xjf "$1"     ;;
        *.tgz)       tar xzf "$1"     ;;
        *.zip)       unzip "$1"       ;;
        *)     echo "'$1' cannot be extracted via extract()" ;;
         esac
     else
         echo "'$1' is not a valid file"
     fi
}

# 进程能打开的最大文件描述符数
ulimit -n 10240

export DYLD_LIBRARY_PATH="$DYLD_LIBRARY_PATH:/usr/local/mysql/lib/"

export LDFLAGS="-L/usr/local/opt/libffi/lib"

# 每次开启终端时运行powerline.sh
. /Library/Python/2.7/site-packages/powerline/bindings/bash/powerline.sh

# Setting PATH for Python 3.7
# The original version is saved in .bash_profile.pysave
export PATH="/Library/Frameworks/Python.framework/Versions/3.7/bin:${PATH}"

# icu4c
export PATH="/usr/local/opt/icu4c/bin:$PATH"
export PATH="/usr/local/opt/icu4c/sbin:$PATH"
export LDFLAGS="-L/usr/local/opt/icu4c/lib"
export CPPFLAGS="-I/usr/local/opt/icu4c/include"
