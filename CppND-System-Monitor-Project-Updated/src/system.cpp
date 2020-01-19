#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_.clear();

    vector<int> pids = LinuxParser::Pids();
    
    for (auto pid : pids){
        float cpu_util = 0.5;
        string user = LinuxParser::User(pid);
        string ram = LinuxParser::Ram(pid);
        long uptime = LinuxParser::UpTime(pid);
        string command = LinuxParser::Command(pid);
        Process p(pid, cpu_util, user, ram, uptime, command);
        processes_.emplace_back(p);
    }

    return processes_; 
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }