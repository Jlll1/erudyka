erudyka: erudyka.c
	@${CC} -o erudyka erudyka.c

test: erudyka
	./test.sh

install: erudyka
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f erudyka ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/erudyka
	chmod 755 erudyka