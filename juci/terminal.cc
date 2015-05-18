#include "terminal.h"
#include <iostream>
#include <thread>
#include "logging.h"


Terminal::Config::Config() {
}
Terminal::Config::Config(Terminal::Config& original) {
  for (auto it = 0; it<original.compile_commands().size(); ++it) {
    InsertCompileCommand(original.compile_commands().at(it));
  }
}

void Terminal::Config::InsertCompileCommand(std::string command){
  compile_commands_.push_back(command);
}

Terminal::View::View(){
  scrolledwindow_.add(textview_);
  scrolledwindow_.set_size_request(-1,150);
  view_.add(scrolledwindow_);
  textview_.set_editable(false);
}


Terminal::Controller::Controller(Terminal::Config& cfg) :
  config_(cfg) {  
  folder_command_ = "";
}

void Terminal::Controller::SetFolderCommand( boost::filesystem::path
					     CMake_path) {
  INFO("Terminal: SetFolderCommand");
  path_ = CMake_path.string();
  folder_command_ = "cd "+ path_ + "; ";
}

void Terminal::Controller::Compile(){
  INFO("Terminal: Compile");
  Terminal().get_buffer()->set_text("");
  DEBUG("Terminal: Compile: running cmake command");
  std::vector<std::string> commands = config().compile_commands();
  for (auto it = 0; it < commands.size(); ++it) {
    ExecuteCommand(commands.at(it), "r");
    
  }
  // ExecuteCommand("rm -rf ./.build", "r");
  // ExecuteCommand("mkdir ./.build", "r");
  // ExecuteCommand("cmake -B./build -H.", "r");
  // if (ExistInConsole(cmake_sucsess)){
  //   DEBUG("Terminal: Compile: running make command");
  //   ExecuteCommand("cd ./.build/; make", "r");
  // }
  PrintMessage("\n");
  DEBUG("Terminal: Compile: compile done");
}

void Terminal::Controller::Run(std::string executable) {
  INFO("Terminal: Run");
  PrintMessage("juCi++ execute: " + executable + "\n");
  DEBUG("Terminal: Compile: running run command: ");
  DEBUG_VAR(executable);
  ExecuteCommand("cd ./build/; ./"+executable, "r");
  PrintMessage("\n");
}

void Terminal::Controller::PrintMessage(std::string message){
  INFO("Terminal: PrintMessage");
  Terminal().get_buffer()->
    insert(Terminal().get_buffer()-> end(),"> "+message);
}


bool Terminal::Controller::ExistInConsole(std::string string) {
    INFO("Terminal: ExistInConsole");
    DEBUG("Terminal: PrintMessage: finding string in buffer");
  double pos = Terminal().get_buffer()->
    get_text().find(string);
  if (pos == std::string::npos) return false;
  return true;
}

void Terminal::Controller::ExecuteCommand(std::string command, std::string mode) {
  INFO("Terminal: ExecuteCommand");
  command = folder_command_+command;
  DEBUG("Terminal: PrintMessage: Running command");
  FILE* p = NULL;
  p = popen(command.c_str(), mode.c_str());
  std::cout << "KJØRTE FINT!" << std::endl;
  if (p == NULL) {
    PrintMessage("juCi++ ERROR: Failed to run command" + command + "\n");
  }else {
    std::cout << "SKRIVER UT KOMMANDO RESULAT" << std::endl;
    char buffer[1028];
    while (fgets(buffer, 1028, p) != NULL) {
      PrintMessage(buffer);
    }
    pclose(p); 
  }

}
