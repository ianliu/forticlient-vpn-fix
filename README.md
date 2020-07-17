# Forticlient VPN Fix

Forticlient proprietary's VPN client messes up with the host's routing table
and DNS servers. This project aims to fix those issues.


## Requirements

You will need [OpenResolv](https://roy.marples.name/projects/openresolv/) and
[Dnsmasq](http://www.thekelleys.org.uk/dnsmasq/doc.html) to forward DNS
requests to the specified domain. A configuration setup is described
[here](https://wiki.archlinux.org/index.php/Dnsmasq#openresolv).

## Building & Installing

For the script to work properly, you need to set some Make variables:

 * `FORTICLIENT_CLI`: This variable should point to the forticlient binary, it
   must accept a `--server` and `--vpnuser` flags.
 * `DOMAIN`: Your VPN domain; only requests to this domain will be forwarded to
   the VPN's DNS servers.
 * `PREFIX`: Install prefix; defaults to `/opt/forticlient`.
 * `BINDIR`: Binary install dir; defaults to `$(PREFIX)/bin`.

To build the project, run `make DOMAIN=... FORTICLIENT_CLI=...` with your
choice of variables. After that, install with `sudo make install`.

## What the heck

The forticlient binary uses PPPd to create a connection with the remove server.
PPPd will be called with the `usepeerdns` flag, which will then call
`resolvconf` with the peer DNS. The problem here is that the peer doesn't
report the search domain and doesn't include the `-p` flag for private (see
`man resolvconf`). That's where the `fixresolvconf` script comes in.

Another issue is that the forticlient binary will overrite `/etc/resolv.conf`
with the peer DNS. To overcome this, we inject modified versions of the
`fopen`, `fputc`, and `fclose` functions with `LD_PRELOAD`. The modified
functions are defined in [hack.c](hack.c).

The last fix is the routing tables. The default routing table created will
route every connection to the VPN. The `fixroutes` script will route only
`10.0.0.0` connections to the VPN.
