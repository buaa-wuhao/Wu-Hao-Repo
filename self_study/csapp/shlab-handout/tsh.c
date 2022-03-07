/* 
 * tsh - A tiny shell program with job control
 * 
 * <Put your name and login ID here>
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */
int  bg;                    /*if it is background command,1:background,0:foreground*/
int  global_job_id;         /*Store the job id which either terminated or stopped,causing
                             sig_child is received by the main loop*/

struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */
volatile sig_atomic_t child_pid;
volatile sig_atomic_t signal_received;
/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
int do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs); 
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid); 
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
	    break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
	    break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
	    break;
	default:
            usage();
	}
    }

    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler); 

    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1) {

	/* Read command line */
	if (emit_prompt) {
	    printf("%s", prompt);
	    fflush(stdout);
	}
	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	    app_error("fgets error");
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    exit(0);
	}

	/* Evaluate the command line */
	eval(cmdline);
    //printf("After cmdline\n");
	fflush(stdout);
  //  printf("After fflush(stdout)\n");
  	fflush(stdout);
   // printf("child pid: %d\n",child_pid);
    } 

    exit(0); /* control never reaches here */
}
  
/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
void eval(char *cmdline) 
{
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int  is_builtin_cmd;
    pid_t pid;
    sigset_t mask_all,mask_one,prev_one;

    sigfillset(&mask_all);
    sigemptyset(&mask_one);
    sigaddset(&mask_one,SIGCHLD);
    //sigaddset(&mask_one,SIGTSTP);
    //sigaddset(&mask_one,SIGINT);

    strcpy(buf,cmdline);
    bg = parseline(buf,argv);
    if(argv[0]==NULL)
        return;
    is_builtin_cmd = builtin_cmd(argv); 
    if(!is_builtin_cmd){
        sigprocmask(SIG_BLOCK,&mask_one,&prev_one);

        if((pid = fork()) == 0){
            sigprocmask(SIG_SETMASK, &prev_one,NULL);
            setpgid(0,0);
            if(execve(argv[0],argv,environ) < 0){
                printf("%s: Command not found.\n",argv[0]);
                exit(0);
            }
        }
        //printf("a new pid is generated:%d\n",pid);
        int job_state = (bg?BG:FG);
        //sigprocmask(SIG_BLOCK,&mask_all,NULL);
        addjob(&jobs,pid,job_state,cmdline);
        sigprocmask(SIG_SETMASK,&prev_one,NULL);
    }
    /*fore ground and not built in command*/
    if(!bg){
        //printf("is builtin cmd?: %d\n",is_builtin_cmd);
        // non-builtin command or "fg" command
        if(is_builtin_cmd==0 || is_builtin_cmd==2){
            sigprocmask(SIG_BLOCK,&mask_one,&prev_one);
            child_pid = 0;        
            while(!child_pid)
            {
                //printf("Loop forever!\n");
                sigsuspend(&prev_one);
            }
            sigprocmask(SIG_SETMASK,&prev_one,NULL);
            switch(signal_received)
            {
                case SIGINT:
                    //printf("Job [%d] (%d) terminated by signal 2\n",pid2jid(child_pid),child_pid);
                    printf("Job [%d] (%d) terminated by signal 2\n",global_job_id,child_pid);
                    break;
                case SIGTSTP:
                    //printf("Job [%d] (%d) stopped by signal 20\n",pid2jid(child_pid),child_pid);
                    printf("Job [%d] (%d) stopped by signal 20\n",global_job_id,child_pid);
                    break;
                default:
            //        printf("unknown signal received:[%d],its sending pid is: [%d]\n",signal_received,child_pid);
                    break;
            }
            signal_received = 0;
        }
    }
    else
    {
        //printf("bg\n");
        //if it is not command "bg" then we print out jobID,pid and command line
        //otherwise stay quite.Based on behavior of "tshref"
        if(is_builtin_cmd!=3)
            printf("[%d] (%d) %s",pid2jid(pid),pid,cmdline);
    }
    return;
}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
	buf++;
	delim = strchr(buf, '\'');
    }
    else {
	delim = strchr(buf, ' ');
    }

    while (delim) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;

	if (*buf == '\'') {
	    buf++;
	    delim = strchr(buf, '\'');
	}
	else {
	    delim = strchr(buf, ' ');
	}
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* ignore blank line */
	return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	argv[--argc] = NULL;
    }
    return bg;
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 * rtn 0: non-builtin command
 * rtn 1: non-bg/fg builtin command 
 * rtn 2: fg builtin command
 * rtn 3: bg builtin command
 * rtn 4: invalid fg/bg command such as "fg a"
 */
