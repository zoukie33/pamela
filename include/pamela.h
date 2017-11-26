
#ifndef PAMELA_H_
# define PAMELA_H_

# include <stdio.h>
# include <stdarg.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include <sys/stat.h>
# include <security/pam_appl.h>
# include <security/pam_modules.h>
# include <sys/types.h>
# include <sys/wait.h>

# define BUFFER     512
# define CONTAINER  100

// Fonctions de manipulations de container

int	exec(const char *, ...);
int	container_open(const char *, const char *);
int	container_close(const char *);
int	container_create(const char *, const char *, const char *);

// fonctions de chiffrement avec luks

int	crypt_container_format(const char *, const char *);
int	crypt_container_open(const char *, const char*);

#endif /* PAMELA_H_ */
