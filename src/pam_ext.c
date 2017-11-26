/*
** pamela.c for pamela in /home/martin/rendu/pamela/src
** 
** Made by Martin
** Login   <martin.rimbaud@epitech.eu>
** 
** Started on  Thu Nov 23 15:37:55 2017 Martin
** Last update Thu Nov 23 18:38:12 2017 Martin
*/

# include "pamela.h"

void  pam_me_cleanup(pam_handle_t *pamh,
		     void *data,
		     int error_status)
{
  if (data) {
    memset(data, 0, strlen(data));
    free(data);
  }
}

// Appellé quand un utilisateur s'authentifie
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  const char	*user;
  const char	*mdp;
  int		pam_err;

  pam_err = pam_get_user(pamh, &user, "Username: ");
  if (pam_err == PAM_SUCCESS) {
    pam_err = pam_get_item(pamh, PAM_AUTHTOK, (const void **)&mdp); 
    if (pam_err == PAM_SUCCESS) {
      pam_err = pam_set_data(pamh, "pam_user_pass", strdup(mdp), &pam_me_cleanup);
      if (pam_err == PAM_SUCCESS){
	printf("PAMELA | Obtenez votre clé d'authentification.\n");
      }
    }
  }
  return (pam_err);
}

// Appellé quand un utilisateur ouvre une session
PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  const char	*user;
  const char	*mdp;

  int		pam_err;

  printf("PAMELA | Ouverture/déchiffrement de votre container.\n");
  pam_err = pam_get_user(pamh, &user, "Username: ");
  if (pam_err == PAM_SUCCESS) {
    pam_err = pam_get_data(pamh, "pam_user_pass", (const void **)&mdp);
    if (pam_err == PAM_SUCCESS){
      pam_err = container_open(user, mdp);
      if (pam_err != 0)
	printf("PAMELA | Vous retrouverez votre container dans votre /home.\n");
    }
  }
  return (pam_err);
}

// Appellé quand un utilisateur ferme sa session
PAM_EXTERN int  pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  const char *user;
  const char *mdp;

  int pam_err;

  printf("PAMELA | Chiffrement de votre container.\n");
  pam_err = pam_get_user(pamh, &user, "Username: ");
  if (pam_err == PAM_SUCCESS) {
    pam_err = container_close(user);
    if (pam_err != 0) {
      printf("PAMELA | Chiffrement terminé.\n");
    }
  }
  return (pam_err);
}

// Appellé quand le mot de passe est changé
PAM_EXTERN int  pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return PAM_IGNORE;
}
