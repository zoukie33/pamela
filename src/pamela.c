/*
** pamela.c for pamela in /home/martin/rendu/pamela/src
** 
** Made by Martin
** Login   <martin.rimbaud@epitech.eu>
** 
** Started on  Thu Nov 23 15:37:55 2017 Martin
** Last update Thu Nov 23 18:38:12 2017 Martin
*/

# include <sys/stat.h>
# include <security/pam_appl.h>
# include <security/pam_modules.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>


// Appellé quand un utilisateur s'authentifie
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  int script_response = 255;
  const char *user;
  char *password;
  char *service;
  int pam_err;

  pam_err = pam_get_user(pamh, &user, NULL);
  if (pam_err == PAM_SUCCESS) {
    pam_err = pam_get_item(pamh, PAM_SERVICE, (const void **)&service);
    if (pam_err == PAM_SUCCESS) {
      pam_err = pam_get_authtok(pamh, PAM_AUTHTOK, (const char **)&password, NULL);
      if (pam_err == PAM_SUCCESS) {
	if (mkdir("~/secure_data-rw", 770) == 0){
	  printf("hello\n");
	}
      }
    }
  }
  return (pam_err);
}

// Appellé quand un utilisateur ouvre une session
PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return PAM_SUCCESS;
}

// Appellé quand un utilisateur ferme sa session
PAM_EXTERN int  pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return PAM_SUCCESS;
}

// Appellé quand le mot de passe est changé
PAM_EXTERN int  pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return PAM_IGNORE;
}
