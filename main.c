#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
//--------------------------------------------------
//                 man proc 5
//--------------------------------------------------
struct proc_stat
{
   struct process
   {
       long pid;
       char name[255];
       char state;
       long ppid;
       long pgrp;
       long session;
       long tty_nr;
       long tpgid;
       unsigned long flags;
       unsigned long minflt;
       unsigned long cminflt;
       unsigned long majflt;
       unsigned long cmajflt;
       unsigned long utime;
       unsigned long stime;
       long cutime;
       long cstime;
       long priority;
       long nice;
       long num_threads;
       long itrealvalue;
       unsigned long long starttime;
       unsigned long vsize;
       long rss;
       unsigned long rsslim;
       unsigned long startcode;
       unsigned long endcode;
       unsigned long startstack;
       unsigned long kstkesp;
       unsigned long kstkeip;
       unsigned long signals;
       unsigned long blocked;
       unsigned long sigignore;
       unsigned long sigcatch;
       unsigned long wchan;
       unsigned long nswap;
       unsigned long cnswap;
       long exit_signal;
       long processor;
       unsigned long rt_priority;
       unsigned long polocy;
       unsigned long long delayacct_blkio_ticks;
       unsigned long long guest_time;
       unsigned long cguest_time;
       unsigned long start_data;
       unsigned long end_data;
       unsigned long start_brk;
       unsigned long arg_start;
       unsigned long arg_end;
       unsigned long env_start;
       unsigned long env_end;
       long exit_code;
       //------------------------
       //предрасчитанные штучки
       //------------------------
       float cpu_percent_usage;
   }process;

   struct system
   {
       long uptime;
       unsigned long  load_average[3];
       //----------------------------
       unsigned long  ram_total;
       unsigned long  ram_free;
       unsigned long  ram_shared;
       unsigned long  ram_buffer;
       //---------------------------
       unsigned long  swap_total;
       unsigned long  swap_free;
       //---------------------------
       unsigned short procs;
   }system;
   //--------------------------------------------------
   // Пид текущего процесса, дабы не грохнуть самого
   // себя если что или не зарулить или ещё чего кря
   //--------------------------------------------------
   long  self_pid;
};

//--------------------------------------------------
// CPU - CPU PERCENT USAGE
//--------------------------------------------------
float cpu(long uptime,unsigned long utime,unsigned long stime,unsigned long long starttime)
{
   const long  CLK_TCK = sysconf(_SC_CLK_TCK);
   const long  PERCENT = 100;
   return (((float)utime/CLK_TCK) + ((float)stime/CLK_TCK)) * PERCENT / (((float)uptime) - ((float)starttime/CLK_TCK));
}

