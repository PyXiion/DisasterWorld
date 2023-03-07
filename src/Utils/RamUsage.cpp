#include <Disaster/Utils/RamUsage.hpp>
#include <cstdio>

#ifdef __linux__
# include <sys/resource.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <unistd.h>
#endif

#ifdef __APPLE__
# include <mach/task.h>
# include <mach/mach_init.h>
#endif

#ifdef _WINDOWS
# include <windows.h>
#else
# include <sys/resource.h>
#endif

size_t px::disaster::utils::GetUsedMemory(bool resident) {
#if defined(__linux__)
# if defined(NO_RUSAGE)
  // Ugh, getrusage doesn't work well on Linux.  Try grabbing info
  // directly from the /proc pseudo-filesystem.  Reading from
  // /proc/self/statm gives info on your own process, as one line of
  // numbers that are: virtual mem program size, resident set size,
  // shared pages, text/code, data/stack, library, dirty pages.  The
  // mem sizes should all be multiplied by the page size.
  size_t size = 0;
  FILE *file = fopen("/proc/self/statm", "r");
  if (file) {
      unsigned long vm = 0;
      fscanf (file, "%lu", &vm);  // Just need the first num: vm size
      fclose (file);
    size = (size_t)vm * getpagesize();
  }
  return size;
# else
  struct rusage r_usage;
  int ret = getrusage(RUSAGE_SELF,&r_usage);
  // if (ret == 0) return 0;
  return r_usage.ru_maxrss * 1024;
# endif 

#elif defined(__APPLE__)
  // Inspired by:
  // http://miknight.blogspot.com/2005/11/resident-set-size-in-mac-os-x.html
  struct task_basic_info t_info;
  mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
  task_info(current_task(), TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count);
  size_t size = (resident ? t_info.resident_size : t_info.virtual_size);
  return size;

#elif defined(_WINDOWS)
  // According to MSDN...
  PROCESS_MEMORY_COUNTERS counters;
  if (GetProcessMemoryInfo (GetCurrentProcess(), &counters, sizeof (counters)))
      return counters.PagefileUsage;
  else return 0;

#else
  // No idea what platform this is
  return 0;   // Punt
#endif
}