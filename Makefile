NAME 			=	gomoku

GRAPHICALGAME	=	yes

#########################################################
#			BASICS				#
#########################################################

CC		=	g++
RM		=	rm -f
RMDIR	=	rm -rf
ECHO	=	echo -ne
LINK	=	ln -fs
CAT		=	cat
GROUNDS ?= 	aplay -q
COLOR_OFF	=	"\033[0m"
COLOR_1		=	"\033[1;32m"
COLOR_2		=	"\033[1;33m"
COLOR_3		=	"\033[1;34m"
COLOR_4		=	"\033[1;35m"
COLOR_5		=	"\033[0;36m"
COLOR_6		=	"\033[1;37m"
COLOR_7		=	"\033[1;38m"
COLOR_8		=	"\033[1;39m"

LINKING		=	yes

#########################################################
#			FLAGS				#
#########################################################

CXXFLAGS		=		-W -Wall -Wextra -ansi -std=c++14		\
								-Wno-strict-overflow			\
								-I$(GAME_INC_DIR)				\
								-I$(GRAPHICS_INC_DIR)			\
								-I$(AI_INC_DIR)					\

MAKEFLAGS		+=	--warn-undefined-variables				\
								--warn-unused-variables		\
								--no-print-directory

LFLAGS			= 	-lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio -lpthread

INCLUDE_DIR		=		./include/
BINARY_DIR		=		./bin/
SRC_DIR			=		./src/
OBJ_DIR			=		./obj/


GAME_SRC_DIR	=	$(addprefix $(SRC_DIR), game/)
GAME_OBJ_DIR	=	$(addprefix $(OBJ_DIR), game/)
GAME_INC_DIR	=	$(addprefix $(INCLUDE_DIR), game/)
GAME_OBJS		=	$(addsuffix .o, $(basename $(subst $(GAME_SRC_DIR), $(GAME_OBJ_DIR), $(GAME_SRCS))))
GAME_SRCS		=	$(addprefix $(GAME_SRC_DIR), $(GAME_SRC))

AI_SRC_DIR	=	$(addprefix $(SRC_DIR), AI/)
AI_OBJ_DIR	=	$(addprefix $(OBJ_DIR), AI/)
AI_INC_DIR	=	$(addprefix $(INCLUDE_DIR), AI/)
AI_OBJS		=	$(addsuffix .o, $(basename $(subst $(AI_SRC_DIR), $(AI_OBJ_DIR), $(AI_SRCS))))
AI_SRCS		=	$(addprefix $(AI_SRC_DIR), $(AI_SRC))

GRAPHICS_SRC_DIR	=	$(addprefix $(SRC_DIR), graphic/)
GRAPHICS_OBJ_DIR	=	$(addprefix $(OBJ_DIR), graphic/)
GRAPHICS_INC_DIR	=	$(addprefix $(INCLUDE_DIR), graphic/)
GRAPHICS_SRCS		=	$(addprefix $(GRAPHICS_SRC_DIR), $(GRAPHICS_SRC))
GRAPHICS_OBJS		=	$(addsuffix .o, $(basename $(subst $(GRAPHICS_SRC_DIR), $(GRAPHICS_OBJ_DIR), $(GRAPHICS_SRCS))))

GAME_SRC		?=	main.cpp			\
								Board.cpp			\
								Player.cpp		\
								Error.cpp			\
								Referee.cpp		\
								Game.cpp

AI_SRC			?=	AI_MonteCarlo.cpp	\
					Heuristic.cpp			\

GRAPHICS_SRC	?=	GameEngine.cpp	\


#GRAPHICS_SRC	+=			initMenu.cpp	\

UNITTEST 		?=	false

PRO				=	$(addprefix $(BINARY_DIR), $(NAME))

OBJS			=		$(GAME_OBJS)								\
							$(GRAPHICS_OBJS)						\
							$(AI_OBJS)								\

SRCS			=		$(GAME_SRC)									\
							$(GRAPHICS_SRCS)						\
							$(AI_SRCS)								\



FIRST		:=	$(shell test -d $(BINARY_DIR)				|| mkdir $(BINARY_DIR))		\
					$(shell test -d $(OBJ_DIR)					|| mkdir $(OBJ_DIR))	\
					$(shell test -d $(GRAPHICS_OBJ_DIR)	|| mkdir $(GRAPHICS_OBJ_DIR))	\
					$(shell test -d $(GAME_OBJ_DIR)	|| mkdir $(GAME_OBJ_DIR))			\
					$(shell test -d $(AI_OBJ_DIR)	|| mkdir $(AI_OBJ_DIR))			\

CLEVER_MAKEFILE	:=	undefined
LOVE := $(COLOR_6)" - Best Regards,\n\n\t\t\tJamais.\n\n"

all			:	$(FIRST)
					@$(ECHO) $(COLOR_4) "\n\nCompiling in "$(COLOR_2)"[ "$(COLOR_6)"RELEASE "$(COLOR_2)"]"$(COLOR_4)" mode.\n\n"$(COLOR_OFF)
					@if ($(MAKE) $(NAME) CC=$(CC) CXXFLAGS='$(CXXFLAGS) -O3' MAKEFLAGS='$(MAKEFLAGS) -j4 -l4' UNITTEST=no CLEVER_MAKEFILE=release); then $(ECHO);\
			 		else if ($(ECHO) $(COLOR_3) && $(CAT)  "./.IWishIWasClever/.iwishicompiled"$(shell echo $${RANDOM} % 3 + 1 | bc)".ascii"); then $(ECHO) $(LOVE) && $(GROUNDS) ./.IWishIWasClever/.jibbouze.wav; fi; fi