void proc_stat_update(void(handler)(struct proc_stat))
{
  DIR * proc;
  struct dirent *dir;
  unsigned long  pid;
  if(proc = opendir("/proc/"))
  {
    while ((dir = readdir(proc)) != NULL)
    {
       if(dir->d_type == DT_DIR  && sscanf(dir->d_name,"%d",&pid) == 1)
       {
          char path[255] = {0};
          memset(path,0,sizeof(path));
          sprintf(path,"/proc/%s/stat",dir->d_name);
          FILE * file = fopen(path,"r");
          assert(file);
          struct proc_stat ps;
          char buff[2048] = {0};
          memset(buff,0,sizeof(buff));
          //--------------------------------------------------
          // First get pid
          //--------------------------------------------------
          fgets(buff,sizeof(buff),file);
          int values = sscanf(buff,"%ld ",&ps.process.pid);
          assert(values == 1);
          //--------------------------------------------------
          // Now get process FUCK why not just %s Ж(
          // extract name looks soooooooo stuuupiid
          //--------------------------------------------------
          char * start = strchr(buff,'(');
          start++; //skip "("
          char * end = strrchr(buff,')');
          *end = '\0';//finalize process name in "(" for strlen
          memcpy(ps.process.name,start,strlen(start)+1);
          //--------------------------------------------------
          // Last get all another values (50 values)
          // TODO: for get acess to all values need use open()
          // with flags man proc 5, not fopen(). But user need
          // it trace values? I dont know. proc/pid/blala
          // containt more more files with more more information
          // if open and parce all it slow, i think i stopped
          // in small and fast util variant
          // * base info cpu usage
          // * base info mem usage
          // * name,pid,parent pid
          // * add process statictics mechanism for custom
          //   OOM for example
          // * base common system info and stop implementation
          // if need bigdata lol info about process we have
          // more more slow system utilits and another kernel
          // pathes for it. My util just stupid, small and fun
          //--------------------------------------------------
          values = sscanf(end+2,// 2 becounse jump "\0" and " "
          "%c %ld %ld %ld %ld %ld %lu %lu %lu %lu %lu %lu "
          "%lu %ld %ld %ld %ld %ld %ld %llu %lu %ld %lu %lu %lu %lu "
          "%lu %lu %lu %lu %lu %lu %lu %lu %lu %ld %ld %lu %lu %llu "
          "%llu %lu %lu %lu %lu %lu %lu %lu %lu %ld",
          &ps.process.state,
          &ps.process.ppid,
          &ps.process.pgrp,
          &ps.process.session,
          &ps.process.tty_nr,
          &ps.process.tpgid,
          &ps.process.flags,
          &ps.process.minflt,
          &ps.process.cminflt,
          &ps.process.majflt,
          &ps.process.cmajflt,
          &ps.process.utime,
          &ps.process.stime,
          &ps.process.cutime,
          &ps.process.cstime,
          &ps.process.priority,
          &ps.process.nice,
          &ps.process.num_threads,
          &ps.process.itrealvalue,
          &ps.process.starttime,
          &ps.process.vsize,
          &ps.process.rss,
          &ps.process.rsslim,
          &ps.process.startcode,
          &ps.process.endcode,
          &ps.process.startstack,
          &ps.process.kstkesp,
          &ps.process.kstkeip,
          &ps.process.signals,
          &ps.process.blocked,
          &ps.process.sigignore,
          &ps.process.sigcatch,
          &ps.process.wchan,
          &ps.process.nswap,
          &ps.process.cnswap,
          &ps.process.exit_signal,
          &ps.process.processor,
          &ps.process.rt_priority,
          &ps.process.polocy,
          &ps.process.delayacct_blkio_ticks,
          &ps.process.guest_time,
          &ps.process.cguest_time,
          &ps.process.start_data,
          &ps.process.end_data,
          &ps.process.start_brk,
          &ps.process.arg_start,
          &ps.process.arg_end,
          &ps.process.env_start,
          &ps.process.env_end,
          &ps.process.exit_code);
          fclose(file);
          //--------------------------------------------------
          // What happend in another kernels? All kernels
          // have 52 stat options? Current 5.x+ yes but older?
          //--------------------------------------------------
          assert(values == 50);// 50 + 1 name + 1 pid

          ps.self_pid = getpid();

          struct sysinfo s_info;
          int err = sysinfo(&s_info);
          assert((err == 0));
          float uptime = s_info.uptime;

          ps.system.uptime = s_info.uptime;
          ps.system.load_average[0] = s_info.loads[0];
          ps.system.load_average[1] = s_info.loads[1];
          ps.system.load_average[2] = s_info.loads[2];
          ps.system.ram_total  = s_info.totalram;
          ps.system.ram_free   = s_info.freeram;
          ps.system.ram_shared = s_info.sharedram;
          ps.system.ram_buffer = s_info.bufferram;
          ps.system.swap_total = s_info.totalswap;
          ps.system.swap_free  = s_info.freeswap;
          ps.system.procs      = s_info.procs;
          ps.process.cpu_percent_usage = cpu(ps.system.uptime,
                                             ps.process.utime,
                                             ps.process.stime,
                                             ps.process.starttime);
          //--------------------------------------------------
          // TODO: Add calculate memory usage, need read one
          // more file from proc ohh nooo, why calc mem usage
          // from this values? Why mem usage info not full
          // in stat file? rss and vsize no give actual info
          // about memory usage in bytes it just abstractions
          // FUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU :D
          //--------------------------------------------------
          //ps.process.mem_percent_usage = mpu()

          handler(ps);

       }
    }
    closedir(proc);
  }

}

void handler(struct proc_stat ps)
{
   if( ps.process.cpu_percent_usage > 1)
   {
     printf("%-6ld %c %.1f %-2d %ld %lu %s\n",ps.process.pid,
                                    ps.process.state,
                                    ps.process.cpu_percent_usage,
                                    ps.process.priority,ps.process.rss,ps.process.vsize,
                                    ps.process.name);
   }
}


int main(void)
{

    proc_stat_update(handler);


  return(0);
}

