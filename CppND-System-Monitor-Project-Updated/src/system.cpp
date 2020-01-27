#include <unistd.h>
#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::sort;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();

  vector<int> pids = LinuxParser::Pids();

  for (auto pid : pids) {
    long starttime = LinuxParser::UpTime(pid);
    long total_time = LinuxParser::ActiveJiffies(pid);
    long sys_uptime = LinuxParser::UpTime();
    long seconds = sys_uptime - (starttime / LinuxParser::GetHertz());

    float cpu_util =
        (float)(total_time / LinuxParser::GetHertz()) / (float)seconds;
    string user = LinuxParser::User(pid);
    string ram = LinuxParser::Ram(pid);
    long uptime = starttime / LinuxParser::GetHertz();
    string command = LinuxParser::Command(pid);

    Process p(pid, cpu_util, user, ram, uptime, command);
    processes_.emplace_back(p);
  }
  sort(processes_.begin(), processes_.end());
  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }