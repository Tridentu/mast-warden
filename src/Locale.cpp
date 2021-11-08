
#include "Locale.h"
#include <mast_tk/MenuUtils.h>
#include <unicode/locid.h>
#include <unicode/unistr.h>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <mast_tk/NotificationUtils.h>
#include <fstream>
#include <mast_tk/ExecUtils.h>
#include <iostream>

namespace MastWarden {
  LocaleMenu::LocaleMenu(){
    CompileMenu();
  }

  MENU* LocaleMenu::GetRawMenu(){
    return m_Menu;
  }

  void LocaleMenu::CompileMenu(){
   int32_t SIZE;
   const icu::Locale* locArr1 = NULL;
   locArr1 = icu::Locale::getAvailableLocales(SIZE);
    for (int i = 0; i < SIZE; ++i) {
       UErrorCode ec = U_ZERO_ERROR;
      icu::Locale locArr = locArr1[i];
      icu::UnicodeString ls = icu::UnicodeString((std::string(locArr.getBaseName()) + std::string(".UTF-8")).c_str());
      std::string ls2;
      ls.toUTF8String(ls2);
      char* localeSS = (char*)calloc((sizeof(ls2) / sizeof(char)) + 1, sizeof(char));
      ls.extract(localeSS, (sizeof(ls2) / sizeof(char)) + 1, NULL,  ec );
      {
	m_Locales.push_back(localeSS);
	icu::UnicodeString name2 = "";
	name2 = locArr.getDisplayName(name2);
	std::string localeDisplay;
	name2.toUTF8String(localeDisplay);
	char* displayStr = (char*)calloc((sizeof(localeDisplay) / sizeof(char)) + 1, sizeof(char));
	name2.extract(displayStr, sizeof(localeDisplay) / sizeof(char), NULL, ec);
	m_Titles.push_back(displayStr);
      }
    }

    m_Size = SIZE;
  }

  LocaleMenu::~LocaleMenu(){
    Free();
  }


  void LocaleMenu::Construct(){
    
    m_Items = (ITEM**)calloc(m_Size+1, sizeof(ITEM*));
    for (int i = 0; i < m_Size; ++i){
      m_Items[i] = new_item(m_Titles.data()[i], m_Locales.data()[i]);
    }
    m_Items[m_Size] = (ITEM*)NULL;
    m_Menu = new_menu((ITEM**)m_Items);
  }

  const char** LocaleMenu::GetItemsArray(){
    return m_Titles.data();
  }

  int LocaleMenu::GetFirstColor(){
    return COLOR_BLACK;
  }

  int LocaleMenu::GetSecondColor(){
    return COLOR_BLUE;
  }

  void LocaleMenu::ProcessInput(int key){
        m_State.Key = key;
    switch(m_State.Key){
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

  void LocaleMenu::OnMenuChoose(int choice){
    m_ChoiceString = std::string(m_Locales[choice]);
    SaveChoice(choice);
  }


  void LocaleMenu::SaveChoice(int choice){
    std::vector<std::string> lines;
    std::ifstream lineIn("/etc/setloader/i18n.setl");
    std::string text;
    while(getline(lineIn,text)){
      lines.push_back(std::string(text));
    }
    lineIn.close();
    std::ofstream i18nFile;
    std::stringstream tzs;
    i18nFile.open("/etc/setloader/i18n.setl");
    tzs << "export LANG=" << m_Locales[choice]  << std::endl;
    i18nFile << tzs.str();
    i18nFile << lines[1] << std::endl;
    i18nFile.close();

    
    MastTK::runAsUser(getenv("USER"), [this,choice](){
          std::stringstream msgStr;
	  msgStr << "The current locale  was changed to " <<  this->m_Titles[choice] << "("  << m_Locales[choice] << ").";
          MastTDE::NotificationPinger* pinger = new MastTDE::NotificationPinger("MastWarden");
	  pinger->Ping("Locale Saved",msgStr.str());
    });
    

    
  }

  void LocaleMenu::Post(){
    post_menu(m_Menu);
  }


  MastTDE::MenuState&  LocaleMenu::GetState(){
    return m_State;
  }

  void LocaleMenu::Free(){
    unpost_menu(m_Menu);
    for (int i = 0; i < m_Size; ++i){
      free_item(m_Items[i]);
    }
    free_menu(m_Menu);
  }
  
}
