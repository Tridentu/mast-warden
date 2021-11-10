#pragma once 
#include <mast_tk/MenuUtils.h>
#include <menu.h>
namespace MastWarden {
  class KeymapMenu : public MastTDE::Menu {
  public:
    KeymapMenu();
    virtual ~KeymapMenu();
    virtual MENU* GetRawMenu() override;
    virtual void CompileMenu() override;
    virtual const char** GetItemsArray() override;
    virtual int GetFirstColor() override;
    virtual int GetSecondColor() override;
    virtual void ProcessInput(int key) override;
    virtual void OnMenuChoose(int choice) override;
    virtual void Post() override;
    virtual void Construct() override;
    virtual MastTDE::MenuState& GetState() override;
    virtual void Free() override;
    virtual void SaveChoice(int choice) override;
  private:
    MENU* m_Menu;
    ITEM **m_Items;
    MastTDE::MenuItemVector m_Titles;
    MastTDE::MenuItemVector m_Layouts;
    MastTDE::MenuState m_State;
    int m_Choice = 0;
    int m_Size;
    std::string m_ChoiceString;
  };
}
