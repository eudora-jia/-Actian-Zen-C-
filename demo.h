{\rtf1\ansi\ansicpg936\cocoartf1671\cocoasubrtf500
{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\paperw11900\paperh16840\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx566\tx1133\tx1700\tx2267\tx2834\tx3401\tx3968\tx4535\tx5102\tx5669\tx6236\tx6803\pardirnatural\partightenfactor0

\f0\fs24 \cf0 #include <stdio.h>\
#include <string>\
#include <iostream>\
#include <stdlib.h>\
#include <stdint.h>\
#include <unistd.h>\
#include <regex>\
#include <vector>\
#include "btrieveCpp.h"\
using namespace std;\
static char *btrieveFileName = (char *)"coretemp.btr";\
typedef struct CpuStatus \{\
   double temperature;\
   double frequency;\
   double* cpuLoad;\
\} record_t;\
double toDouble(std::string s) \{\
   std::replace(s.begin(), s.end(), ',', '.');\
   return std::atof(s.c_str());\
   \}\
string do_console_command_get_result(char* command) \{\
   FILE* pipe = popen(command, "r");\
   if (!pipe)\
      return "ERROR";\
   char buffer[128];\
   string result = "";\
   while (!feof(pipe))\
   \{\
      if (fgets(buffer, 128, pipe) != NULL)\
         result += buffer;\
   \}\
   pclose(pipe);\
   return(result);\
\}\
double getTemperature(int zone) \{\
   char buffer[100];\
   sprintf(buffer, "cat /sys/class/thermal/thermal_zone%d/temp", zone);\
   string result = do_console_command_get_result(buffer);\
   if (result.size() > 0)  result.resize(result.size() - 1);\
   return toDouble(result);\
\}\
double getCpuFrequency(int cpu) \{\
   char buffer[100];\
   sprintf(buffer, "cat /sys/devices/system/cpu/cpu%d/cpufreq/cpuinfo_cur_freq", cpu);\
   string result = do_console_command_get_result(buffer);\
   if (result.size() > 0)  result.resize(result.size() - 1);\
   return toDouble(result);\
\}\
vector<string> tokenize(const string& str, char delim) \{\
   vector<std::string> tokens;\
   std::stringstream mySstream(str);\
   string temp;\
   while (getline(mySstream, temp, delim))\
      tokens.push_back(temp);\
      return tokens;\
\}\
double *getMpStat() \{\
   double* result = new double[4];\
   std::regex regex(R"(\\d\{2\}:\\d\{2\}:\\d\{2\}\\s+(\\d)\\s+\\d+[.,]\\d\{2\}\\s+\\d+[.,]\\d\{2\}\\s+\\d+[.,]\\d\{2\}\\s+\\d+[.,]\\d\{2\}\\s+\\d+[.,]\\d\{2\}\\s+\\d+[.,]\\d\{2\}\\s+\\d+[.,]\\d\{2\}\\s+\\d+[.,]\\d\{2\}\\s+\\d+[.,]\\d\{2\}\\s+(\\d+[.,]\\d\{2\}))");\
   string s = do_console_command_get_result((char*)"mpstat -P ALL 1 1");\
   vector<string> lines = tokenize(s, '\\n');\
   for (size_t i = 0; i < lines.size(); ++i)\
   \{\
      smatch matches;\
      if (regex_search(lines[i], matches, regex))\
      \{\
         int cpunum = stoi(matches[1].str());\
         if (cpunum >= 0 && cpunum < 4) \{\
            result[cpunum] = 100 - toDouble(matches[2].str());\
         \}\
      \}\
   \}\
   return result;\
\}\
void printHelp() \{\
   printf("\\n\\nUsage:\\n coretempmon [options]\\n\\n");\
   printf("-h            This help\\n");\
   printf("-s            Silent output, just data");\
   printf("-d <milliseconds>  Monitor delay in seconds (default=5)\\n");\
\}\
void monitoring(unsigned int delay, int loops, bool silent) \{\
   if (!silent) \{\
   printf("Core Temperature Monitor 1.0\\n");\
   printf("Scan delay is %d seconds\\n", delay);\
   printf("Stop monitoring using [ctrl]-[c]\\n");\
   printf("Time Temperature \\u2103 Freq_CPU1 CPULoad1  %%CPULoad2 \\%%CPULoad3 \\%%CPULoad3\\n");\
   \}\
   \
   bool infinite = (loops == -1);\
   int counter = loops;\
   record_t data;\
      // Getting the CPU stats\
   while (infinite || counter-- >0)\
   \{\
      usleep(delay * 1000);\
      data.cpuLoad = getMpStat();\
      data.frequency = getCpuFrequency(0);\
      data.temperature = getTemperature(0);\
   \
      printf("%0.2f \\u2103 %0.0f MHz %0.2f %0.2f %0.2f %0.2f\\n", data.temperature, data.frequency, data.cpuLoad[0], data.cpuLoad[1], data.cpuLoad[2], data.cpuLoad[3]);\
      data.cpuLoad;\
   \}\
\}\
static Btrieve::StatusCode\
createFile(BtrieveClient *btrieveClient) \{\
   Btrieve::StatusCode status;\
   BtrieveFileAttributes btrieveFileAttributes;\
   // If Seting Fixed Record Length fails\
   if ((status = btrieveFileAttributes.SetFixedRecordLength(sizeof(record_t))) != Btrieve::STATUS_CODE_NO_ERROR)\
   \{\
      printf("Error: BtrieveFileAttributes::SetFixedRecordLength():%d:%s.\\n", status, Btrieve::StatusCodeToString(status));\
      goto leave;\
   \}\
   // If Creating file fails\
   if ((status = btrieveClient->FileCreate(&btrieveFileAttributes, btrieveFileName, Btrieve::CREATE_MODE_OVERWRITE)) != Btrieve::STATUS_CODE_NO_ERROR)\
   \{\
      printf("Error: BtrieveClient::FileCreate():%d:%s.\\n", status, Btrieve::StatusCodeToString(status));\
      goto leave;\
   \}\
leave:\
   return status;\
\}\
static Btrieve::StatusCode\
openFile(BtrieveClient *btrieveClient, BtrieveFile *btrieveFile) \{\
   Btrieve::StatusCode status;\
   // If opening file fails\
   if ((status = btrieveClient->FileOpen(btrieveFile, btrieveFileName, NULL, Btrieve::OPEN_MODE_NORMAL)) != Btrieve::STATUS_CODE_NO_ERROR)\
   \{\
      printf("Error: BtrieveClient::FileOpen():%d:%s.\\n", status, Btrieve::StatusCodeToString(status));\
      goto leave;\
   \}\
leave:\
   return status;\
\}\
static Btrieve::StatusCode\
loadFile(BtrieveFile *btrieveFile) \{\
   Btrieve::StatusCode status = Btrieve::STATUS_CODE_NO_ERROR;\
   return status;\
\}\
static Btrieve::StatusCode\
closeFile(BtrieveClient *btrieveClient, BtrieveFile *btrieveFile) \{\
   Btrieve::StatusCode status;\
   // Closing the File fails.\
   if ((status = btrieveClient->FileClose(btrieveFile)) != Btrieve::STATUS_CODE_NO_ERROR)\
   \{\
      printf("Error: BtrieveClient::FileClose():%d:%s.\\n", status, Btrieve::StatusCodeToString(status));\
      goto leave;\
   \}\
leave:\
   return status;\
\}}