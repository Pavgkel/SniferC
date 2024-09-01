CC=gcc
user=sudo

sniffer-build: sniffer.c
	$(CC) sniffer.c -o sniffer
sniffer-run:
	$(user) ./sniffer
sniffer-clean:
	rm sniffer
sniffer-build-and-run:
	$(CC) sniffer.c -o sniffer
	$(user) ./sniffer
