#include <spawn.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ENVIRONMENT variables*/
extern char **environ;

/* ASLR disabling magic constant from Apple LLDB source code
   https://opensource.apple.com/source/lldb/lldb-76/tools/darwin-debug/darwin-debug.cpp
*/
#ifndef _POSIX_SPAWN_DISABLE_ASLR
#define _POSIX_SPAWN_DISABLE_ASLR 0x0100
#endif


int main(int argc, char* argv[]) {
    pid_t pid;
    int status;
    char **argv2;
    posix_spawnattr_t p_attr;
    char *suffix = ".orig";
    
    argv2 = malloc(argc * sizeof(char*));

    /* add suffix to executable */
    argv2[0] = malloc(strlen(argv[0] + strlen(suffix) + 1));
    
    sprintf(argv2[0], "%s%s", argv[0], suffix);

    /* copy all arguments */
    for(int i = 1; i < argc; i++) {
	argv2[i] = strdup(argv[i]);
    }

    /* set magic constant to disable ASLR */
    posix_spawnattr_init(&p_attr);    
    posix_spawnattr_setflags(&p_attr, _POSIX_SPAWN_DISABLE_ASLR);
    
    status = posix_spawnp(&pid, argv2[0], NULL, &p_attr, argv, environ);
    if(status == 0) {	
	/* wait for end */
	if (waitpid(pid, &status,  WUNTRACED) != -1) {

	    /* normal case, just exit */
	    if (WIFEXITED(status)) {
		/* return original exit code */
		return WEXITSTATUS(status);
	    }
	    /* abnormal cases */
	    else if (WIFSIGNALED(status)) {
		fprintf(stderr, "%s SIGNALED by signal %d\n", argv2[0], WTERMSIG(status));
		return -1;
	    }
	    else if (WIFSTOPPED(status)) {
		fprintf(stderr, "%s STOPPED by signal %d\n", argv2[0], WSTOPSIG(status));
		return -1;
	    }
	    else {
		fprintf(stderr, "%s waitpid unknown status %d\n", argv2[0], status);
		return -1;
	    }
	}
	else {
	    perror("waitpid");
	    return -1;
	}
    }
    else {
	fprintf(stderr, "posix_spawn: %s\n", strerror(status));
	return -1;
    }
    
    return -1;
}