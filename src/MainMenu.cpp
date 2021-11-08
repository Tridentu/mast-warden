

#include "MainMenu.h"
#include "TimeZones.h"
#include "Locale.h"
#include <stdlib.h>
#include "MenuUtils.h"

namespace MastWarden {
  const char** MainMenu::GetItemsArray(){
  	std::vector<const char*> strings;
  	strings.push_back(std::string("Change Time Zone").c_str());
  	strings.push_back(std::string("Change Locale").c_str());
  	strings.push_back(std::string("Exit").c_str());

    return strings.data();
  }

  MainMenu::MainMenu(){
    m_Keys.push_back("TZ");
    m_Keys.push_back("LANG");
    m_Keys.push_back(".exit");
    AddMenu("TZ", [](){
        TimeZoneMenu* menuTZ = new TimeZoneMenu();
        menuTZ->Construct();
        menuTZ->Post();
         refresh();
	  int ck;
	  while((ck = getch()) != KEY_F(8)){
	    menuTZ->ProcessInput(ck);
	    refresh();
	  }
	  delete menuTZ;
    });
     AddMenu("LANG", [](){
        LocaleMenu* menuTZ = new LocaleMenu();
        menuTZ->Construct();
        menuTZ->Post();
         refresh();
	  int ck;
	  while((ck = getch()) != KEY_F(8)){
	    menuTZ->ProcessInput(ck);
	    refresh();
	  }
	  delete menuTZ;
    });
  }


   MainMenu::~MainMenu(){
        Free();
   }

   
  void MainMenu::CompileMenu(){
    const char* items[] {
    	"Change Time Zone",
    	"Change Locale",
    	"Exit"
    };
    int SIZE = 2;
    m_Size = SIZE + 1;
    m_Items = (ITEM**)calloc(m_Size + 1, sizeof(ITEM*));
    for (int i = 0; i < m_Size; i++)
      m_Items[i] = new_item(items[i], m_Keys[i]);
    m_Items[m_Size] = (ITEM*)NULL;
    m_Menu = new_menu((ITEM**)m_Items);
    
  }
  
  MastTDE::MenuState& MainMenu::GetState(){
  	return m_State;
  }
  
  void MainMenu::Construct(){
    
  }

  MENU* MainMenu::GetRawMenu(){
    return  m_Menu;
  }
  
  void MainMenu::Free(){
      unpost_menu(m_Menu);
      for (int i = 0; i < m_Size; ++i){
	      free_item(m_Items[i]);
      }   
       free_menu(m_Menu);
  }


  const bool MainMenu::IsRunning(){
     return !m_State.Off;
  }

 void MainMenu::Post(){
   post_menu(m_Menu);
 }
 void MainMenu::RefreshMenu(){
 	CompileMenu();
      	Post();
      	DrawMenuTitle();
      	refresh();
 }
 
 void MainMenu::ProcessInput(int key){
 	switch(key){
	    case KEY_DOWN:
	      menu_driver(m_Menu,REQ_DOWN_ITEM);
	      m_Choice++;
	      if(m_Choice >= m_Size){
		m_Choice = 0;
	      }
	      break;
	    case KEY_UP:
	      menu_driver(m_Menu,REQ_UP_ITEM);
	      m_Choice--;
	      if (m_Choice <= 0){
		m_Choice = m_Size - 1; 
	      }
	      break;
	    case 10:
	      OnMenuChoose(m_Choice);
	      break;
 	}
 }
 
  void MainMenu::OnMenuChoose(int choice)
  {
    std::string choiceName = m_Keys[choice];
    if (choiceName == ".exit" && m_Mode == MastTDE::MenuMode::Main){
      m_State.Off = true;
      
    } else {
      auto it = m_Menus.find(choiceName);
      if (it != m_Menus.end()) {
	m_CurrentMenu = choice;
	m_Mode = MastTDE::MenuMode::Secondary;
	Free();
	it->second();
      	if (m_Mode == MastTDE::MenuMode::Secondary){
      	    m_Mode = MastTDE::MenuMode::Main;
      	    
      	    RefreshMenu();
      	    
      	}
      }
      }
  }

  void MainMenu::AddMenu(std::string name, std::function<void()> processor){
    m_Menus[name] = processor;
  }

 void MainMenu::DrawMenuTitle(){

  if(m_Mode == MastTDE::MenuMode::Secondary){
    mvprintw(LINES - 4, 0, GetItemsArray()[m_CurrentMenu]);
    mvprintw(LINES - 3, 0, "Press F8 to exit this menu.");
    } else {
      mvprintw(LINES - 4, 0, "MastWarden");
 } 
}
}
