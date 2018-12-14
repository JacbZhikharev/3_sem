/* Create two directories backupdir and dir with two empty text file in dir*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void
diff (const char *file, const char *name)
{
  mkdir ("backupdir", ACCESSPERMS);
  char nam[80] = "backupdir/";
  strcat (nam, name);
  char arg[256] = "diff ";
  strcat (arg, file);
  strcat (arg, " ");
  strcat (arg, nam);
  int r;
  int fd =
    open (nam, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRGRP | S_IROTH);
  if (fd < 0)
    {
      printf ("can't open file\n");
      exit (-1);
    }
  strcat (arg, "\n");
  close (STDOUT_FILENO);
  if (dup (fd) != STDOUT_FILENO)
    {
      close (fd);
      exit (-1);
    }
  close (fd);
  r = system (arg);
  return;
}

void
SearchDirectory (const char *name)
{
  DIR *dir = opendir (name);
  if (dir)
    {
      char path[PATH_MAX], *EndPtr = path;
      struct stat info;
      struct dirent *e;
      strcpy (path, name);
      EndPtr += strlen (name);
      while ((e = readdir (dir)) != NULL)
	{
	  char add[200] = "/";
	  strcat (add, e->d_name);
	  strcpy (EndPtr, add);
	  if (!stat (path, &info))
	    {
	      if (S_ISDIR (info.st_mode))
		{
		  if (e->d_name[0] != '.')
		    {
		      SearchDirectory (path);
		    }
		}
	      else
		{
		  if (S_ISREG (info.st_mode))
		    {
		      diff (path, e->d_name);
		    }
		}
	    }
	}
    }
  return;
}

void
Daemon (const char *path)
{
  while (1)
    {
      SearchDirectory (path);
      sleep (1);
    }
}

int
main ()
{
  mkdir ("dir", ACCESSPERMS);
  int a = creat ("dir/test1", ACCESSPERMS);
  int b = creat ("dir/test2", ACCESSPERMS);
  char path[256] = "dir";
  pid_t pid;
  if ((pid = fork ()) < 0)
    {
      perror ("can't fork");
      exit (1);
    }
  else if (pid != 0)
    {
      printf ("Kill me %d", pid);
      exit (0);
    }
  setsid ();
  Daemon (path);
  return 0;
}
