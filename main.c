#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>

void procmsg(const char* format, ...) {
    va_list ap;
    fprintf(stdout, "[%d] ", getpid());
    va_start(ap, format);
    vfprintf(stdout, format, ap);
    va_end(ap);
}

void run_target_process(const char* progname) {
    procmsg("Starting `%s`\n", progname);

    if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
        perror("[!] ptrace() failed.\n");
        return;
    }

    execl(progname, progname, 0);
}

void run_debugger(pid_t child_pid) {
    int wait_status;
    unsigned i_counter = 0;
    procmsg("Starting debugger for process %d...\n", child_pid);

    struct user_regs_struct regs;
    unsigned instruction;
    wait(&wait_status);
    while (WIFSTOPPED(wait_status)) {
        ++i_counter;
        ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
        instruction = ptrace(PTRACE_PEEKTEXT, child_pid, regs.rip, 0);

        procmsg("[%d][0x%08x] %s\n", i_counter, regs.rip, instruction);

        if (ptrace(PTRACE_SINGLESTEP, child_pid, 0, 0) < 0) {
            perror("[!] ptrace() failed.\n");
            return;
        }

        wait(&wait_status);
    }

    procmsg("Done debugging. Executed %d instructions.\n", i_counter);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "[!] Expected a program name as argument.\n");
        return -1;
    }

    pid_t child_pid = fork();

    if (child_pid == 0) {
        run_target_process(argv[1]);
    }
    else if (child_pid > 0) {
        run_debugger(child_pid);
    }
    else {
        perror("[!] fork() failed.\n");
        return -1;
    }

    return 0;
}

