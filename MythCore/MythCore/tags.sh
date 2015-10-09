#! /bin/sh
#生成工程的tag文件

ctags -R --languages=c++ --langmap=c++:+.inl -h +.inl --c++-kinds=+px --fields=+aiKSz --extra=+q -f MythCoretags
