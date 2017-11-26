# include "pamela.h"

const char *get_home(const char *user)
{
  struct passwd	*passwdEnt;
  char		*home;

  if ((passwdEnt = getpwnam(user)) != NULL)
    if((home = passwdEnt->pw_dir) != NULL)
      return (home);
  return (NULL);
}

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
  if (exec("umount -f %s/secure_data-rw", get_home(user)) != 0 ||
      exec("cryptsetup luksClose %s_container", user) != 0 ||
      exec("rm -rf %s/secure_data-rw", get_home(user)) != 0)
    return (-1);
  return (0);
}

int	container_create(const char *v_path, const char *user, const char *mdp)
{
  if (exec("dd if=/dev/urandom bs=1M count=%d of=%s", CONTAINER, v_path) != 0 ||
      crypt_container_format(user, mdp) != 0 ||
      crypt_container_open(user, mdp) != 0 ||
      exec("mkfs.ext4 /dev/mapper/%s_container", user) != 0 ||
      exec("mkdir %s/secure_data-rw", get_home(user)) != 0 ||
      exec("mount /dev/mapper/%s_container %s/secure_data-rw", user, get_home(user)) != 0 ||
      exec("chown %s %s/secure_data-rw", user, get_home(user)) != 0)
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
  if (sprintf(v_path, "%s/.crypted_container", get_home(user)) < 0)
    return (-1);
  else if (access(v_path, F_OK) == -1)
    return (container_create(v_path, user, mdp));
  else if (crypt_container_open(user, mdp) != 0)
    return (-1);
  if (exec("mkdir %s/secure_data-rw", get_home(user)) != 0 ||
      exec("mount /dev/mapper/%s_container %s/secure_data-rw", user, get_home(user)) != 0 ||
      exec("chown %s %s/secure_data-rw", user, get_home(user)) != 0)
    return (-1);
  return (0);
}
