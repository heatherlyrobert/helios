
# get the temporary files out of the equation
make clean

# get the updated version number and description
VER_NUM="$(grep VER_NUM *.h | cut -d\" -f 2)"
VER_TXT="$(grep VER_TXT *.h | cut -d\" -f 2)"
# printf "v%s : %s\n" "$VER_NUM" "$VER_TXT"

# write the changes to the git file
git commit -a -m "${VER_NUM} : ${VER_TXT}"

