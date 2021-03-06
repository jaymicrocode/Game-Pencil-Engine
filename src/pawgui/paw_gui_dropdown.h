/*
paw_gui_dropdown.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 Game Pencil Engine contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/
#ifndef PAW_GUI_DROPDOWN_H
#define PAW_GUI_DROPDOWN_H

#include "paw_gui_base.h"
#include "paw_gui_parsers.h"
#include "paw_gui_context.h"
#include "../gpe/gpe_parser.h"

class GPE_DropDown_Menu: public GPE_GeneralGuiElement
{
private:
    int widestOptionSpace;
    int maxOptionsAllowed;
    std::string dropdownName;
    int opId;
    bool isSelectable;
    int selectedId;
    std::string selectedName;
    std::string selectedTag;
    float selectedValue;
    gpe::key_pair * selectedPair;
    gpe::key_pair * dropDownParentPair;
    bool isOpen;
    bool justActivated;
    bool showJustOptions;
public:
    bool subMenuIsOpen;
    gpe::color * barColor;
    GPE_DropDown_Menu( std::string name = "", bool justOptions = false);
    ~GPE_DropDown_Menu();

    std::string get_data();
    void load_data(std::string dataString);
    void remove_data(std::string dataString);
    bool add_to_context_menu( GPE_PopUpMenu_Option *  cLevel, gpe::key_pair * cKey );
    gpe::key_pair * add_menu_option(std::string optionName, std::string optionSubStr= "",float optionValue = 0, bool selectOption = false);
    void clear_dropdown();
    gpe::key_pair *  find_option_id(  int pairId );
    gpe::key_pair *  find_option_valie(  float pairValue );
    gpe::key_pair *  find_selected_pair( gpe::key_pair *  pairIn, std::string pairName, int pairId = -1);
    gpe::key_pair *  find_selected_pair_sub( gpe::key_pair *  pairIn, std::string pairSubString );
    std::string get_menu_option(int atNumb);
    int get_menu_size();
    std::string get_plain_string();
    int get_selected_id();
    std::string get_selected_tag();
    std::string get_selected_name();

    float get_selected_value();

    std::string get_tag_from( std::string tagName, int tagId = -1);
    bool just_activated();
    void remove_option(std::string optionToRemove);
    void reset_suboptions();
    void set_id(int new_id);
    void set_option_named( std::string newselectedOptionName );
    void set_option_subvalue( std::string newselectedOptionName );
    void set_option_value(float sValue);
    void show_just_options(bool justOptions);
    void process_self( gpe::shape_rect * viewedSpace=NULL, gpe::shape_rect *cam=NULL);
    void render_self( gpe::shape_rect * viewedSpace=NULL, gpe::shape_rect * cam = NULL);
    void set_name(std::string new_name);
};

#endif
