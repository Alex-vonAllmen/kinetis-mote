CONTIKI_PROJECT = kinets-mote
all: $(CONTIKI_PROJECT)

#UIP_CONF_IPV6=1

TARGETDIRS += targets
APPDIRS += ${addprefix ../apps/, $(APPS)}

CONTIKI = contiki
include $(CONTIKI)/Makefile.include
