
#include <menu.h>
#include <string>
#include <functional>
#include <map>
#include "MenuUtils.h"

namespace MastWarden {

  class MainMenu : public MastTDE::MenuContainer {
  public:
    MainMenu();
    ~MainMenu();
    virtual MENU* GetRawMenu() override;
    virtual void CompileMenu() override;
    virtual const char** GetItemsArray() override;                                          
    virtual void ProcessInput(int key) override;                                                           
    virtual void OnMenuChoose(int choice) override;                                                        
    virtual void Post() override;                                                                         
    virtual void Construct() override;                                                                    
    virtual MastTDE::MenuState& GetState() override;                                                               
    virtual void Free() override;                                                                                                                                                                       
    virtual void DrawMenuTitle() override;
    virtual void AddMenu(std::string name, std::function<void()> processor) override;
  public:
     void RefreshMenu();  
     const bool IsRunning();
  private:
    std::map<std::string, std::function<void()>> m_Menus;
    MENU* m_Menu;
    int  m_Size;
    MastTDE::MenuItemVector m_Keys;
    ITEM** m_Items;
    int m_Choice = 0;
    MastTDE::MenuState m_State;
    int  m_CurrentMenu;
    MastTDE::MenuMode m_Mode = MastTDE::MenuMode::Main;
  };
  
}
