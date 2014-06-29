CONTIKI_PROJECT = kl25z-mote
all: $(CONTIKI_PROJECT)

#UIP_CONF_IPV6=1

TARGETDIRS += targets

CONTIKI = contiki
include $(CONTIKI)/Makefile.include
