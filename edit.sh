#!/bin/bash
# Generates a vim script to open all C files and split them with their headers

printf "" > edit.vim

split_cmd=${1:-"vsplit"} # split command override in $1 -- vsplit defailt
open_h=''
open_cmd='o'

# Open C files and their corresponding headers
for i in $(ls | grep '\.c$')
do
    echo "$open_cmd $i" >> edit.vim
    
    # Switch open command to tab after first file
    open_cmd='tabe'

    # Split corresponding headers with C files
    header=$(printf "$i" | sed 's/\w$/h/')
    if [ -r $header ]
    then
        echo "$split_cmd $header" >> edit.vim
        open_h="$open_h $header"
    fi
done

# Open standalone header files
for i in $(ls | grep '\.h$')
do
    printf "$open_h" | grep "$i" 2>&1 1>/dev/null
    
    if [ $? -eq 1 ]
    then 
        echo "tabe $i" >> edit.vim 
    fi
done

vim -c "so edit.vim"
#tmux new-session -d "vim -c \"so edit.vim\""
#tmux split-window 
#tmux attach-session
