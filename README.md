# readable

count = 0

open the specified directory

go through files ntill null

skip . and ..

if file is symbolic link skip

if file is a dir then run readable on it and:
    count += readable(dir)

if file is a file: check if readable and if it is count += 1

return count.

# fully recursive

start:

check if null: open the current dir and save that in current.




