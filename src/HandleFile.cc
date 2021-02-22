#include "../include/HandleFile.hpp"
#include <io.h>
#include <sys/stat.h>
#include <iostream>
#define M_DEBUG 2
void getFiles(std::string folderpath, std::vector<std::string>& files)
{
  long hFile = 0;
  struct _finddata_t fileinfo;
  std::string p;
  if ((hFile =_findfirst(p.assign(folderpath).append("\\*").c_str(), &fileinfo)) != -1){
    do {
      if (fileinfo.attrib & _A_SUBDIR){
        if (strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name, "..") != 0){
          ;
        }
      }else{
        files.push_back(p.assign(folderpath).append("\\").append(fileinfo.name));
      }
    }while( _findnext(hFile, &fileinfo) == 0);
  }
  _findclose(hFile);
}
void handleFile(std::string command, std::string file, std::vector<std::string> args = {} , std::vector<std::string> args_before = {})
{
  // handle args_before 
  std::string run = command;
  if(!args_before.empty()) {
    for (std::string& str:args_before){
      run += " " + str;
    }
  }
  run += " " + file;
  if (!args.empty()){
    for (std::string& arg:args){
#if M_DEBUG == 1
      std::cout << arg << std::endl;
#endif
      run += " " + arg;
    }
  }
#if M_DEBUG == 1
  std::cout << run << std::endl;
#endif
  system(run.c_str());
}

void handleinput::getMode(int argc, char *argv[])
{
  // the command formate should be like : HandleFile command -[mode] dir/file
  std::string mode = argv[2];
  if (mode == "-S"){
#if M_DEBUG == 1
  std::cout << " single_file mode " << std::endl;
#endif
    imode = inputMode::single_file;
  }else if (mode == "-M"){
    imode = inputMode::multi_files;
  }else if (mode == "-SM"){
    imode = inputMode::single_file_with_multi_times;
  }else if (mode == "-MM"){
    imode = inputMode::multi_files_with_multi_times;
  }else{
    imode = inputMode::def;
  }
}

void handleinput::operator()(int argc, char *argv[])
{
  if (!valid(argc, argv)){
    std::cout << "formate wrong" << std::endl;
    std::cout << "HandleInput command -[mode:S|M] arg -A args" << std::endl;
    exit(1);
  }
  getMode(argc, argv);
  switch(imode) {
    case inputMode::single_file:
    case inputMode::single_file_with_multi_times:
      handleSingleFile(argc,argv);
      break;
    case inputMode::multi_files_with_multi_times:
    case inputMode::multi_files:
      handleMultiFiles(argc,argv);
      break;
    default:
      std::cout << "no mode like this" << std::endl;
      break;
  }
}

void handleinput::handleSingleFile(int argc, char *argv[])
{
  std::vector<std::string> args;
  std::vector<std::string> args_before;
  // handle args_before
  int iterat_int = 3;
  for (; iterat_int < argc && strcmp(argv[iterat_int],"-args") != 0; ++iterat_int){
    args_before.push_back(argv[iterat_int]);
  }
  std::string path = args_before.back();
  args_before.erase(--args_before.end());
  // handle args
  iterat_int++;
  for (; iterat_int < argc; ++iterat_int){
    args.push_back(argv[iterat_int]);
  }
// #if M_DEBUG == 1
//   std::cout << "argc " << argc << std::endl;
//   std::cout << "argv[4] " << (argc > 4 ? argv[4] : "") << std::endl;
// #endif
//   if (argc > 5 ){
// #if M_DEBUG == 1
//   std::cout << " condition argc > 5 is true " << std::endl;
// #endif
//     if ( strcmp(argv[4],"-A") == 0 ) {
//       // args.reserve(argc - 5);
//       for (int i=5; i < argc; ++i) {
//         args.push_back(argv[i]);
//       }
// #if M_DEBUG == 1
//   std::cout << "handleSingleFile read -A" << std::endl;
//   std::cout << "args size " <<  args.size() << std::endl;
// #endif
//     }else{
//       std::cout << " format wrong :  " << std::endl;
//       std::cout << "HandleInput command -[mode:S|M|SM|MM] arg -args args [run times]" << std::endl;
//     }
//   }
  int run_times = 1;
  if (strcmp(argv[2],"-SM") == 0){
    run_times = std::atoi(argv[argc-1]);
    args.erase(--args.end());
  }
  if (run_times <= 0 || run_times > 200){
    run_times = 1;
  }
  for (int i = 0; i < run_times; ++i){
    handleFile(argv[1], path,args,args_before);
  }
}

void handleinput::handleMultiFiles(int argc, char *argv[])
{
  std::vector<std::string> args;
  std::vector<std::string> args_before;
  // handle args_before
  int iterat_int = 3;
  for (; iterat_int < argc && strcmp(argv[iterat_int],"-args") != 0; ++iterat_int){
    args_before.push_back(argv[iterat_int]);
  }
  std::string path = args_before.back();
  args_before.erase(--args_before.end());
  // handle args
  iterat_int++;
  for (; iterat_int < argc; ++iterat_int){
    args.push_back(argv[iterat_int]);
  }
// #if M_DEBUG == 1
//   std::cout << "argc " << argc << std::endl;
//   std::cout << "argv[4] " << (argc > 4 ? argv[4] : "") << std::endl;
// #endif
//   if (argc > 5 ){
// #if M_DEBUG == 1
//   std::cout << " condition argc > 5 is true " << std::endl;
// #endif
//     if ( strcmp(argv[4],"-A") == 0 ) {
//       // args.reserve(argc - 5);
//       for (int i=5; i < argc; ++i) {
//         args.push_back(argv[i]);
//       }
// #if M_DEBUG == 1
//   std::cout << "handleSingleFile read -A" << std::endl;
//   std::cout << "args size " <<  args.size() << std::endl;
// #endif
//     }else{
//       std::cout << " format wrong :  " << std::endl;
//       std::cout << "HandleInput command -[mode:S|M] arg -A args" << std::endl;
//     }
//   }
  int run_times = 1;
  if (strcmp(argv[2],"-MM") == 0){
    run_times = std::atoi(argv[argc-1]);
    args.erase(--args.end());
  }
  if (run_times <= 0 || run_times > 200){
    run_times = 1;
  }
  getFiles(path,files);
#if M_DEBUG == 1
  std::cout << "files size " << files.size() << std::endl;
  for (std::string& str:files) {
    std::cout << str << std::endl;
  }
#endif
  for (std::string& file:files){
    for (int i=0 ; i < run_times; ++i){
      handleFile(argv[1], file, args,args_before);
    }
  }
}

bool handleinput::valid(int argc, char *argv[])
{
  if (argc >= 4) {
    return true;
  }else{
    return false;
  }
}
