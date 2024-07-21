echo $1 >hoge && nasm -f elf64 -o /dev/null -l /dev/stdout hoge
#echo -n "0x"
#rasm2 -a elf64 "$1" | sed -e 's/\(..\)/\1,0x/g'