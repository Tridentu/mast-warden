
#pragma once

#include <vector>
#include <menu.h>

namespace MastTDE {

  typedef std::vector<const char*> MenuItemVector;
  
  struct MenuState {
    bool Off = false;
    int Key = 0;
    
  };

  class Menu {
  public:
    virtual ~Menu(){};
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
};
