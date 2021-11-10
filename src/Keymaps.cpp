
#include "Keymaps.h"
#include <mast_tk/MenuUtils.h>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <mast_tk/NotificationUtils.h>
#include <fstream>
#include <mast_tk/ExecUtils.h>
#include <iostream>
#include <xkbcommon/xkbregistry.h>

namespace MastWarden {
  KeymapMenu::KeymapMenu(){
    CompileMenu();
  }

  MENU* KeymapMenu::GetRawMenu(){
    return m_Menu;
  }

  void KeymapMenu::CompileMenu(){
   int32_t SIZE;
   rxkb_context* context  = rxkb_context_new(RXKB_CONTEXT_NO_FLAGS);
   rxkb_context_include_path_append_default(context);
   rxkb_context_parse(context,"evdev");
   rxkb_layout* layout = rxkb_layout_first(context);
   while(layout != NULL){

     m_Titles.push_back(rxkb_layout_get_description(layout));
     m_Layouts.push_back(rxkb_layout_get_name(layout));
     layout = rxkb_layout_ref(layout);
     layout = rxkb_layout_next(layout);
     SIZE += 1;
   }
    m_Size = SIZE;
    std::cout << std::to_string(SIZE) << std::endl;
  }

  KeymapMenu::~KeymapMenu(){
    Free();
  }


  void KeymapMenu::Construct(){
    
    m_Items = (ITEM**)calloc(m_Size+1, sizeof(ITEM*));
    for (int i = 0; i < m_Size; ++i){
      m_Items[i] = new_item(m_Titles.data()[i], m_Layouts.data()[i]);
    }
    m_Items[m_Size] = (ITEM*)NULL;
    m_Menu = new_menu((ITEM**)m_Items);
  }

  const char** KeymapMenu::GetItemsArray(){
    return m_Titles.data();
  }

  int KeymapMenu::GetFirstColor(){
    return COLOR_BLACK;
  }

  int KeymapMenu::GetSecondColor(){
    return COLOR_BLUE;
  }

  void KeymapMenu::ProcessInput(int key){
        m_State.Key = key;
    switch(m_State.Key){
    case KEY_DOWN:
      menu_driver(m_Menu,REQ_DOWN_ITEM);
      m_Choice++;
      if(m_Choice >= m_Size){
	m_Choice = m_Size - 1;
      }
      break;
    case KEY_UP:
      menu_driver(m_Menu,REQ_UP_ITEM);
      m_Choice--;
      if (m_Choice <= 0){
	m_Choice = 0; 
      }
      break;
    case KEY_NPAGE:
      menu_driver(m_Menu,REQ_SCR_DPAGE);
      break;
    case KEY_PPAGE:
      menu_driver(m_Menu,REQ_SCR_UPAGE);
      break;
    case 10:
      OnMenuChoose(m_Choice);
      break;
    }
    refresh();
  }

  void KeymapMenu::OnMenuChoose(int choice){
    m_ChoiceString = std::string(m_Layouts[choice]);
    SaveChoice(choice);
  }


  void KeymapMenu::SaveChoice(int choice){
    std::string cmd("setxkbmap -layout ");
    cmd += m_ChoiceString;
    system(cmd.c_str());
  }

  void KeymapMenu::Post(){
    post_menu(m_Menu);
  }


  MastTDE::MenuState&  KeymapMenu::GetState(){
    return m_State;
  }

  void KeymapMenu::Free(){
    unpost_menu(m_Menu);
    for (int i = 0; i < m_Size; ++i){
      free_item(m_Items[i]);
    }
    free_menu(m_Menu);
  }
  
}
