PREFIX          ?= /opt/forticlient
BINDIR          ?= $(PREFIX)/bin
FORTICLIENT_CLI ?= $(PREFIX)/forticlientsslvpn_cli
DOMAIN          ?= abtlus.org.br

all: hack.so fixresolvconf forticlient

hack.so: hack.c
	$(CC) -Wall -FPIC -DPIC -shared -o $@ $^ -ldl

forticlient: forticlient.in
	sed \
		-e 's|@@BINDIR@@|$(BINDIR)|' \
		-e 's|@@PREFIX@@|$(PREFIX)|' \
		-e 's|@@FORTICLIENT_CLI@@|$(FORTICLIENT_CLI)|' \
		$^ > $@

fixresolvconf: fixresolvconf.in
	sed \
		-e 's|@@DOMAIN@@|$(DOMAIN)|' \
		$^ > $@

$(DESTDIR)$(BINDIR)/forticlient: forticlient
	install -D -m 755 $^ $@

$(DESTDIR)$(PREFIX)/%: %
	install -D -m 755 $^ $@

install_TARGETS = $(DESTDIR)$(BINDIR)/forticlient \
		  $(DESTDIR)$(PREFIX)/fixresolvconf \
		  $(DESTDIR)$(PREFIX)/fixroutes \
		  $(DESTDIR)$(PREFIX)/hack.so

install: $(install_TARGETS)
	true

clean:
	-rm -f forticlient fixresolvconf hack.so
