export PATH="$PATH:$PWD"
cd ../../../liblicense-kde/trunk
mkdir po
../../liblicense/trunk/po/extract-messages.sh
cp po/liblicense.pot ../../liblicense/trunk/po/
cd -
xgettext --keyword=_:1 -j -o liblicense.pot ../liblicense/*.c ../liblicense/*.h
xgettext --from-code=ascii --copyright-holder="This file copyright Creative Commons (http://www.creativecommons.org)" --msgid-bugs-address="cc-devel@lists.ibiblio.org" -j -o liblicense.pot ../../../liblicense-gnome/trunk/src/*.py
