#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
string line;
  string key;
  string value;
  float mem_total, mem_free;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") mem_total = std::stof(value);
        if (key == "MemFree:") mem_free = std::stof(value);
      }
    }
  }
  return (mem_total-mem_free)/mem_total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime, iddletime;
  string line;
  long uptime_long;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line); //passou a linha do conteudo de stream para a string line
    std::istringstream linestream(line);
    linestream >> uptime >> iddletime;
    uptime_long = std::stol(uptime);
  }
  return uptime_long;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::UpTime() * sysconf(_SC_CLK_TCK); 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpu_procs;
  string cpu;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> cpu){
      cpu_procs.push_back(cpu);
    } 
  }
  return cpu_procs;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int total_processes;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") total_processes = std::stoi(value);
      }
    }
  }
  return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  int running_processes;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") running_processes = std::stoi(value);
      }
    }
  }
  return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  string process_command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> process_command; 
  }
  return process_command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  string ram_usage;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") ram_usage = value;
      }
    }
  }
  int ram_mb = (std::stoi(ram_usage))/1000;
  string ram_usage_mb = to_string(ram_mb); 
  return ram_usage_mb;   
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  string process_uid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") process_uid = value;
      }
    }
  }
  return process_uid;  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function 
//example:			root:x:0:0:root:/root:/bin/bash
string LinuxParser::User(int pid) { 
 string user_id = LinuxParser::Uid(pid); 
  //test
 //return user_id; 
 string user_name, id;
  string line;
  string ret_user;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::istringstream linestream(line);
      while (linestream >> user_name >> id) {
        if (id == user_id) {
          ret_user = user_name;
        }
      }
    }
  }
  return ret_user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  string uptime;
  //long uptime_long;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int i=0;i<22;i++){ //22 is the index for uptime in the file
        linestream >> uptime;
      }     
    }
  }
  
  //as said in review, the calculation was wrong.
  //uptime_long = stol(uptime);
  //return (uptime_long / sysconf(_SC_CLK_TCK));  
  
  int upTimePid = UpTime() - stol(uptime)/sysconf(_SC_CLK_TCK);
  return upTimePid;  
}

/* contagem começãndo do 1
#14 utime - CPU time spent in user code, measured in clock ticks
#15 stime - CPU time spent in kernel code, measured in clock ticks
#22 starttime - Time when the process started, measured in clock ticks */
//então na verdade é 13,14,21


float LinuxParser::ProcessCPU(int pid){
string line;
  string data;
  float utime, stime, cutime, cstime, starttime, total_time, seconds, cpu_usage;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int i=1;i<23;i++){ //22 is the index for uptime in the file
        linestream >> data;
        if (i==14) utime = std::stof(data);
        else if (i==15) stime = std::stof(data);
        else if (i == 16) cutime = std::stof(data);
        else if (i == 17) cstime = std::stof(data);
        else if (i == 22)starttime = std::stof(data);
      }     
    }
  }  
  total_time = utime + stime + cutime + cstime;
  seconds = LinuxParser::UpTime() - (starttime / sysconf(_SC_CLK_TCK)); 
  cpu_usage = ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
  return (cpu_usage);
}


