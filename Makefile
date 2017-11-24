CC		=	gcc

NAME		=	pamela.so

NAME_TEST	=	pamela_test.so

SRC		=	src/pamela.c

OBJ		=	$(SRC:.c=.o)

SRC_T		=	src/pamela_test.c

OBJ_T		=	$(SRC_T:.c=.o)

LDFLAGS		=	-fPIC -fno-stack-protector

CFLAGS		=	-I./include/ -W -Wall -Wextra

FLAGS_T		=	-lpam -lpam_misc -lcriterion

RM		=	rm -rf

$(NAME)		:	$(OBJ)
			$(CC) -o $(NAME) $(LDFLAGS) $(OBJ)

$(NAME_TEST)	:	$(OBJ_T)
			$(CC) -o $(NAME_TEST) $(OBJ_T) $(FLAGS_T)

clean		:
			$(RM) $(OBJ)

fclean		:	clean
			$(RM) $(OBJ)

re		:	fclean all

install		:
			@echo "Sharing Pamela so."
			sudo ld -x --shared -o /lib/security/pamela.so pamela.o
			@echo "Editing common-auth config."
			echo "auth sufficient pamela.so" > /etc/pam.d/common-auth
			@echo "Editing common-account config."
			echo "account sufficient pamela.so" > /etc/pam.d/common-account
uninstall	:
			@echo "Suppression de de pamela so."
			$(RM) /lib/security/pamela.so
			@echo "Suppression de la ligne auth."
			sed 'auth sufficient pamela.so' /etc/pam.d/common-auth
			@echo "Suppression de la ligne account."
			sed 'account sufficient pamela.so' /etc/pam.d/common-account

check		:

test		:
			@echo "Compiling pamela_test."
			$(NAME_TEST)

.PHONY		:	clean fclean re test install uninstall check
