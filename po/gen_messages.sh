export PATH="$PATH:$PWD"
cd ../../../liblicense-kde/trunk
mkdir po
../../liblicense/trunk/po/extract-messages.sh
cp po/liblicense.pot ../../liblicense/trunk/po/
cd -
xgettext --keyword=_:1 -j -o liblicense.pot ../liblicense/*.c ../liblicense/*.h
xgettext -j -o liblicense.pot ../../../liblicense-gnome/trunk/src/*.py
