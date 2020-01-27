#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  float mem_total, mem_free;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      if (linestream >> key >> value) {
        if (key == "MemTotal") mem_total = stof(value);
        if (key == "MemFree") mem_free = stof(value);
      }
    }
  }
  return (mem_total - mem_free) / mem_total;
}

long LinuxParser::UpTime() {
  string line;
  string uptime, spenttime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> spenttime;
    return stol(uptime);
  }
  return 0;
}

long LinuxParser::Jiffies() {
  vector<string> jiffies = CpuUtilization();
  long total_jiffies_since_boot = 0;
  for (string s : jiffies) {
    total_jiffies_since_boot += atol(s.c_str());
  }
  return total_jiffies_since_boot;
}

long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string value;
  long utime, stime, cutime, cstime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  std::getline(filestream, line);
  std::istringstream linestream(line);
  for (int i = 1; i < 18; ++i) {
    linestream >> value;
    if (i == 14) utime = stol(value);
    if (i == 15) stime = stol(value);
    if (i == 16) cutime = stol(value);
    if (i == 17) cstime = stol(value);
  }
  return utime + stime + cutime + cstime;
}

long LinuxParser::ActiveJiffies() {
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

long LinuxParser::IdleJiffies() {
  vector<string> jiffies = CpuUtilization();
  // idle + iowait
  return atol(jiffies[kIdle_].c_str()) + atol(jiffies[kIOwait_].c_str());
}

vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string user, nice, system, idle, iowit, irq, softirq, steal, guest, guestnice;
  vector<string> utils;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> key) {
      if (key == "cpu") {
        linestream >> user >> nice >> system >> idle >> iowit >> irq >>
            softirq >> steal >> guest >> guestnice;
        utils.emplace_back(user);
        utils.emplace_back(nice);
        utils.emplace_back(system);
        utils.emplace_back(idle);
        utils.emplace_back(iowit);
        utils.emplace_back(irq);
        utils.emplace_back(softirq);
        utils.emplace_back(steal);
        return utils;
      }
    }
  }
  return {};
}

int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return -1;
}

int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return -1;
}

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) return line;
  }
  return string();
}

string LinuxParser::Ram(int pid) {
  string line, key, ram;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> ram) {
        if (key == "VmSize") return to_string((stoi(ram) / 1024));  // [MB]
      }
    }
  }
  return string();
}

string LinuxParser::Uid(int pid) {
  string line, key, uid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> uid) {
        if (key == "Uid") return uid;
      }
    }
  }
  return string();
}

string LinuxParser::User(int pid) {
  string userid = Uid(pid);
  string line, user, pr, id;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> pr >> id) {
        if (id == userid) {
          return user;
        }
      }
    }
  }
  return string();
}

long LinuxParser::UpTime(int pid) {
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; ++i) linestream >> value;  // 22 starttime;
    return stol(value);
  }
  return 0;
}

long LinuxParser::GetHertz() { return sysconf(_SC_CLK_TCK); }