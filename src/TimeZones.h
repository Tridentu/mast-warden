

#include <mast_tk/MenuUtils.h>
#include <chrono>
#include <string>

namespace MastWarden {

  
  class TimeZoneMenu : public MastTDE::Menu {
  public:
    TimeZoneMenu();
    virtual ~TimeZoneMenu(){
      Free();
    }
    virtual MENU* GetRawMenu() override;
    virtual void CompileMenu() override;
    virtual const char** GetItemsArray() override;
    virtual int GetFirstColor() override;
    virtual int GetSecondColor() override;
    virtual void ProcessInput(int key) override;
    virtual void OnMenuChoose(int choice) override;
    virtual void Post() override;
    virtual void Construct() override;
    virtual MastTDE::MenuState&  GetState() override;
    virtual void Free() override;
    virtual void SaveChoice(int choice) override;
  private:
    MENU* m_Menu;
    ITEM** m_ItemsRaw;
    MastTDE::MenuItemVector m_Titles;
    MastTDE::MenuItemVector m_ZoneNames;
    MastTDE::MenuState m_State;
    int m_Choice = 0;
    int m_Size;
    std::string m_choiceString;
  };
} 
