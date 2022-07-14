include ../Makefile.default

PACKAGE=linux_logo
INSTALL_LOCALEPATH=$(PREFIX)/share/locale

PO_FILES = $(wildcard *.po)
LINGUAS = $(basename $(PO_FILES))
CATALOGS = $(addsuffix .mo,$(LINGUAS))

POTFILES= ../*.c ../*.h

all: $(PACKAGE).pot $(CATALOGS)

$(PACKAGE).pot: $(POTFILES)
	$(XGETTEXT) --default-domain=$(PACKAGE) --add-comments --keyword=_ --keyword=S_ $(POTFILES); 
	if cmp -s $(PACKAGE).po $(PACKAGE).pot; then \
	rm -f $(PACKAGE).po; \
	else \
	mv $(PACKAGE).po $(PACKAGE).pot; \
	fi 

clean:
	rm -f *mo *~ *.bac

install: $(CATALOGS)
	for l in $(LINGUAS); do \
	$(INSTALL) -c -m 755 -d $(INSTALL_LOCALEPATH)/$$l; \
	$(INSTALL) -c -m 755 -d $(INSTALL_LOCALEPATH)/$$l/LC_MESSAGES; \
	$(INSTALL) -c -m 644 $$l.mo $(INSTALL_LOCALEPATH)/$$l/LC_MESSAGES/$(PACKAGE).mo; \
	done

%.mo: %.po
	msgfmt -c -o $@ $<

update: $(PACKAGE).pot
	for l in $(LINGUAS); do \
	l=$$l".po"; \
	mv -f $$l $$l".bac"; \
	msgmerge -o $$l $$l".bac" $(PACKAGE).pot; \
	done
