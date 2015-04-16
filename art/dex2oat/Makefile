DPATH		:=	/home/merck/Projects/dtools/art/dex2oat/data/dalvik-cache
TART		:=	framework/core.art
IFILE		:=	./files/dhry.jar
OFILE		:=	./dhry_
OFILE_E	:=	.oat
EXEC		:=	./bin/dex2oatmd
DUMP		:=	./bin/oatdumpmd

all:
	@$(EXEC) --boot-image=$(TART) --dex-file=$(IFILE) --oat-file=$(OFILE)x86$(OFILE_E) --android-root=./ --instruction-set=x86
	@$(EXEC) --boot-image=$(TART) --dex-file=$(IFILE) --oat-file=$(OFILE)arm$(OFILE_E) --android-root=./ --instruction-set=arm
	@$(EXEC) --boot-image=$(TART) --dex-file=files/Hello.jar --oat-file=Hello.oat --android-root=./ --instruction-set=x86

help:
	$(EXEC)

dump:
	$(DUMP) --oat-file=$(OFILE)x86$(OFILE_E)

dumparm:
	$(DUMP) --oat-file=$(OFILE)arm$(OFILE_E)

dump2:
	$(DUMP) --oat-file=Hello.oat

clean:
	rm -f *.oat

