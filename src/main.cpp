#include "TimeZones.h"
#include <curses.h>


int main(){

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  MastWarden::TimeZoneMenu* menu = new MastWarden::TimeZoneMenu();

  
  menu->Construct();

  menu->Post();
  refresh();
  int c;
  while((c = getch()) != KEY_F(8)){
    menu->ProcessInput(c);
    refresh();
  }
  delete menu;
  endwin();
}
