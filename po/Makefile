include ../Makefile.default

PACKAGE=linux_logo
INSTALL_LOCALEPATH=$(PREFIX)/share/locale

CATALOGS = da.mo de.mo es.mo fr.mo nb.mo nl.mo it.mo pl.mo pt_BR.mo ru.mo sv.mo uk.mo zh_TW.mo

POTFILES= ../*.c ../*.h

all: $(PACKAGE).pot update $(CATALOGS)

$(PACKAGE).pot: $(POTFILES)
	$(XGETTEXT) --default-domain=$(PACKAGE) --add-comments --keyword=_ --keyword=S_ $(POTFILES); 
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
	$(INSTALL) -c -m 755 -d $(INSTALL_LOCALEPATH)/$$l; \
	$(INSTALL) -c -m 755 -d $(INSTALL_LOCALEPATH)/$$l/LC_MESSAGES; \
	$(INSTALL) -c -m 644 $$n $(INSTALL_LOCALEPATH)/$$l/LC_MESSAGES/$(PACKAGE).mo; \
	done

%.mo: %.po
	msgfmt -o $@ $< 

update: $(PACKAGE).pot
	for n in $(CATALOGS); do \
	l=`basename $$n .mo`; \
	l=$$l".po"; \
	mv -f $$l $$l".bac"; \
	msgmerge -o $$l $$l".bac" $(PACKAGE).pot; \
	done
