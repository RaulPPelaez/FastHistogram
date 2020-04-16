/*Raul P. Pelaez 2020. Histogram utility.

  Given a two column input through stdin computes a weighted histogram.
  The first column of the output is the center position of each bin, the second contains the un normalized histogram (the sum of weigths falling inside each bin).

  USAGE:
    histogram -u [upper limit] -l [lower limit] -n [number of intervals]
    
  EXAMPLE:
   Histogram of an uniform distribution

   seq 1 10000 | awk '{print rand(), 1}' | histogram -l 0 -u 1 -n 10000 > flatHistogram.dat

 */
#include<fstream>
#include <vector>
#include"third_party/superio/superIO.h"

using real = double;

struct Config{
  std::string fileName = "/dev/stdin";
  real lowerLimit;
  real upperLimit;
  int numberIntervals;
};

class CommandLineParser{
  int m_argc;
  char **m_argv;
public:
  enum OptionType {Required, Optional};
  CommandLineParser(int argc, char **argv): m_argc(argc), m_argv(argv){}
  
  std::istringstream getArgumentOfFlag(const char *flag,
				       OptionType type){
    for(int i=1; i<m_argc; i++){
      if(strcmp(flag, m_argv[i]) == 0){
	std::string line;
	if(i == m_argc-1){
	  break;
	}
	for(int j=i+1; j<m_argc; j++){	  
	  line += m_argv[j];
	  line += " ";
	}
	std::istringstream ss(line);
	return ss;
      }
    }
    if(type == Required)
      throw std::runtime_error(("Option " + std::string(flag) + " not found").c_str());
    return std::istringstream();
  }
  
};

std::vector<real> processSignal(Config config){
  std::vector<real> histogram(config.numberIntervals, 0);
  superIO::superInputFile file(config.fileName);
  if(!file.good()){
    throw std::runtime_error("Input ended before expected");
  }
  
  while(!file.eof()){
    char* line;
    int numberChars = file.getNextLine(line);
    real lineNumbers[2];
    superIO::string2numbers(line, numberChars, 2, lineNumbers);
    real position = lineNumbers[0];
    real weight = lineNumbers[1];
    int interval = config.numberIntervals*(position-config.lowerLimit)/(config.upperLimit - config.lowerLimit);
    if(interval >= config.numberIntervals or interval < 0){
      continue;
    }
    histogram[interval] += weight;
  }
  return std::move(histogram);
}

void printHelp();

int main(int argc, char *argv[]){
  Config config;
  CommandLineParser parser(argc, argv);
  try{
    parser.getArgumentOfFlag("-l", CommandLineParser::Required)>>config.lowerLimit;
    parser.getArgumentOfFlag("-u", CommandLineParser::Required)>>config.upperLimit;
    parser.getArgumentOfFlag("-n", CommandLineParser::Required)>>config.numberIntervals;
  }
  catch(...){
    std::cerr<<"ERROR: Missing options"<<std::endl;
    printHelp();
    exit(1);
  }
  auto histogram = processSignal(config);

  for(int i = 0; i< config.numberIntervals; i++){
    real position = (i+0.5)/(real)config.numberIntervals*(config.upperLimit - config.lowerLimit) + config.lowerLimit;
    std::cout<<position<<"  "<<histogram[i]<<"\n";
  }
  
  return 0;
}


void printHelp(){
std::cerr<<"  Raul P. Pelaez 2020. Histogram utility."<<std::endl;
std::cerr<<""<<std::endl;
std::cerr<<"  Given a two column input through stdin computes a weighted histogram."<<std::endl;
std::cerr<<"  The first column of the output is the center position of each bin, the second contains the un normalized histogram (the sum of weigths falling inside each bin)."<<std::endl;
std::cerr<<""<<std::endl;
std::cerr<<"  USAGE:"<<std::endl;
std::cerr<<"    histogram -u [upper limit] -l [lower limit] -n [number of intervals]"<<std::endl;
std::cerr<<"    "<<std::endl;
std::cerr<<"  EXAMPLE:"<<std::endl;
std::cerr<<"   Histogram of an uniform distribution"<<std::endl;
std::cerr<<""<<std::endl;
std::cerr<<"   seq 1 10000 | awk '{print rand(), 1}' | histogram -l 0 -u 1 -n 10000 > flatHistogram.dat"<<std::endl;
}
