/*
** pamela.c for pamela in /home/martin/rendu/pamela/src
** 
** Made by Martin
** Login   <martin.rimbaud@epitech.eu>
** 
** Started on  Thu Nov 23 15:37:55 2017 Martin
** Last update Thu Nov 23 18:38:12 2017 Martin
*/

#include "pamela.h"

// Appellé quand un utilisateur s'authentifie
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  int script_response = 255;
  const char *user;
  char *password;
  char *service;
  int pam_err;
  FILE *fp;

  pam_err = pam_get_user(pamh, &user, NULL);
  if (pam_err == PAM_SUCCESS) {
    pam_err = pam_get_item(pamh, PAM_SERVICE, (const void **)&service);
    if (pam_err == PAM_SUCCESS) {
      pam_err = pam_get_authtok(pamh, PAM_AUTHTOK, (const char **)&password, NULL);
      if (pam_err == PAM_SUCCESS) {
	fp = popen(auth_cmd, "w");
	if (!fp) {
	  pam_err = PAM_SYSTEM_ERR;
	} else {
	  /* send authentication data to script */
	  fprintf(fp, "service=%s%c", service, 0);
	  fprintf(fp, "user=%s%c", user, 0);
	  fprintf(fp, "password=%s%c", password, 0);
	  /* extra NUL to mark end of data */
	  fprintf(fp, "%c", 0);

	  /* use exit status to authenticate */
	  script_response = pclose(fp);
	  if (script_response) {
	    if (WEXITSTATUS(script_response) == 2) {
	      // signal user unknown, so PAM may consider other options
	      pam_err = PAM_USER_UNKNOWN;
	    } else {
	      pam_err = PAM_AUTH_ERR;
	    }
	  }
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
