TARGET=GSENSORDEMO

CXXFLAGS=-O2 -Wall -Werror -D _LINUX
LINKS=-lSUSIDevice
DGFLAGS=-MMD -MP -MT $@ -MF $(dir $@)/$(*F).d

OBJS=common.o gsensor.o
OUTPUTDIR=output
OUTPUTOBJ:=$(addprefix $(OUTPUTDIR)/,$(OBJS))

$(OUTPUTDIR)/$(TARGET): $(OUTPUTOBJ)
ifeq (yocto,$(OS_VERSION))
	$(CXX) -o $@ $(OUTPUTOBJ) -Wl,-rpath,./ $(LIBPATH)/$(OUTPUTDIR)/libSUSIDevice.so -Wl,-rpath,./ $(SUSI_YOCTO_LIB)libSUSI-4.00.so
	$(STRIP) $@
else
	$(CXX) -o $@ $(OUTPUTOBJ) $(LINKS)
	strip $@
endif

$(OUTPUTDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CXX) -c -o $@ $< $(DGFLAGS) $(CXXFLAGS)

clean:
	rm -rf $(OUTPUTDIR)/

-include $(OUTPUTOBJ:.o=.d)
