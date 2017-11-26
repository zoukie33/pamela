CC		=	gcc -g

MKDIR		=	sudo mkdir -p

CP		=	sudo cp

NAME		=	pamela.so

NAME_TEST	=	pamela_test

SRC		=	src/pam_ext.c	\
			src/pam_container.c	\
			src/pam_crypt.c

OBJ		=	$(SRC:.c=.o)

SRC_T		=	src/pamela_test.c

OBJ_T		=	$(SRC_T:.c=.o)

LDFLAGS		=	-lcryptsetup 

CFLAGS		=	-W -Wall -Wextra -fPIC -fno-stack-protector -I./include

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
				@printf "\033[0;31mPAMELA | Already installed.\n\033[0m"
else
				sudo apt-get install -y cryptsetup gcc libcryptsetup-dev libpam0g-dev
				make
				$(MKDIR) /lib/security
				$(CP) $(NAME) /lib/security/
				@echo "Sharing Pamela so."
				@echo "Editing common-auth config."
				echo 'auth optional pamela.so' | sudo tee --append /etc/pam.d/common-auth
				@echo "Editing common-account config."
				echo 'account optional pamela.so' | sudo tee --append /etc/pam.d/common-account
				@echo "Editing common-session config."
			        echo 'session optional pamela.so' | sudo tee --append /etc/pam.d/common-session
				@echo "Editing common-password config."
				echo 'password optional pamela.so' | sudo tee --append /etc/pam.d/common-password
				@printf "\033[0;32mPAMELA |  Successfully installed.\n\033[0m"
endif

uninstall	:

ifeq ("$(wildcard /lib/security/$(NAME))", "")
				@printf "\033[0;31mPAMELA | Not installed.\n\033[0m"
else
				@echo "PAMELA | Deleting pamela.so."
				make fclean
				sudo $(RM) /lib/security/$(NAME)
				sudo sed -i '/auth optional pamela.so/d' /etc/pam.d/common-auth
				sudo sed -i '/account optional pamela.so/d' /etc/pam.d/common-account
				sudo sed -i '/password optional pamela.so/d' /etc/pam.d/common-password
				sudo sed -i '/session optional pamela.so/d' /etc/pam.d/common-session
				@printf "\033[0;32mPAMELA | Successfully uninstalled.\n\033[0m"
endif

check		:
ifeq ("$(wildcard /lib/security/$(NAME))", "")
				@printf "\033[0;31mPAMELA | Not installed\n\033[0m"
else
				@printf "\033[0;32mPAMELA | Installed\n\033[0m"
endif

test		:
			@echo "Compiling pamela_test."
			$(NAME_TEST)

.PHONY		:	clean fclean re test install uninstall check
