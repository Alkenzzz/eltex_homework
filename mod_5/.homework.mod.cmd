savedcmd_/home/user/mod_5/homework.mod := printf '%s\n'   homework.o | awk '!x[$$0]++ { print("/home/user/mod_5/"$$0) }' > /home/user/mod_5/homework.mod
