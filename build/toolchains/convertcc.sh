#/bin/sh
arm-none-eabi-gcc `echo $@ | sed -e 's/ \/cygdrive\/\([a-zA-Z]\)\// \1:\//g;s/-I\/cygdrive\/\([a-zA-Z]\)\//-I\1:\//g'`
