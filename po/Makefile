INSTALL= /usr/bin/install -c
INSTALL_CMD= ${INSTALL} -m 644
INSTALLDIR=/usr/lib/locale
PACKAGE = linux_logo

CATALOGS = de.mo dk.mo es.mo fr.mo nb.mo nl.mo it.mo pl.mo pt_BR.mo ru.mo ua.mo zh_TW.mo

POTFILES= ../*.c ../*.h

all: $(PACKAGE).pot update $(CATALOGS)

$(PACKAGE).pot: $(POTFILES)
	xgettext --default-domain=$(PACKAGE) --add-comments --keyword=_ --keyword=S_ $(POTFILES)
	if cmp -s $(PACKAGE).po $(PACKAGE).pot; then \
	rm -f $(PACKAGE).po; \
	else \
	mv $(PACKAGE).po $(PACKAGE).pot; \
	fi

clean:
	rm -f *mo $(PACKAGE).pot *~ *.bac

install: $(CATALOGS)
	for n in $(CATALOGS); do \
	l=`basename $$n .mo`; \
	$(INSTALL) -m 755 -d $(INSTALLDIR)/$$l; \
	$(INSTALL) -m 755 -d $(INSTALLDIR)/$$l/LC_MESSAGES; \
	$(INSTALL) -m 644 $$n $(INSTALLDIR)/$$l/LC_MESSAGES/$(PACKAGE).mo; \
	done
%.mo: %.po
	msgfmt -o $@ $<

update: $(PACKAGE).pot
       for n in $(CATALOGS); do \
       l=basename $$n .mo; \
       l=$$l".po";\
       mv -f $$l $$l".bac";\
       msgmerge -o $$l $$l".bac" $(PACKAGE).pot; \
       done
