#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "processor.h"

using std::vector;
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  vector<string> cpu_vector = LinuxParser::CpuUtilization();
  //necessário tirar o primeiro, porque é o nome do cpu...a partir daí pega a posição no vetor
  float not_iddle;
  float iddle;
  float total;
  string aux;
  vector<int> cpu_int_vector;
  total = 0;
  iddle = 0;
  for (int i=10;i>0;i--){
   // aux = cpu_vector.back();
    //se o i for 4 ou 5, soma no iddle
    if (i ==4 || i==5) iddle += std::stof(cpu_vector.back());
    //nos outros soma no not iddle
    else not_iddle += std::stof(cpu_vector.back());
    cpu_vector.pop_back();
  }
  
  total = iddle + not_iddle;
    
  return(total - iddle)/(total);
}
