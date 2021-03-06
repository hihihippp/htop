/* Do not edit this file. It was automatically generated. */

#ifndef HEADER_Process
#define HEADER_Process
/*
htop - Process.h
(C) 2004-2011 Hisham H. Muhammad
Released under the GNU GPL, see the COPYING file
in the source distribution for its full text.
*/

#ifdef HAVE_LIBHWLOC
#endif

// This works only with glibc 2.1+. On earlier versions
// the behavior is similar to have a hardcoded page size.
#ifndef PAGE_SIZE
#define PAGE_SIZE ( sysconf(_SC_PAGESIZE) )
#endif
#define PAGE_SIZE_KB ( PAGE_SIZE / ONE_K )

#include "Object.h"
#include "Affinity.h"
#include "IOPriority.h"
#include <sys/types.h>

#define PROCESS_FLAG_IO 1
#define PROCESS_FLAG_IOPRIO 2
#define PROCESS_FLAG_OPENVZ 4
#define PROCESS_FLAG_VSERVER 8
#define PROCESS_FLAG_CGROUP 16

#ifndef Process_isKernelThread
#define Process_isKernelThread(_process) (_process->pgrp == 0)
#endif

#ifndef Process_isUserlandThread
#define Process_isUserlandThread(_process) (_process->pid != _process->tgid)
#endif

#ifndef Process_isThread
#define Process_isThread(_process) (Process_isUserlandThread(_process) || Process_isKernelThread(_process))
#endif

typedef enum ProcessField_ {
   PID = 1, COMM, STATE, PPID, PGRP, SESSION, TTY_NR, TPGID, FLAGS, MINFLT, CMINFLT, MAJFLT, CMAJFLT, UTIME,
   STIME, CUTIME, CSTIME, PRIORITY, NICE, ITREALVALUE, STARTTIME, VSIZE, RSS, RLIM, STARTCODE, ENDCODE,
   STARTSTACK, KSTKESP, KSTKEIP, SIGNAL, BLOCKED, SSIGIGNORE, SIGCATCH, WCHAN, NSWAP, CNSWAP, EXIT_SIGNAL,
   PROCESSOR, M_SIZE, M_RESIDENT, M_SHARE, M_TRS, M_DRS, M_LRS, M_DT, ST_UID, PERCENT_CPU, PERCENT_MEM,
   USER, TIME, NLWP, TGID,
   #ifdef HAVE_OPENVZ
   CTID, VPID,
   #endif
   #ifdef HAVE_VSERVER
   VXID,
   #endif
   #ifdef HAVE_TASKSTATS
   RCHAR, WCHAR, SYSCR, SYSCW, RBYTES, WBYTES, CNCLWB, IO_READ_RATE, IO_WRITE_RATE, IO_RATE,
   #endif
   #ifdef HAVE_CGROUP
   CGROUP,
   #endif
   #ifdef HAVE_OOM
   OOM,
   #endif
   IO_PRIORITY,
   LAST_PROCESSFIELD
} ProcessField;

struct ProcessList_;

typedef struct Process_ {
   Object super;

   struct ProcessList_ *pl;

   pid_t pid;
   char* comm;
   int indent;
   char state;
   bool tag;
   bool showChildren;
   bool show;
   pid_t ppid;
   unsigned int pgrp;
   unsigned int session;
   unsigned int tty_nr;
   pid_t tgid;
   int tpgid;
   unsigned long int flags;

   uid_t st_uid;
   float percent_cpu;
   float percent_mem;
   char* user;

   unsigned long long int utime;
   unsigned long long int stime;
   unsigned long long int cutime;
   unsigned long long int cstime;
   long int priority;
   long int nice;
   long int nlwp;
   IOPriority ioPriority;
   char starttime_show[8];
   time_t starttime_ctime;

   #ifdef HAVE_TASKSTATS
   unsigned long long io_rchar;
   unsigned long long io_wchar;
   unsigned long long io_syscr;
   unsigned long long io_syscw;
   unsigned long long io_read_bytes;
   unsigned long long io_write_bytes;
   unsigned long long io_cancelled_write_bytes;
   double io_rate_read_bps;
   unsigned long long io_rate_read_time;
   double io_rate_write_bps;
   unsigned long long io_rate_write_time;   
   #endif

   int processor;
   long m_size;
   long m_resident;
   long m_share;
   long m_trs;
   long m_drs;
   long m_lrs;
   long m_dt;

   #ifdef HAVE_OPENVZ
   unsigned int ctid;
   unsigned int vpid;
   #endif
   #ifdef HAVE_VSERVER
   unsigned int vxid;
   #endif

   #ifdef HAVE_CGROUP
   char* cgroup;
   #endif
   #ifdef HAVE_OOM
   unsigned int oom;
   #endif

   int exit_signal;
   int basenameOffset;
   bool updated;

   #ifdef DEBUG
   unsigned long int minflt;
   unsigned long int cminflt;
   unsigned long int majflt;
   unsigned long int cmajflt;
   long int itrealvalue;
   unsigned long int vsize;
   long int rss;
   unsigned long int rlim;
   unsigned long int startcode;
   unsigned long int endcode;
   unsigned long int startstack;
   unsigned long int kstkesp;
   unsigned long int kstkeip;
   unsigned long int signal;
   unsigned long int blocked;
   unsigned long int sigignore;
   unsigned long int sigcatch;
   unsigned long int wchan;
   unsigned long int nswap;
   unsigned long int cnswap;
   #endif

} Process;


extern const char *Process_fieldNames[];

extern const int Process_fieldFlags[];

extern const char *Process_fieldTitles[];


void Process_getMaxPid();

#define ONE_K 1024L
#define ONE_M (ONE_K * ONE_K)
#define ONE_G (ONE_M * ONE_K)

#define ONE_DECIMAL_K 1000L
#define ONE_DECIMAL_M (ONE_DECIMAL_K * ONE_DECIMAL_K)
#define ONE_DECIMAL_G (ONE_DECIMAL_M * ONE_DECIMAL_K)

void Process_delete(Object* cast);

extern ObjectClass Process_class;

Process* Process_new(struct ProcessList_ *pl);

void Process_toggleTag(Process* this);

bool Process_setPriority(Process* this, int priority);

bool Process_changePriorityBy(Process* this, size_t delta);

IOPriority Process_updateIOPriority(Process* this);

bool Process_setIOPriority(Process* this, IOPriority ioprio);

/*
[1] Note that before kernel 2.6.26 a process that has not asked for
an io priority formally uses "none" as scheduling class, but the
io scheduler will treat such processes as if it were in the best
effort class. The priority within the best effort class will  be
dynamically  derived  from  the  cpu  nice level of the process:
extern io_priority;
*/
#define Process_effectiveIOPriority(p_) (IOPriority_class(p_->ioPriority) == IOPRIO_CLASS_NONE ? IOPriority_tuple(IOPRIO_CLASS_BE, (p_->nice + 20) / 5) : p_->ioPriority)

#ifdef HAVE_LIBHWLOC

Affinity* Process_getAffinity(Process* this);

bool Process_setAffinity(Process* this, Affinity* affinity);

#elif HAVE_NATIVE_AFFINITY

Affinity* Process_getAffinity(Process* this);

bool Process_setAffinity(Process* this, Affinity* affinity);

#endif

void Process_sendSignal(Process* this, size_t sgn);

int Process_pidCompare(const void* v1, const void* v2);

int Process_compare(const void* v1, const void* v2);

#endif
