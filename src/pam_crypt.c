#include "pamela.h"

int	crypt_container_format(const char *user, const char *mdp)
{
  struct crypt_params_luks1	p;
  struct crypt_device		*crypt;
  char				v_path[BUFFER];

  if ((sprintf(v_path, "%s/.crypted_container", get_home(user)) < 0) ||
      (crypt_init(&crypt, v_path) < 0))
    return (-1);
  p.hash = "sha1";
  p.data_alignment = 0;
  p.data_device = NULL;
  if ((crypt_format(crypt, CRYPT_LUKS1, "aes", "xts-plain64", NULL, NULL, 256 / 8, &p) < 0) ||
      (crypt_keyslot_add_by_volume_key(crypt, CRYPT_ANY_SLOT, NULL, 0, mdp, strlen(mdp)) < 0))
    return (-1);
  return (0);
}


int	crypt_container_open(const char *user, const char *mdp)
{
  struct crypt_device		*crypt;
  char				v_path[BUFFER];
  char				device_name[BUFFER];

  if ((sprintf(v_path, "%s/.crypted_container", get_home(user)) < 0) ||
      (sprintf(device_name, "%s_container", user) < 0) ||
      (crypt_init(&crypt, v_path) < 0) ||
      (crypt_load(crypt, CRYPT_LUKS1, NULL) < 0) ||
      (crypt_activate_by_passphrase(crypt, device_name, CRYPT_ANY_SLOT, mdp, strlen(mdp), 0) < 0))
    return (-1);
  crypt_free(crypt);
  return (0);
}
