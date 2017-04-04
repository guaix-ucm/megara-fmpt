
extraconf = Manuals Models Samples Measures

EXTRA_DIST = $(extraconf)

modelsdir = $(datadir)/megara-fmpt

install-data-local:
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`;
	$(MKDIR_P) $(DESTDIR)$(modelsdir) ;\
	for base in $(extraconf); do \
          dd=`find $(srcdir)/$$base -type d`; \
	  for d in $$dd; do \
	    stripdd=`echo "$$d" | sed "s|^$(srcdir)/||"`; \
	    $(MKDIR_P) $(DESTDIR)$(modelsdir)/$$stripdd; \
            done; \
	  dd=`find $(srcdir)/$$base -type f`; \
	  for d in $$dd; do \
	     stripdd=`echo "$$d" | sed "s|^$(srcdir)/||"`; \
	     $(INSTALL_DATA) $$d $(DESTDIR)$(modelsdir)/$$stripdd; \
	  done; \
	done;

uninstall-local:
	rm -rf $(DESTDIR)$(modelsdir) || :
