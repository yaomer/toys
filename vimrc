"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                   通用设置                                      "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
colorscheme monokai         " 主题
syntax on                   " 开启语法高亮
filetype on                 " 开启文件类型检测
filetype plugin on          " 设置加载对应文件类型的插件
set encoding=utf-8          " 使用utf-8编码
set t_Co=256                " 启用256色
let mapleader = ";"         " 定义<leader>键
set number                  " 开启行号
set cursorline              " 高亮当前行
set nocompatible            " 关闭vi兼容模式
set scrolloff=3             " 垂直滚动时光标距离底部的位置
set sidescroll=30           " 向右滚动时每次滚动30个字符
set noerrorbells            " 关闭出错响铃
set history=100             " vim需要保存的历史记录条数

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                   代码排版                                      "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
set autoindent              " 自动缩进
set cindent                 " 使用C/C++语言的自动缩进方式
set smartindent             " 智能的选择对其方式
filetype indent on          " 自适应不同语言的智能缩进
set tabstop=4               " 将tab设置为4个空格
set expandtab               " 将tab扩展为空格
set shiftwidth=4            " 每一级缩进为4个空格
set nowrap                  " 禁止折行

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                   搜索设置                                      "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
set hlsearch                " 高亮所有匹配项
set incsearch               " 搜索时逐渐高亮
set ignorecase              " 搜索时对大小写不敏感
set smartcase               " 搜索时若包含大写字母，则对大小写敏感

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                   补全设置                                      "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
set wildmenu                " 命令补全
set completeopt-=preview    " 命令行补全时不显示窗口，只显示补全列表

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                   缓存设置                                      "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
set autoread                " 文件在vim之外修改过，自动重新读入
set confirm                 " 在处理未保存或只读文件的时候，弹出确认
set hidden                  " 允许有未保存的修改时切换缓冲区

set path=.,/Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk/usr/include

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                   插件列表                                      "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
call plug#begin('~/.vim/plugged')

" 延迟加载以下插件
Plug 'Valloric/YouCompleteMe', { 'on': [] }                     " 代码自动补全插件
Plug 'tenfyzhong/CompleteParameter.vim', { 'on': [] }           " 对YCM自动补全的补充，自动补全函数参数
Plug 'w0rp/ale', { 'on': [] }                                   " 异步语法检查
Plug 'Yggdroot/LeaderF', { 'on': [] }                           " 文件模糊搜索工具
Plug 'easymotion/vim-easymotion', { 'on': [] }                  " 快速移动和跳转
Plug 'scrooloose/nerdtree', { 'on': [] }                        " 代码资源管理器(显示树形目录)
Plug 'tiagofumo/vim-nerdtree-syntax-highlight', { 'on': [] }    " nerdtree文件类型高亮
Plug 'xuyuanp/nerdtree-git-plugin', { 'on': [] }                " nerdtree显示git状态
Plug 'majutsushi/tagbar', { 'on': [] }                          " 显示类/方法/变量
Plug 'tpope/vim-surround', { 'on': [] }                         " 操纵配对字符（配合vim的文本对象使用）
Plug 'iamcco/markdown-preview.vim', { 'on': [] }                " markdown实时预览

Plug 'junegunn/vim-slash'                               " 优化搜索，移动光标后清除高亮
Plug 'jiangmiao/auto-pairs'                             " 自动补全括号
Plug 'scrooloose/nerdcommenter'                         " 快速代码注释插件
Plug 'kien/rainbow_parentheses.vim'                     " 彩虹括号
Plug 'vim-airline/vim-airline'                          " 状态栏美化插件
Plug 'vim-airline/vim-airline-themes'                   " airline的主题
Plug 'ryanoasis/vim-devicons'                           " 显示文件类型图标
Plug 'octol/vim-cpp-enhanced-highlight'                 " cpp语法高亮的增强
Plug 'terryma/vim-expand-region'                        " (visual mode)快速扩展区域
Plug 'tpope/vim-repeat'                                 " 重复一个插件的上一次操作（配合vim-surround）

call plug#end()

" 100ms后调用LoadPlug，且只调用一次
call timer_start(100, 'LoadPlug')

function! LoadPlug(timer) abort
    call plug#load('YouCompleteMe')
    call plug#load('CompleteParameter.vim')
    call plug#load('ale')
    call plug#load('LeaderF')
    call plug#load('vim-easymotion')
    call plug#load('nerdtree')
    call plug#load('vim-nerdtree-syntax-highlight')
    call plug#load('nerdtree-git-plugin')
    call plug#load('tagbar')
    call plug#load('vim-surround')
    call plug#load('markdown-preview.vim')
