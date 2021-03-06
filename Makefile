#coding: utf-8
app = borne.bin

srcExt = cpp
srcDir = src
objDir = obj
binDir = bin

debug = 1

RPIadress = 172.19.34.64
#RPIadress = 192.168.1.10
RPIuser = pi
RPIdir = /home/borne/borneGui
CXX = C:/Libraries/Cygwin/opt/cross/x-tools/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++

CFlags = -DGLES2 -DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wall -Wno-psabi
LDFlags = -rdynamic -Wl,-rpath=extern/lib
libs = GLESv2 EGL openmaxil bcm_host ilclient png freetype z pthread rt dl curl
libDir = extern/lib
inc = extern/include extern/include/interface/vcos/pthreads src/OpenUtility


#************************ DO NOT EDIT BELOW THIS LINE! ************************

ifeq ($(debug),1)
	debug=-g
else
	debug=
endif
inc := $(addprefix -I,$(inc))
libs := $(addprefix -l,$(libs))
libDir := $(addprefix -L,$(libDir))
CFlags += -c $(debug) $(inc)
LDFlags += $(libDir) $(libs)
sources := $(shell find $(srcDir) -name '*.$(srcExt)')
srcDirs := $(shell find . -name '*.$(srcExt)' -exec dirname {} \; | sort | uniq)
objects := $(patsubst %.$(srcExt),$(objDir)/%.o,$(sources))

ifeq ($(srcExt),cpp)
	CC = $(CXX)
else
	CFlags += -std=gnu99
endif

.phony: all clean distclean


all: $(binDir)/$(app)

$(binDir)/$(app): $(objects)
	@mkdir -p `dirname $@`
	@echo "Linking $@..."
	@$(CC) $(objects) $(LDFlags) -o $@

$(objDir)/%.o: %.$(srcExt)
	@echo "Generating dependencies for $<..."
	@mkdir -p `dirname $(objDir)/$<`
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "Compiling $<..."
	@$(CC) $(CFlags) $< -o $@

clean:
	@echo "Deleting object and dependencies files"
	@$(RM) -r $(objDir)

distclean: clean
	@echo "Deleting binary file $(binDir)/$(app)"
	@$(RM) -r $(binDir)/$(app)

buildrepo:
	@$(call make-repo)

copysource:
	@echo "Synching sources with Raspberry Pi..."
	@rsync --chmod=Du=rwx,Dgo=rx,Fu=rw,Fgo=r -e ssh -avz --delete-after ./$(srcDir) $(RPIuser)@$(RPIadress):$(RPIdir)

install:
	@$(call sync)

run:
	@$(call sync)
	ssh -t $(RPIuser)@$(RPIadress) $(RPIdir)/bin/borne.bin -sd

rundebug:	
    C:/Libraries/Cygwin/opt/cross/x-tools/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gdb

sshkey: ~/.ssh/id_dsa.pub
	ssh-copy-id -i ~/.ssh/id_dsa.pub $(RPIuser)@$(RPIadress)

~/.ssh/id_dsa.pub:
	ssh-keygen -t dsa -f ~/.ssh/id_dsa -N ''

define make-repo
   for dir in $(srcDirs); \
   do \
	mkdir -p $(objDir)/$$dir; \
   done
endef

ifneq "$(MAKECMDGOALS)" "clean"
-include $(objects:.o=.d)
endif

# usage: $(call sync)
define sync
	@echo "Synching files with Raspberry Pi..."
	@rsync --chmod=Du=rwx,Dgo=rx,Fu=rw,Fgo=r -e ssh -avz --delete-after ./$(binDir) ./content ./shader $(RPIuser)@$(RPIadress):$(RPIdir)
	@ssh $(RPIuser)@$(RPIadress) 'chmod 755 $(RPIdir)/$(binDir)/$(app)'
endef

# usage: $(call make-depend,source-file,object-file,depend-file)
define make-depend
  $(CC) -MM       \
        -MF $3    \
        -MP       \
        -MT $2    \
        $(CFlags) \
        $1
endef