int builtin_cmd(char **argv) 
{
    if(!strcmp(argv[0],"quit"))
        exit(0);
    if(!strcmp(argv[0],"jobs")){
        listjobs(&jobs);
        return 1;
    }
    if(!strcmp(argv[0],"bg") || !strcmp(argv[0],"fg")){
        //invalid bg/fg command
        if(do_bgfg(argv)==0)
        {
            return 4;
        }
        //printf("bg command\n");
        //return 2;
    }
    if(!strcmp(argv[0],"fg")){
        return 2;
    }
    if(!strcmp(argv[0],"bg")) {
        return 3;
    }

    return 0;     /* not a builtin command */
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 * return 1:valid fg/bg command
 * return 0:invalid fg/bg command
 */
int do_bgfg(char **argv) 
{
    int job_id;
    if(argv[1]==NULL || (argv[1][0]!='%' && !(argv[1][0]>'0' && argv[1][0]<='9'))) {
        //to eascape % in printf function,use another %
        printf("%s command requires PID or %%jobid argument\n",argv[0]);
        return 0;
    }
    if(argv[1][0]!='%')
        job_id = pid2jid(atoi(argv[1]));
    else
        job_id = atoi(argv[1]+1);
    
    struct job_t* ptr_job = getjobjid(&jobs,job_id);
    if(ptr_job==NULL)
    {
        printf("%s : No such job or process\n",argv[1]);
        return 0;
    }

    if(!strcmp(argv[0],"fg"))
    {
       if(ptr_job->state==ST) 
       {
           //send SIGCONT signal to all process in that group.
           //incase ./mysplit 50 command, will create an grand child in same process group
           kill(-ptr_job->pid,SIGCONT);
       }
       ptr_job->state = FG;
       //waitfg(ptr_job->pid);
       bg = 0;
    }

    if(!strcmp(argv[0],"bg"))
    {
        ptr_job->state = BG;
        kill(-ptr_job->pid,SIGCONT);
        bg = 1;
        printf("[%d] (%d) %s",pid2jid(ptr_job->pid),ptr_job->pid,ptr_job->cmdline);
    }
    return 1;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{
   // child_pid = 0;        
   // while(!child_pid)
   //     sigsuspend(&prev_one);
    waitpid(pid,NULL,0);
    return;
}

/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) 
{
    int olderrno = errno,status;
    sigset_t mask_all, prev_all;
    signal_received = sig;
    struct job_t* ptr_job = NULL;
    sigfillset(&mask_all);
    //printf("reap the zombieeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee\n");
    //waitpid will return if child process either terminated or stopped(WUNTRACED flg)
    //if child process terminated, waitpid will reap it.
    child_pid = waitpid(-1,&status,WNOHANG | WUNTRACED);
    /*For trace16 file, when child process is dead because of interrupt signal(interrupt signal is sent 
      to child process,parent process(tsh process) will not reveice this interrupt signal)
      the child process will be removed from jobs_list here.In the same time,
      main loop still needs to display this child process's death is caused by 
      interrupt signal,needs to display the job id of this child process.But 
      job information already is removed from jobs_list.So use this global variable
      global_job_id to transfer this already deleted information to main loop, so that
      main loop still could display its job_id.
      
      If the interrupt signal is sent to our shell directly,this is not a problem, because
      when interrupt signal was sent to our shell, interrupt signal handler will be called first
      which will cause main loop comes out of sleep and display the going to be deleted process information
      then,sigchld_handler will be called after that.*/
    global_job_id = pid2jid(child_pid);
    if(WIFEXITED(status) || WIFSIGNALED(status))
    {
        sigprocmask(SIG_BLOCK,&mask_all,&prev_all);
        deletejob(&jobs,child_pid);
        sigprocmask(SIG_SETMASK,&prev_all,NULL);
    }
    if(WIFSIGNALED(status)){
        signal_received = WTERMSIG(status);
    }
    if(WIFSTOPPED(status)){
        ptr_job = getjobpid(&jobs,child_pid);
        ptr_job->state = ST;
        signal_received = WSTOPSIG(status);
    }
    //printf("sigchld received,the sending child_pid is: %d\n",child_pid);
    errno = olderrno;
    return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) 
{
    int olderrno = errno;
    sigset_t mask_all, prev_all;
    signal_received = sig;
    sigfillset(&mask_all);
    sigprocmask(SIG_BLOCK,&mask_all,&prev_all);
    //printf("interrupt handler**************************************!\n");
    child_pid = fgpid(&jobs);
    //global_job_id = pid2jid(child_pid);
    if(child_pid) 
    {
       kill(-child_pid,SIGINT); 
       //forece foreground PID to 0 to make main loop stay in the while loop of sigsuspend.
       //it will come out of while loop of sigsuspend when sigchld_handler is called when
       //child process state changes.
       child_pid = 0;
       //deletejob(&jobs,child_pid);
    }
    sigprocmask(SIG_SETMASK,&prev_all,NULL);
    errno = olderrno;
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) 
{
    int olderrno = errno;
    sigset_t mask_all, prev_all;
    //struct job_t* ptr_job;
    signal_received = sig;
    sigfillset(&mask_all);
    sigprocmask(SIG_BLOCK,&mask_all,&prev_all);
    child_pid = fgpid(&jobs);
    //printf("signal stop received,stop pid%d\n",child_pid);
    if(child_pid) 
    {
       kill(-child_pid,SIGTSTP); 
       //when Ctrl+z is pressed in tsh shell,tsh will receive two signals
       //1.SIGTSTP and 2.SIGCHLD.
       //sigtstp_handler will be called first, then since this handler send kill signal
       //to child process,so sigchld_handler will be called secondly when child process
       //either terminated or state changed to stop.
       //when sigstp_handler is called,force child_pid(foreground process group ID) to zero
       //So, in the main loop, it will stays in the while loop of sigsuspend();
       //Everything is handled only after sigchld_handler is called. because calling of
       //sigchld_handler means states of child process has changed. Main loop will come out of
       //while loop of sigsuspend(), when sicchld_handler is called.
       // sigchild_handler is the main and the only entry to handle child process death or stopped
       
       child_pid = 0;
       //ptr_job = getjobpid(&jobs,child_pid);
       //ptr_job->state = ST;
    }
    sigprocmask(SIG_SETMASK,&prev_all,NULL);
    errno = olderrno;
    return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs) 
{
    int i, max=0;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid > max)
	    max = jobs[i].jid;
    return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) 
{
    int i;
    
    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == 0) {
	    jobs[i].pid = pid;
	    jobs[i].state = state;
	    jobs[i].jid = nextjid++;
	    if (nextjid > MAXJOBS)
		nextjid = 1;
	    strcpy(jobs[i].cmdline, cmdline);
  	    if(verbose){
	        printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
            }
            return 1;
	}
    }
    printf("Tried to create too many jobs\n");
    return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid) 
{
    int i;

    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == pid) {
	    clearjob(&jobs[i]);
	    nextjid = maxjid(jobs)+1;
	    return 1;
	}
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].state == FG)
	    return jobs[i].pid;
    return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid)
	    return &jobs[i];
    return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid) 
{
    int i;

    if (jid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid == jid)
	    return &jobs[i];
    return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) 
{
    int i;

    if (pid < 1)
	return 0;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid) {
            return jobs[i].jid;
        }
    return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs) 
{
    int i;
    //printf("list jobs \n"); 
    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid != 0) {
	    printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
	    switch (jobs[i].state) {
		case BG: 
		    printf("Running ");
		    break;
		case FG: 
		    printf("Foreground ");
		    break;
		case ST: 
		    printf("Stopped ");
		    break;
	    default:
		    printf("listjobs: Internal error: job[%d].state=%d ", 
			   i, jobs[i].state);
	    }
	    printf("%s", jobs[i].cmdline);
	}
    }
}
/******************************
 * end job list helper routines
 ******************************/


/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) 
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}