endfunction

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                               自定义快捷键                                      "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" 编辑vimrc
nnoremap <leader><leader>e :edit $MYVIMRC<cr>

" 打开当前光标所在单词的vim帮助文档
nnoremap <leader><leader>h :execute ":help " . expand("<cword>")<cr>

" 打开文件自动定位到最后编辑的位置
autocmd BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | execute "normal! g'\"" | endif

" 每次保存.c文件时自动删除尾随空格
autocmd BufWritePre *.c :%s/\s\+$//e

" 安装、更新、删除插件
nnoremap <leader><leader>i :PlugInstall<cr>
nnoremap <leader><leader>u :PlugUpdate<cr>
nnoremap <leader><leader>c :PlugClean<cr>

" buffer切换
nnoremap <c-p> :bp<cr>
nnoremap <c-n> :bn<cr>
nnoremap <leader>d :bd<cr>

" 跳转到配对的符号处
nmap <space> %

" 查看graphviz下几种不同命令渲染生成的图片
nmap <F2>1 :w<CR>:!dot    -Tpng -o %<.png % && open %<.png<CR>
nmap <F2>3 :w<CR>:!circo  -Tpng -o %<.png % && open %<.png<CR>

" <F5>一键编译运行c、cpp、python和sh
nmap <F5> :call CompileRun()<cr>

func! CompileRun()
    exec "w"
    if &filetype == 'c'
        exec '!cc % -o /tmp/a.out && /tmp/a.out'
    elseif &filetype == 'cpp'
        exec '!c++ % -o /tmp/a.out && /tmp/a.out'
    elseif &filetype == 'python'
        exec '!py3 %'
    elseif &filetype == 'sh'
        :!bash %
    endif
endfunc

let g:cpp_class_scope_highlight = 1

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                   airline                                       "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
let g:airline_theme="base16_monokai"
let g:airline_powerline_fonts = 1
let g:airline#extensions#tabline#enabled = 1
if !exists('g:airline_symbols')
    let g:airline_symbols = {}
endif
let g:airline_left_sep      = ''
let g:airline_left_alt_sep  = ''
let g:airline_right_sep     = ''
let g:airline_right_alt_sep = ''

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                   nerdtree                                      "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
nnoremap <silent><leader>n :NERDTreeToggle<cr>
let g:NERDTreeFileExtensionHighlightFullName    = 1
let g:NERDTreeExactMatchHighlightFullName       = 1
let g:NERDTreePatternMatchHighlightFullName     = 1
let g:NERDTreeHighlightFolders                  = 1
let g:NERDTreeHighlightFoldersFullName          = 1
let g:NERDTreeDirArrowExpandable                = '▷'
let g:NERDTreeDirArrowCollapsible               = '▼'

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                  nerdcommenter                                  "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
let g:NERDSpaceDelims               = 1
let g:NERDCompactSexyComs           = 1
let g:NERDDefaultAlign              = 'left'
let g:NERDCommentEmptyLines         = 1
let g:NERDTrimTrailingWhitespace    = 1
let g:NERDToggleCheckAllLines       = 1

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                               rainbow_parentheses                               "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
let g:rbpt_colorpairs = [
    \ ['darkgreen',   'firebrick3'],
    \ ['red',         'firebrick3'],
    \ ['darkmagenta', 'DarkOrchid3'],
    \ ['darkcyan',    'RoyalBlue3'],
    \ ['darkgreen',   'RoyalBlue3'],
    \ ['darkmagenta', 'DarkOrchid3'],
    \ ['darkcyan',    'SeaGreen3'],
    \ ]
let g:rbpt_max = 16
let g:rbpt_loadcmd_toggle = 0
au VimEnter * RainbowParenthesesToggle
au Syntax * RainbowParenthesesLoadRound
au Syntax * RainbowParenthesesLoadSquare
au Syntax * RainbowParenthesesLoadBraces

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                               nerdtree-git-plugin                               "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
let g:NERDTreeIndicatorMapCustom = {
            \ "Modified"  : "✹",
            \ "Staged"    : "✚",
            \ "Untracked" : "✭",
            \ "Renamed"   : "➜",
            \ "Unmerged"  : "═",
            \ "Deleted"   : "✖",
            \ "Dirty"     : "✗",
            \ "Clean"     : "✔︎",
            \ 'Ignored'   : '☒',
            \ "Unknown"   : "?"
            \ }

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                   LeaderF                                       "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
nnoremap <silent><leader>f :LeaderfFile<cr>
let g:Lf_WildIgnore = {
            \ 'dir': ['.svn','.git','.hg','.vscode','.wine','.deepinwine','.oh-my-zsh'],
            \ 'file': ['*.sw?','~$*','*.bak','*.exe','*.o','*.so','*.py[co]']
            \}
