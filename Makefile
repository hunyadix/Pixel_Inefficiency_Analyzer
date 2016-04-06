include Makefile.arch

MAIN_S = ./Resources/Sources/Programs/main.$(SrcSuf)
MAIN_O = ./Resources/Objects/main.$(ObjSuf)
MAIN_A = Pixel_Inefficiency_Analyzer$(ExeSuf)
OBJS          += $(MAIN_O)
PROGRAMS      += $(MAIN_A)

CONSOLE_COL_S = ./Resources/Sources/Includes/Console_colors/Console_colors.$(SrcSuf)
CONSOLE_COL_O = ./Resources/Objects/Console_colors.$(ObjSuf)
OBJS          += $(CONSOLE_COL_O)

CONSOLE_ACTOR_S = ./Resources/Sources/Includes/Console_actor/Console_actor.$(SrcSuf)
CONSOLE_ACTOR_O = ./Resources/Objects/Console_actor.$(ObjSuf)
OBJS          += $(CONSOLE_ACTOR_O)

COMMON_ACTORS_S = ./Resources/Sources/Includes/Console_actor/common_actors.$(SrcSuf)
COMMON_ACTORS_O = ./Resources/Objects/common_actors.$(ObjSuf)
OBJS          += $(COMMON_ACTORS_O)

TIMER_S = ./Resources/Sources/Includes/Timer/Timer.$(SrcSuf)
TIMER_O = ./Resources/Objects/Timer.$(ObjSuf)
OBJS          += $(TIMER_O)

TIMER_COL_S = ./Resources/Sources/Includes/Timer/Timer_colored.$(SrcSuf)
TIMER_COL_O = ./Resources/Objects/Timer_colored.$(ObjSuf)
OBJS          += $(TIMER_COL_O)

ROOT_COLOR_SETS_S = ./Resources/Sources/Includes/Histogram_generation/Root_color_sets/root_color_sets.$(SrcSuf)
ROOT_COLOR_SETS_O = ./Resources/Objects/root_color_sets.$(ObjSuf)
OBJS          += $(ROOT_COLOR_SETS_O)

INPUT_OUTPUT_INTERFACE_S = ./Resources/Sources/Includes/Input_output_interface/Input_output_interface.$(SrcSuf)
INPUT_OUTPUT_INTERFACE_O = ./Resources/Objects/Input_output_interface.$(ObjSuf)
OBJS          += $(INPUT_OUTPUT_INTERFACE_O)

TREE_INITIALIZER_S = ./Resources/Sources/Includes/Ntuple_reader/Tree_initializer.$(SrcSuf)
TREE_INITIALIZER_O = ./Resources/Objects/Tree_initializer.$(ObjSuf)
OBJS          += $(TREE_INITIALIZER_O)

NTUPLE_READER_S = ./Resources/Sources/Includes/Ntuple_reader/Ntuple_reader.$(SrcSuf)
NTUPLE_READER_O = ./Resources/Objects/Ntuple_reader.$(ObjSuf)
OBJS          += $(NTUPLE_READER_O)

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:                  $(PROGRAMS)

$(MAIN_A): $(MAIN_O) $(FITTER_O) $(CONSOLE_COL_O) $(CONSOLE_ACTOR_O) $(COMMON_ACTORS_O) $(TIMER_O) $(TIMER_COL_O) $(ROOT_COLOR_SETS_O) $(INPUT_OUTPUT_INTERFACE_O) $(TREE_INITIALIZER_O) $(NTUPLE_READER_O) 
	@printf "Compiling done, linking...\n"
	@$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
	$(MT_EXE)
	@echo "Succesful make..."
	@echo "...$@ is ready to use."

$(MAIN_O): $(MAIN_S)
	@printf "Compiling main...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Main compiled.\n"

$(CONSOLE_COL_O): $(CONSOLE_COL_S)
	@printf "Compiling console colors...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Console colors compiled.\n"

$(CONSOLE_ACTOR_O): $(CONSOLE_ACTOR_S)
	@printf "Compiling console actor...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Console actor compiled.\n"

$(COMMON_ACTORS_O): $(COMMON_ACTORS_S)
	@printf "Compiling common actors...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Common actors compiled.\n"

$(TIMER_O): $(TIMER_S)
	@printf "Compiling timer...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Timer compiled.\n"

$(TIMER_COL_O): $(TIMER_COL_S)
	@printf "Compiling colored timer...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Colored timer compiled.\n"

$(ROOT_COLOR_SETS_O): $(ROOT_COLOR_SETS_S)
	@printf "Compiling root color sets...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Root color sets compiled.\n"

$(INPUT_OUTPUT_INTERFACE_O): $(INPUT_OUTPUT_INTERFACE_S)
	@printf "Compiling i/o interface...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Input-output interface compiled.\n"

$(TREE_INITIALIZER_O): $(TREE_INITIALIZER_S)
	@printf "Compiling tree initializer...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Tree initializer compiled.\n"

$(NTUPLE_READER_O): $(NTUPLE_READER_S)
	@printf "Compiling ntuple reader...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Ntuple reader compiled.\n"

clean:
	@rm -f $(OBJS) core


distclean: clean
	@rm -f $(PROGRAMS) *Dict.* *.def *.exp \
	*.root *.ps *.so *.lib *.dll *.d *.log .def so_locations
	@rm -rf cxx_repository

.SUFFIXES: .$(SrcSuf)


#.$(SrcSuf).$(ObjSuf):
#	$(CXX) $(CXXFLAGS) -c $<

