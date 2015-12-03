##
## Makefile for gomoku in /home/guiz/School/rendu/gomoku/
##
## Made by Guiz
## Login   <briard_g@epitech.eu>
##
## Started on  Thu Dec  3 14:40:36 2015 Guiz
## Last update Thu Dec  3 15:09:01 2015 Guiz
##

NAME		=	gomoku

SRC		=	src/main.cpp		\
			src/Board.cpp

OBJ		=	$(SRC:.cpp=.o)

CXXFLAGS	+=	-W -Wall -Wextra -I./include

CXX		=	g++

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ) *.~

fclean:	clean
	rm -f $(NAME)

re: fclean all
