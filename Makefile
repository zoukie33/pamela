CC		=	gcc

MKDIR		=	sudo mkdir -p

CP		=	sudo cp

ECHO		=	sudo echo

NAME		=	pamela.so

NAME_TEST	=	pamela_test

SRC		=	src/pamela.c

OBJ		=	$(SRC:.c)

SRC_T		=	src/pamela_test.c

OBJ_T		=	$(SRC_T:.c=.o)

LDFLAGS		=	-lpam -lpam_misc

CFLAGS		=	-W -Wall -Wextra -fPIC -fno-stack-protector

FLAGS_T		=	-lpam -lpam_misc -lcriterion

RM		=	rm -rf

all		:	$(NAME)

$(NAME)		:	$(OBJ)
			$(CC) $(OBJ) -shared -rdynamic $(LDFLAGS) -o $(NAME)

$(NAME_TEST)	:	$(OBJ_T)
			$(CC) $(OBJ_T) $(FLAGS_T) -o $(NAME_TEST)

clean		:
			$(RM) $(OBJ)
			$(RM) $(OBJ_T)

fclean		:	clean
			$(RM) $(NAME)
			$(RM) $(NAME_TEST)

re		:	fclean all

install		:
ifneq ("$(wildcard /lib/security/$(NAME))", "")
				@printf "\033[0;31mPAM module already installed\n\033[0m"
else
				sudo apt-get install -y cryptsetup gcc libcryptsetup-dev libpam0g-dev
				make
				$(MKDIR) /lib/security
				$(CP) $(NAME) /lib/security/
				@echo "Sharing Pamela so."
				@echo "Editing common-auth config."
				echo 'auth sufficient pamela.so' | sudo tee --append /etc/pam.d/common-auth
				@echo "Editing common-account config."
				echo 'account sufficient pamela.so' | sudo tee --append /etc/pam.d/common-account
				@printf "\033[0;32mPAM module installed successfully\n\033[0m"
endif

uninstall	:

ifeq ("$(wildcard /lib/security/)", "")
				@printf "\033[0;31mPAM module not installed\n\033[0m"
else
				@echo "Suppression de de pamela so."
				make clean
				$(RM) /lib/security/$(NAME)
				sudo sed -i '/auth sufficient pamela.so /d' /etc/pam.d/common-account
				sudo sed -i '/account sufficient pamela.so /d' /etc/pam.d/common-auth
				@printf "\033[0;32mPAM module uninstalled successfully\n\033[0m"
endif

check		:
ifeq ("$(wildcard /lib/security)", "")
				@printf "\033[0;31mPAM module not installed\n\033[0m"
else
				@printf "\033[0;32mPAM module installed\n\033[0m"
endif

test		:
			@echo "Compiling pamela_test."
			$(NAME_TEST)

.PHONY		:	clean fclean re test install uninstall check