debug		:	$(FIRST) fclean
					@$(ECHO) $(COLOR_4) "\n\nCompiling in "$(COLOR_2)"[ "$(COLOR_6)"DEBUG "$(COLOR_2)"]"$(COLOR_4)" mode.\n\n"$(COLOR_OFF)
					@if ($(MAKE) $(NAME) CC=$(CC) CXXFLAGS='$(CXXFLAGS) -g3 -D_DEBUG=1' MAKEFLAGS='$(MAKEFLAGS) -j4 -l4 ' UNITTEST=no CLEVER_MAKEFILE=debug); then $(ECHO);\
			 		else if ($(ECHO) $(COLOR_3) && $(CAT)  "./.IWishIWasClever/.iwishicompiled"$(shell echo $${RANDOM} % 3 + 1 | bc)".ascii"); then $(GROUNDS) ./.IWishIWasClever/.jibbouze.wav; fi; fi

test		:	$(FIRST) fclean
					@$(ECHO) $(COLOR_4) "\n\nCompiling in "$(COLOR_2)"[ "$(COLOR_6)"TESTING "$(COLOR_2)"]"$(COLOR_4)" mode.\n\n"$(COLOR_OFF)
					@if ($(MAKE) $(NAME) CC=$(CC) CXXFLAGS='$(CXXFLAGS) -g3 -D_DEBUG=1 -D_BENCHMARK=1' MAKEFLAGS='$(MAKEFLAGS) -j4 -l4 ' UNITTEST=yes CLEVER_MAKEFILE=benchmark); then $(ECHO);\
			 		else if ($(ECHO) $(COLOR_3) && $(CAT)  "./.IWishIWasClever/.iwishicompiled"$(shell echo $${RANDOM} % 3 + 1 | bc)".ascii"); then $(ECHO) $(LOVE); fi; fi

$(NAME)		:	$(PRO)

ifeq ($(LINKING), yes)
			@$(LINK) $(PRO) $(NAME)
endif

ifeq ($(UNITTEST), yes)
#GRAPHICS_SRC+=AnimationUnitTest.cpp
#GRAPHICS_SRC+=main.cpp
else
#GRAPHICS_SRC+=main.cpp
endif


$(OBJ_DIR)%.o	:	$(SRC_DIR)%.cpp
			@$(ECHO) $(COLOR_5)
			@if\
			$(CC) $(CXXFLAGS) -c $< -o $@ $(LFLAGS);\
			then\
			$(ECHO) $(COLOR_8)"["$(COLOR_1)" OK "$(COLOR_8)"] "$(COLOR_5) ": " $< $(COLOR_OFF)"\n";\
			else\
			$(ECHO) $(COLOR_8)"["$(COLOR_4)" FAIL "$(COLOR_8)"] "$(COLOR_5) ": " $< $(COLOR_OFF)"\n";\
			fi

$(PRO)			:	$(OBJS)
			@$(ECHO) $(COLOR_8)"\n" "[" $(COLOR_1) " LINKING ... "$(COLOR_8)"]"$(COLOR_4)" >>>>>>>>>>>>>>>>>> "$(COLOR_OFF)
			@$(CC) $(CXXFLAGS) $(OBJS) -o $(PRO) $(LFLAGS)
			@$(ECHO) $(COLOR_8)"["$(COLOR_1)" DONE "$(COLOR_8)"]"$(COLOR_OFF)"\n\n";
			@$(ECHO) $(COLOR_3)
ifeq ($(CLEVER_MAKEFILE), release)
			 		@if ($(CAT)  "./.IWishIWasClever/.iwishiwasclever"$(shell echo $${RANDOM} % 5 + 1 | bc)".ascii"); then $(ECHO) $(LOVE); fi;
else ifeq ($(CLEVER_MAKEFILE), debug)
			 		@if ($(CAT)  "./.IWishIWasClever/.iwishiwasclever"$(shell echo $${RANDOM} % 2 + 6 | bc)".ascii"); then $(ECHO) $(LOVE); fi;
else ifeq ($(CLEVER_MAKEFILE), benchmark)
			 		@if ($(CAT)  "./.IWishIWasClever/.iwishiwasclever"$(shell echo $${RANDOM} % 2 + 8 | bc)".ascii"); then $(ECHO) $(LOVE); fi
endif
			@$(ECHO) $(COLOR_OFF)

clean			:
			@$(ECHO)	"\n\n"
			@$(ECHO) $(COLOR_2)"[ "$(COLOR_6)"Deleting "$(COLOR_2)"] " $(COLOR_4)"Graphic source objects\n"$(COLOR_OFF)
			@$(RM) $(GRAPHICS_OBJS)
			@$(ECHO) $(COLOR_2)"[ "$(COLOR_6)"Deleting "$(COLOR_2)"] " $(COLOR_4)"Object directory\n"$(COLOR_OFF)
			@$(RMDIR) $(OBJ_DIR)

fclean			:	clean
			@$(ECHO) $(COLOR_2)"[ "$(COLOR_6)"Deleting "$(COLOR_2)"] " $(COLOR_4)"Binary\n"$(COLOR_OFF)
			@$(RM) $(NAME)
			@$(ECHO) $(COLOR_2)"[ "$(COLOR_6)"Deleting "$(COLOR_2)"] " $(COLOR_4)"Binary directory\n"$(COLOR_OFF)
			@$(RMDIR) $(BINARY_DIR)
			@$(RM) $(PRO)
			@$(ECHO) $(COLOR_2)"\n\n	[ "$(COLOR_6)"Done"$(COLOR_2)" ]\n"$(COLOR_OFF)

re				:	fclean all

.PHONY			:	all test debug clean fclean re
