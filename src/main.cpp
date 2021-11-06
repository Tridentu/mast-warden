#include "MainMenu.h"
#include <curses.h>


int main(){

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  MastWarden::MainMenu* menu = new MastWarden::MainMenu();

  menu->CompileMenu();
  
  menu->Construct();

  menu->Post();
  
  refresh();
  
  int c;
  while(menu->IsRunning()){
    c = getch();
    menu->ProcessInput(c);
    refresh();
  }
  delete menu;
  endwin();
}
