# include "pamela.h"

int   exec(const char *tmp, ...)
{
  va_list     ap;
  char        cmd[BUFFER];

  va_start(ap, tmp);
  if (vsprintf(cmd, tmp, ap) < 0)
    return (-1);
  if (system(cmd) < 0)
    return (-1);
  va_end(ap);
  return (0);
}

int	container_close(const char *user)
{
  if (exec("umount -f %s/secure_data-rw", get_user_home(user)) != 0 ||
      //exec("cryptsetup luksClose %s_container", user) != 0 ||
      exec("rm -rf %s/secure_data-rw", get_user_home(user)) != 0)
    return (-1);
  return (0);
}

int	container_create(const char *v_path, const char *user, const char *mdp)
{
  if (exec("dd if=/dev/urandom bs=1M count=%d of=%s", CONTAINER_SIZE, volume_path) != 0 ||
      //      crypt_container_format(user, pass) != 0 ||
      // crypt_container_open(user, pass) != 0 ||
      exec("mkfs.ext4 /dev/mapper/%s_container", user) != 0 ||
      exec("mkdir %s/secure_data-rw", get_user_home(user)) != 0 ||
      exec("mount /dev/mapper/%s_container %s/secure_data-rw", user, get_user_home(user)) != 0 ||
      exec("chown %s %s/secure_data-rw", user, get_user_home(user)) != 0)
    return (-1);
  return (0);
}

int	container_open(const char *user, const char *mdp)
{
  char  v_path[BUFFER];

  if (getuid())
    {
      printf("PAMELA | Vous devez être root pour effectuer cette action\n");
      return (-1);
    }
  if (sprintf(v_path, "%s/.crypted_container", get_user_home(user)) < 0)
    return (-1);
  else if (access(v_path, F_OK) == -1)
    return (container_create(v_path, user, mdp));
  else if (secure_volume_open(user, pass) != 0)
    return (-1);
  if (exec("mkdir %s/secure_data-rw", get_user_home(user)) != 0 ||
      exec("mount /dev/mapper/%s_container %s/secure_data-rw", user, get_user_home(user)) != 0 ||
      exec("chown %s %s/secure_data-rw", user, get_user_home(user)) != 0)
    return (-1);
  return (0);
}
