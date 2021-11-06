
#pragma once

#include <vector>
#include <functional>

#include <string>
#include <menu.h>

namespace MastTDE {

  typedef std::vector<const char*> MenuItemVector;
  
  struct MenuState {
    bool Off = false;
    int Key = 0;
    
  };

  class Menu {
  public:
    virtual ~Menu();
    virtual MENU* GetRawMenu() = 0;
    virtual void CompileMenu() = 0;
    virtual const char** GetItemsArray() = 0;
    virtual int GetFirstColor() = 0;
    virtual int GetSecondColor() = 0;
    virtual void ProcessInput(int key) {};
    virtual void OnMenuChoose(int choice) {};
    virtual void Post() = 0;
    virtual void Construct() = 0;
    virtual MenuState& GetState() = 0;
    virtual void Free() = 0;
    virtual int GetChoiceCount() { return 0; };
    virtual void SaveChoice(int choice) {};
  };


  enum class MenuMode {
    Main = 0,
    Secondary = 1
  };

  class MenuContainer {
  public:
    virtual ~MenuContainer();
    virtual MENU* GetRawMenu() = 0;
    virtual void CompileMenu() = 0;
    virtual const char** GetItemsArray() = 0;
    virtual void ProcessInput(int key) {};
    virtual void OnMenuChoose(int choice) {};
    virtual void Post() = 0;
    virtual void Construct() = 0;
    virtual MenuState& GetState() = 0;
    virtual void Free() = 0;
    virtual void DrawMenuTitle() = 0;
    virtual void AddMenu(std::string name, std::function<void()> processor) = 0;
  };
};