let g:Lf_UseCache = 0

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                   tagbar                                        "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
let g:tagbar_width = 30
nnoremap <silent><leader>t :TagbarToggle<cr>

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                 YouCompleteMe                                   "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
let g:ycm_server_python_interpreter='/usr/bin/python'
let g:ycm_global_ycm_extra_conf = '~/.vim/plugged/YouCompleteMe/third_party/ycmd/examples/.ycm_extra_conf.py'
" 让vim的补全菜单行为与一般IDE一致
set completeopt=longest,menu
" 触发(符号)补全时需要键入的字符数
let g:ycm_min_num_of_chars_for_completion = 1
" 补全候选项的最小字符数
let g:ycm_min_num_identifier_candidate_chars = 0
" 关闭诊断显示功能(已经使用了ale进行异步语法检查)
let g:ycm_show_diagnostics_ui = 0
" 在字符串和注释中仍会触发补全
let g:ycm_complete_in_comments = 1
let g:ycm_complete_in_strings = 1
" 禁止缓存匹配项，每次重新生成匹配项
let g:ycm_cache_omnifunc = 0
" 在用户接受提供的完成字符串后自动关闭窗口
let g:ycm_autoclose_preview_window_after_completion = 0
" 跳转到声明或定义处
nnoremap <silent><leader>gj :YcmCompleter GoToDefinitionElseDeclaration<cr>

" 自动触发语义补全
let g:ycm_semantic_triggers =  {
            \ 'c,cpp,python,java,go,erlang,perl': ['re!\w{1}'],
            \ 'cs,lua,javascript': ['re!\w{1}'],
            \ }
" 遇到下列文件时才会开启YCM
let g:ycm_filetype_whitelist = {
            \ "c":1,
            \ "cpp":1,
            \ "python":1,
            \ "sh":1,
            \ }

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                   ale                                           "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
let g:ale_set_highlights = 0
let g:ale_sign_error = '✗'
let g:ale_sign_warning = '⚡'
" 在vim自带的状态栏中整合ale
let g:ale_statusline_format = ['✗ %d', '⚡ %d', '✔ OK']
" 显示Linter名称，出错或警告等相关信息
let g:ale_echo_msg_error_str = 'E'
let g:ale_echo_msg_warning_str = 'W'
let g:ale_echo_msg_format = '[%linter%] %s [%severity%]'
" 打开文件时不进行检查
let g:ale_lint_on_enter = 0
" 普通模式下，sp前往上一个错误或警告，sn前往下一个错误或警告
nmap ap <Plug>(ale_previous_wrap)
nmap an <Plug>(ale_next_wrap)
" 触发/关闭语法检查
nmap <silent><leader>at :ALEToggle<CR>
" 查看错误或警告的详细信息
nmap <silent><leader>ad :ALEDetail<CR>
" 使用clang对c和c++进行语法检查
let g:ale_linters = {
            \   'c': ['clang'],
            \   'cpp': ['clang'],
            \}

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                               CompleteParameter.vim                             "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
inoremap <silent><expr> ( complete_parameter#pre_complete("()")
smap <c-j> <Plug>(complete_parameter#goto_next_parameter)
imap <c-j> <Plug>(complete_parameter#goto_next_parameter)
smap <c-k> <Plug>(complete_parameter#goto_previous_parameter)
imap <c-k> <Plug>(complete_parameter#goto_previous_parameter)

" 消除与auto-pairs的冲突
let g:AutoPairs = {'[':']', '{':'}', "'":"'", '"':'"', '`':'`'}
inoremap <buffer><silent> ) <C-R>=AutoPairsInsert(')')<CR>

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                                   vim-easymotion                                "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
map <Leader> <Plug>(easymotion-prefix)
let g:EasyMotion_do_mapping = 0

map <leader>j <Plug>(easymotion-j)
map <leader>k <Plug>(easymotion-k)
map <leader>h <Plug>(easymotion-linebackward)
map <leader>l <Plug>(easymotion-lineforward)
map <leader>s <Plug>(easymotion-s)
map <leader>w <Plug>(easymotion-bd-w)

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"                               markdown-preview.vim                              "
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
let g:mkdp_path_to_chrome = "/Applications/Google\\ Chrome.app/Contents/MacOS/Google\\ Chrome"
nmap <silent><F3> <Plug>MarkdownPreview
nmap <silent><F4> <Plug>StopMarkdownPreview
