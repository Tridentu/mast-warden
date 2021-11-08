
#include <curses.h>
#include <menu.h>
#include <stdlib.h>

#include "TimeZones.h"
#include "MenuUtils.h"
#include "NotificationUtils.h"
#include <unicode/timezone.h>
#include <unicode/unistr.h>
#include <unicode/stringpiece.h>
#include <unicode/locid.h>
#include <string>
#include <unicode/strenum.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <mast_tk/ExecUtils.h>

namespace MastWarden{
  TimeZoneMenu::TimeZoneMenu(){
    CompileMenu();
  }

  void TimeZoneMenu::CompileMenu(){
    icu::Locale currentLocale;
    m_Size = 0;
    icu::StringEnumeration* enumAll = icu::TimeZone::createEnumeration( );
    UErrorCode status = U_ZERO_ERROR;
    const icu::UnicodeString *zone = enumAll->snext(status);
    while (zone != NULL && status == U_ZERO_ERROR){
      std::string name;
      zone->toUTF8String(name);
      UErrorCode status3 = U_ZERO_ERROR;
      char* nameChar = (char*)calloc((sizeof(name) / sizeof(char)) + 1,  sizeof(char));
      zone->extract(nameChar, sizeof(name) / sizeof(char), NULL, status3);
      icu::TimeZone* zone2 = icu::TimeZone::createTimeZone(icu::UnicodeString::fromUTF8(icu::StringPiece(name.c_str())));
      if (zone2){
	m_ZoneNames.push_back(nameChar);
	icu::UnicodeString name2 = "";
	name2 = zone2->getDisplayName(currentLocale, name2);
	std::string timeZoneDisplay;
	name2.toUTF8String<std::string>(timeZoneDisplay);
	UErrorCode status2 = U_ZERO_ERROR;
	char* display2  = (char*)calloc((sizeof(timeZoneDisplay) / sizeof(char)) + 1, sizeof(char));
	name2.extract(display2, sizeof(timeZoneDisplay) / sizeof(char), NULL, status2);
	m_Titles.push_back(display2);
	m_Size++;
      }
      zone = enumAll->snext(status);
    }
  }

  MENU* TimeZoneMenu::GetRawMenu(){
    return m_Menu;
  }

  void TimeZoneMenu::Construct(){
    printf("%i",m_Size);
    
    m_ItemsRaw = (ITEM**)calloc(m_Size+1, sizeof(ITEM*));
    for(int i = 0; i < m_Size; ++i){
      m_ItemsRaw[i] = new_item(m_Titles.data()[i],m_ZoneNames.data()[i]);
    }
    m_ItemsRaw[m_Size] = (ITEM*)NULL;
    m_Menu = new_menu((ITEM**)m_ItemsRaw);
  }

  const char** TimeZoneMenu::GetItemsArray(){
    return m_Titles.data();
  }

  int TimeZoneMenu::GetFirstColor(){
    return COLOR_BLACK;
  }

  int TimeZoneMenu::GetSecondColor(){
    return COLOR_BLUE;
  }

  void TimeZoneMenu::ProcessInput(int key){
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

  void TimeZoneMenu::OnMenuChoose(int choice){
    m_choiceString = std::string(m_ZoneNames[choice]);
    SaveChoice(choice);
  }

  void TimeZoneMenu::SaveChoice(int choice){
    std::vector<std::string> lines;
    std::ifstream lineIn("/etc/setloader/i18n.setl");
    std::string text;
    printf("Chose %s",m_Titles[choice]);
    while(getline(lineIn,text)){
      lines.push_back(std::string(text));
    }
    lineIn.close();
    int offset = 0;
    int dstOffset = 0;
    int hour = 0;
    {
      auto zone = icu::TimeZone::createTimeZone(icu::UnicodeString::fromUTF8(icu::StringPiece(m_choiceString.c_str())));
      double millis = (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      UErrorCode ec = U_ZERO_ERROR;
      zone->getOffset(millis, true, offset, dstOffset, ec);
      int seconds = (offset + dstOffset) / (1000);
      int minutes = seconds / 60;
      hour = (minutes / 60) * -1;
    }
    std::ofstream i18nFile;
    i18nFile.open("/etc/setloader/i18n.setl");
    i18nFile << lines[0] << std::endl;
    std::stringstream tzs;
    std::string offsetStr = "UTC";
    if (hour > 0 ){
      offsetStr = offsetStr + "+" + std::to_string(abs(hour));
    } else if (hour < 0) {
      offsetStr = offsetStr + "-" + std::to_string(abs(hour));
    } else {
      
    }
    
    tzs << "export TZ=" << offsetStr << std::endl;
    i18nFile << tzs.str();
    i18nFile.close();

    



    MastTK::runAsUser(getenv("USER"), [this,choice](){
          std::stringstream msgStr;
	  msgStr << "The current time zone was changed to " <<  this->m_Titles[choice] << ".";
          MastTDE::NotificationPinger* pinger = new MastTDE::NotificationPinger("MastWarden");
	  pinger->Ping("Time Zone Saved",msgStr.str());
    });
    

    
  }
  
  void TimeZoneMenu::Post(){
    post_menu(m_Menu);
  }


  MastTDE::MenuState&  TimeZoneMenu::GetState(){
    return m_State;
  }

  void TimeZoneMenu::Free(){
    unpost_menu(m_Menu);
    for (int i = 0; i < m_Size; ++i){
      free_item(m_ItemsRaw[i]);
    }
    free_menu(m_Menu);
  }
 
}
