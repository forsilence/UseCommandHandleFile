#ifndef __HANDLE_FILE_HPP
#define __HANDLE_FILE_HPP
#include <vector>
#include <string>
void getFiles(std::string path, std::vector<std::string>& files);
void handleFile(std::string command, std::string file, std::vector<std::string> args, std::vector<std::string> args_before);
class handleinput{
  public:
  enum inputMode{def,
                single_file,
                multi_files,
                single_file_with_multi_times,
                multi_files_with_multi_times};
  void getMode(int argc, char *argv[]);
  bool valid(int argc, char *argv[]);
  void operator()(int argc, char *argv[]);
  void handleSingleFile(int argc, char *argv[]);
  void handleMultiFiles(int argc, char *argv[]);
  private:
  inputMode imode;
  std::vector<std::string> files;
};
#endif