include Makefile.arch

MAIN_S = ./Resources/Sources/Programs/main.$(SrcSuf)
MAIN_O = ./Resources/Objects/main.$(ObjSuf)
MAIN_A = Pixel_Inefficiency_Analyzer$(ExeSuf)
OBJS          += $(MAIN_O)
PROGRAMS      += $(MAIN_A)

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:                  $(PROGRAMS)

$(MAIN_A): $(MAIN_O)
	printf "Compiling done, linking...\n" && $(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
	$(MT_EXE)
	@echo "Succesful make..."
	@echo "...$@ is ready to use."

$(MAIN_O): $(MAIN_S)
	printf "\n\n\n" && $(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@

clean:
	@rm -f $(OBJS) core


distclean: clean
	@rm -f $(PROGRAMS) *Dict.* *.def *.exp \
	*.root *.ps *.so *.lib *.dll *.d *.log .def so_locations
	@rm -rf cxx_repository

.SUFFIXES: .$(SrcSuf)


#.$(SrcSuf).$(ObjSuf):
#	$(CXX) $(CXXFLAGS) -c $<

