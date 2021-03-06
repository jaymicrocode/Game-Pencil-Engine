/*
gpe_editor_state.cpp
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

#include "gpe_editor_state.h"

GPE_Editor_State::GPE_Editor_State(std::string sName)
{
    if( (int)sName.size() > 0)
    {
        stateName = sName;
    }
    else
    {
        stateName = "GPE_Editor";
    }
}

GPE_Editor_State::~GPE_Editor_State()
{
    //Free the surfaces
    if( background !=NULL )
    {
        delete background;
        background = NULL;
    }
    if( message !=NULL )
    {
        delete message;
        message = NULL;
    }
    if( editor_gui_main!=NULL)
    {
        delete editor_gui_main;
        editor_gui_main = NULL;
    }
}

void GPE_Editor_State::process_input()
{
    if( gpe::input->check_mouse_released( mb_anybutton ) )
    {
        GPE_main_HIGHLIGHTER->clear_suggestions();
    }
    gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::arrow) );
    //if( renderer_main->window_changed() )
    {
        //updates toolbars and whatnots info
        gpe::camera_reset();
        mainToolBar->set_width(gpe::screen_width);
        //mainButtonBar->set_width(gpe::screen_width);
        mainButtonBar->set_coords( 0,32 );
        if( main_EDITOR_SETTINGS!=NULL)
        {
            if( main_EDITOR_SETTINGS->ideButtonBarSize!=NULL)
            {
                int foundButtonSize =  main_EDITOR_SETTINGS->ideButtonBarSize->get_selected_value();
                mainButtonBar->set_height( foundButtonSize );
                if( main_EDITOR_SETTINGS->ideButtonBarAlignment !=NULL && foundButtonSize > 0)
                {
                    int foundButtonBarAlignment = main_EDITOR_SETTINGS->ideButtonBarAlignment->get_selected_value();
                    if( foundButtonBarAlignment==1)
                    {
                        mainButtonBar->set_coords( (gpe::screen_width-mainButtonBar->get_width() )/2, 32 );
                    }
                    else if( foundButtonBarAlignment==2)
                    {
                        mainButtonBar->set_coords( (gpe::screen_width-mainButtonBar->get_width() ), 32 );
                    }
                    else
                    {
                        mainButtonBar->set_coords( 0, 32 );
                    }
                }
                else
                {
                    mainButtonBar->set_coords( 0, 24 );
                }
            }
        }

        GPE_main_Statusbar->set_coords(0,gpe::screen_height-24 );
        GPE_main_Statusbar->set_height( 24 );
        GPE_main_Statusbar->set_width(gpe::screen_width);

        if( GPE_DOCK!=NULL )
        {
            GPE_DOCK->set_coords( 0,mainButtonBar->get_y2pos() + GENERAL_GPE_GUI_PADDING);
            GPE_DOCK->set_width( gpe::screen_width );
            GPE_DOCK->set_height( gpe::screen_height-GPE_DOCK->get_ypos() - GPE_main_Statusbar->get_height() );
        }
    }
}

void GPE_Editor_State::apply_logic()
{
    editor_gui_main->apply_logic();

    if( !mainToolBar->is_open() && !main_CONTEXT_MENU->is_open() )
    {
        if( mainButtonBar!=NULL )
        {
            mainButtonBar->process_self();
            if( mainButtonBar->selectedOption>= 0 )
            {
                switch( mainButtonBar->selectedOption)
                {
                case 1:
                    editor_gui_main->launch_new_project();
                    break;
                case 2:
                    editor_gui_main->open_new_project();
                    break;

                case 3:
                    editor_gui_main->save_current_project();
                    break;

                case 4:
                    editor_gui_main->save_all_projects();
                    break;

                case 5:
                    if( current_project!=NULL && current_project->RESC_project_SETTINGS!=NULL )
                    {
                        GPE_main_TabManager->add_new_tab(current_project->RESC_project_SETTINGS->get_held_resource() );
                        projectPropertiesResource * tProjectProps = (projectPropertiesResource *)current_project->RESC_project_SETTINGS->get_held_resource();
                        tProjectProps->projectSettingsBar->set_selected_option("Platforms");
                    }
                    break;

                case 6:
                    if( current_project!=NULL && current_project->RESC_project_SETTINGS!=NULL )
                    {
                        current_project->export_and_play_native();
                    }
                    break;
                default:

                    break;
                }
            }
        }
    }

    if( seeked_project_name != project_current_name )
    {
        set_current_gpe_project_from_name( seeked_project_name );
    }
    /*
    //language tooltip
    //place elsewhere once language support is added
    if( point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,gpe::screen_width-32,0,gpe::screen_width,32) )
    {
        main_OVERLAY->update_tooltip(gpe::input->mouse_position_x,gpe::input->mouse_position_y+GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING,"Language Settings: American English! USA! ",gpe::fa_right);
    }
    */
}

void GPE_Editor_State::clean_up()
{

}

void GPE_Editor_State::end_state()
{

}

void GPE_Editor_State::render()
{
    uint64_t sTicks, eTicks;
    sTicks = gpe::time_keeper->get_ticks();
    theme_main->render_background(NULL, NULL);
    eTicks = gpe::time_keeper->get_ticks();
    gpe::error_log->log_ms_action("theme_main->render_background()",eTicks - sTicks,10 );

    sTicks = gpe::time_keeper->get_ticks();
    if( mainButtonBar!=NULL  )
    {
       mainButtonBar->render_self( NULL, NULL);
    }
    eTicks = gpe::time_keeper->get_ticks();
    gpe::error_log->log_ms_action( "mainButtonBar->render_self)",eTicks - sTicks,10 );

    sTicks = gpe::time_keeper->get_ticks();
    if( editor_gui_main!=NULL )
    {
        editor_gui_main->render_foreground_engine();
    }
    eTicks = gpe::time_keeper->get_ticks();
    gpe::error_log->log_ms_action("editor_gui_main->render_foreground_engine()",eTicks - sTicks,10 );

}

void GPE_Editor_State::start_state()
{
    GPE_main_Logs->log_general_line("Starting Game Pencil Engine Version "+ stg_ex::float_to_string( gpe::version_number_total )+"...");

    gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::arrow) );
    //Gets SDL Version
    SDL_compiled_version.major = 0;
    SDL_VERSION(&SDL_compiled_version);
    SDL_VersionText = "SDL Version: "+ stg_ex::int_to_string(SDL_compiled_version.major)+"."+ stg_ex::int_to_string(SDL_compiled_version.minor)+"."+ stg_ex::int_to_string(SDL_compiled_version.patch);
    int animationDataLabelWidth =  0;
    int hh=0; // the 8th position is the widest

    mainToolBar = editor_gui_main->init_toolbar();

    gpe::animaton2d * mainExportOptionsanimation = paw_gui_rsm->animation_add("guiExportOptions", gpe::app_directory_name+"resources/gfx/animations/main_export_options_icons.png",12,true,0,0,false);
    mainButtonBar = new GPE_ToolIconButtonBar(24);
    mainButtonBar->set_coords( 0, mainToolBar->get_height() );
    mainButtonBar->widthAutoResizes = true;
    mainButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file.png","New Project",1,true );
    mainButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder.png","Open Project",2,true );
    mainButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/save.png","Save Project",3,false );
    mainButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/book.png","Save All Projects",4,true );
    mainButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cog.png","Build Project",5,false);
    mainButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png","Build & PLAY Project",6,false);

    GPE_PopUpMenu_Option * newOption = NULL;
    GPE_PopUpMenu_Option * newOptionLayer2 = NULL;
    GPE_PopUpMenu_Option * newOptionLayer3 = NULL;
    newOption = mainToolBar->add_menu_option("File");
    newOption->add_menu_option("New Project",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file.png"),-1,NULL,false,true,false,kb_ctrl,kb_n );
    newOption->add_menu_option("Open Project",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder.png"),-1,NULL,false,true,false,kb_ctrl,kb_o );
    newOption->add_menu_option("Browse Projects",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/binoculars.png") );
    newOptionLayer2 = main_TOOLBAR_RECENT_PROJECTS = newOption->add_menu_option("Recent Projects",-1,NULL,-1,NULL,true);
    editor_gui_main->update_recent_project_list(false);
    //Adds the recent projects to the list

    //Adds the recent files to the list
    newOption->add_menu_option("Save Project",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/save.png"),-1,NULL,false,true,false,kb_ctrl,kb_s  );
    newOption->add_menu_option("Save All Projects",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/save.png"),-1,NULL,false,true,false,kb_ctrl,kb_s,kb_shift );
    /*
    newOption->add_menu_option("Export",-1,-1,NULL,false);
    newOption->add_menu_option("Import Resources");
    newOption->add_menu_option("Export Resources");
    newOption->add_menu_option("Properties",-1,5,NULL,true);
    */
    newOption->add_menu_option("Quit Editor",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/truck.png"),76,NULL,false,true,false,kb_ctrl,kb_q);


    /*
    newOption = mainToolBar->add_menu_option("Edit");
    newOption->add_menu_option("Undo",-1,-1,NULL,false);
    newOption->add_menu_option("Redo",-1,-1,NULL,false);
    newOption->add_menu_option("Cut",-1,-1,NULL,false);
    newOption->add_menu_option("Copy",-1,-1,NULL,false,false);
    newOption->add_menu_option("Paste",-1,-1,NULL,false);
    */

    newOption = mainToolBar->add_menu_option("View");
    //newOption->add_menu_option("Reset View",-1,-1,NULL,false);

    newOption->add_menu_option("Toggle Fullscreen Mode",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/crop.png"),-1,NULL,false,true,false, kb_f11);
    if( GPE_DOCK!=NULL)
    {
        newOption->add_option( GPE_DOCK->toolbarOptonsHolder );
    }

    newOptionLayer2 = newOption->add_menu_option("Text Area Settings",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cogs.png"),-1,NULL,false,false);
    newOptionLayer2->add_menu_option("Toggle Line Count",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);
    newOptionLayer2->add_menu_option("Toggle Syntax Highlighting",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);
    //newOptionLayer2->add_menu_option("",-1,-1,NULL,false);
    newOption->add_menu_option("Start Page",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cube.png"),-1,NULL,false);
    newOption->add_menu_option("Tip of the Day",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/info.png"),-1,NULL,false);

    //newOption = mainToolBar->add_menu_option("Tools");
    //newOption = mainToolBar->add_menu_option("Resources");

    newOption = mainToolBar->add_menu_option("Search");
    newOption->add_menu_option("Find...",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_f);
    newOption->add_menu_option("Find in files...",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_f,kb_shift);
    newOption->add_menu_option("Replace...",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_r);
    newOption->add_menu_option("Replace in files....",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_r,kb_shift);
    newOption->add_menu_option("Goto Line...",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_g);
    newOption->add_menu_option("Find Project Resource...",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/search.png"),-1,NULL,false);

    newOption = mainToolBar->add_menu_option("Import");
    for( int tempResType = gpe::resource_type_animation; tempResType <= gpe::resource_type_font; tempResType++ )
    {
        if( (int) gpe::resource_type_names[tempResType].size() > 2 )
            newOption->add_menu_option("Import "+ gpe::resource_type_names[tempResType]);
    }

    newOption = mainToolBar->add_menu_option("Build");
    // HTML5/ Web platforms

    newOption->add_menu_option("Build HTML5 GAME",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/html5.png"),-1,NULL,true,true,false,kb_ctrl,kb_f5);
    newOption->add_menu_option("RUN HTML5 GAME",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play-circle.png"),-1,NULL,true,true,false,kb_f8);
    newOption->add_menu_option("Build HTML5 GAME & RUN",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png"),-1,NULL,true,true,false,kb_f5);
    //desktop / laptop platforms
    if( gpe::system_found_os == gpe::system_os_windows)
    {
        newOption->add_menu_option("Build WINDOWS GAME",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/windows.png"),-1,NULL,false,true );
        newOption->add_menu_option("Build OSX GAME",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/apple.png"),-1,NULL,false,true );
        newOption->add_menu_option("Build LINUX GAME",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/linux.png"),-1,NULL,true,true );
    }
    else if( gpe::system_found_os== gpe::system_os_mac )
    {
        newOption->add_menu_option("Build OSX GAME",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/apple.png"),-1,NULL,false,true );
        newOption->add_menu_option("Build LINUX GAME",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/linux.png"),-1,NULL,true,true );
        newOption->add_menu_option("Build WINDOWS GAME",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/windows.png"),-1,NULL,false,true );
    }
    else
    {
        newOption->add_menu_option("ExpBuildort LINUX GAME",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/linux.png"),-1,NULL,true,true );
        newOption->add_menu_option("Build WINDOWS GAME",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/windows.png"),-1,NULL,false,true );
        newOption->add_menu_option("Build OSX GAME",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/apple.png"),-1,NULL,false,true );
    }
    /*
    newOption->add_menu_option("Compile Current Tab",-1);
    newOption->add_menu_option("OUYA- P2",-1,5,mainExportOptionsanimation,false,false);
    */

    //console platforms
    /*
    newOptionLayer2 =newOption->add_menu_option("Build Nintendo Switch GAME",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/animations/nintendo_switch_logo.png"),-1,NULL,true,true);
    newOptionLayer2->renderWhite = true;*/
    /*
    newOption->add_menu_option("Build Xbox One GAME",-1,3,mainExportOptionsanimation,true,false);
    */

    /*
    //handheld consoles
    newOptionLayer2 = newOption->add_menu_option("Handheld Platforms (Coming Soon)",-1);
    newOptionLayer2->add_menu_option("GCW-Zero- P3",-1,8,mainExportOptionsanimation,false);
    newOptionLayer2->add_menu_option("Nintendo 3DS & New 3DS- P3",-1,9,mainExportOptionsanimation,false,false);
    newOptionLayer2->add_menu_option("Open Pandora- P3",-1,10,mainExportOptionsanimation,false,false);
    newOptionLayer2->add_menu_option("Playstation Vita- P3",-1,11,mainExportOptionsanimation,false,false);

    //mobile platforms
    newOptionLayer2 = newOption->add_menu_option("Mobile Platforms(Coming Soon)",-1);
    newOptionLayer2->add_menu_option("Android - P4",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("Blackberry - P5",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("iPhones - P5",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("Tizen Phones - P6",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("Ubuntu Phones - P6",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("Windows Phones - P4",-1,11,NULL,false,false);
    */

    newOptionLayer2 = newOption->add_menu_option("Clean Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [HTML5] Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [Windows] Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [Linux] Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [OSX] Build Folder",-1);
    //newOptionLayer2->add_menu_option("Clean [HTML5] Build Folder",-1,-1,mainExportOptionsanimation,false);

    newOption = mainToolBar->add_menu_option("Tools");
    newOption->add_menu_option("Game Controller Tester",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/gamecontroller.png"),-1,NULL,false,true,false);
    newOption->add_menu_option("Extra Tools",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/briefcase.png"),-1,NULL,false,true,false);

    newOption = mainToolBar->add_menu_option("Settings");
    newOption->add_menu_option("User Settings",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cog.png"),-1, NULL,false,true,false,kb_f2);
    newOption->add_menu_option("JS Compiler Settings",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/code.png"),-1,NULL,false,true,false);
    //C++ Settings
    newOption->add_menu_option("C++ Builder Settings",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/code.png"),-1,NULL,false,true,false);

    newOption = mainToolBar->add_menu_option("Help" );
    //newOption->add_menu_option("Online Documentation",-1,17,mainMenuanimation,false);
    //newOption->add_menu_option("Tips",-1,20,mainMenuanimation,false);
    //newOption->add_menu_option("Forums",-1);
    newOption->add_menu_option("General Help",-1,NULL,-1,NULL,false,true,false,kb_f1);


    newOption->add_menu_option("Online Documentation",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/book.png"),-1,NULL,false);
    newOption->add_menu_option("Online Forums",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/group.png"),-1,NULL,false);
    newOption->add_menu_option("Community Chat",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/comments.png"),-1,NULL,false);
    newOption->add_menu_option("Tip of the Day",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/infO.png"),-1,NULL,false);
    newOption->add_menu_option("Report Issue",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/bug.png"),-1,NULL,false);
    newOption->add_menu_option("Check Updates",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/question-circle.png"),-1,NULL,false);
    newOption->add_menu_option("License",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-text.png"),-1,NULL,false);
    newOption->add_menu_option("About Game Pencil Engine",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/pencil.png"),-1,NULL,false,true,false,kb_ctrl,kb_b);
    //newOption->add_menu_option("Licenses",-1,27,mainMenuanimation,false);
    GPE_main_TabManager->set_coords(-1,mainButtonBar->get_y2pos()+GENERAL_GPE_GUI_PADDING );
    editor_gui_main->init_resourcebar();
    //main resource bar elements

    country_language_image = gpe::rph->get_new_texture();
    if(country_language_image!=NULL)
    {
        //country_language_image->load_new_texture( renderer_main, gpe::app_directory_name+"resources/gfx/flags/24/us.png",-1,true);
    }

    //Load the background
    background = NULL;

    //Render the intro message
    message = NULL;
    if( editor_gui_main!=NULL)
    {
        editor_gui_main->load_settings();
        if( editor_gui_main->showStartPageAtStartUp)
        {
            GPE_main_TabManager->add_new_tab(main_START_PAGE);
            main_START_PAGE->set_name("Start Page");
        }

        if( editor_gui_main->showTipsAtStartUp)
        {
            gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::arrow) );
            gpe::input->handle_input(true);
            process_input();
            gpe::input->reset_all_input();
            apply_logic();
            render();
            if( editor_gui_main->showTipsAtStartUp)
            {
                GPE_Show_Tip_Of_Day();
            }
        }


        /*
        GPE_ProjectFolder * newProj = new GPE_ProjectFolder("New Project","here/","newproject.tgif");
        editor_gui_main->gpeProjects.push_back(newProj);
        GPE_ProjectFolder * newExampleProj = new GPE_ProjectFolder("Examples","here/","examples.tgif");
        editor_gui_main->gpeProjects.push_back(newExampleProj);
        */
    }
}

bool init_gpe_editor( int argc, char* args[] )
{
    gpe::error_log->report("Starting GPE Editor...");
    init_gpe_editor_globals();

    gpe::error_log->report( "Starting GUI..." );
    if( !PAW_GUI_START() )
    {
        gpe::error_log->report("Unable to start Paw_GUI LIBRARY!\n");
        return false;
    }
    editor_gui_main = new GPE_Gui_Engine();
    //If everything initialized fine
    GPE_main_TabManager = new GPE_TabManager();

    main_EDITOR_SETTINGS = new gamePencilEditorSettingsResource();
    main_EDITOR_SETTINGS->set_global_rid(-1);
    main_EDITOR_SETTINGS->set_name("User Settings");
    main_EDITOR_SETTINGS->load_resource();

    main_ABOUT_PAGE = new gamePencilAboutPageResource();
    main_ABOUT_PAGE->set_global_rid(-2);
    main_ABOUT_PAGE->set_name("About...");

    main_HELP_PAGE = new gamePencilHelpPageResource();
    main_HELP_PAGE->set_global_rid(-3);
    main_HELP_PAGE->set_name("Help...");


    main_START_PAGE = new gamePencilStartPageResource();
    main_START_PAGE->set_global_rid(-4);
    main_START_PAGE->set_name("Start Page");

    main_EXTRA_TOOLS = new gamePencilExtraTools();
    main_EXTRA_TOOLS->set_global_rid(-5);
    main_EXTRA_TOOLS->set_name("Extra Tools");

    GPE_JS_COMPILER_SETTINGS = new gameJSCompilerSettingsResource();
    GPE_JS_COMPILER_SETTINGS->set_global_rid(-6);
    GPE_JS_COMPILER_SETTINGS->set_name("JS Compiler Settings");
    GPE_JS_COMPILER_SETTINGS->load_resource();


    GPE_CPP_BUILDER_SETTINGS = new gameCPPBuilderSettingsResource();
    GPE_CPP_BUILDER_SETTINGS->set_global_rid(-7);
    GPE_CPP_BUILDER_SETTINGS->set_name("C++ Builder Settings");
    GPE_CPP_BUILDER_SETTINGS->load_resource();

    main_gamepad_tester = new gamePencilgamepadTesterResource();
    main_gamepad_tester->set_global_rid(-8);
    main_gamepad_tester->set_name("Game Controller Tester");

    //GPE_main_TabManager->add_new_tab(main_START_PAGE);

    GPE_main_Statusbar = new GPE_StatusBar();
    GPE_main_Statusbar->set_coords(0,0);
    GPE_main_Statusbar->set_width(gpe::screen_width);
    GPE_main_Statusbar->set_height(24);

    GPE_main_Logs = new GPE_LogManager();
    GPE_main_Logs->set_coords(0,gpe::screen_height-64);
    GPE_main_Logs->set_width(gpe::screen_width);
    GPE_main_Logs->set_height(32);

    GPE_main_HIGHLIGHTER = new GPE_Syntax_Highlighter();
    gpe::error_log->report("Adding Docking System...");
    GPE_DOCK = new gpeEditorDock();
    GPE_DOCK->add_default_panel("Editor",DOCK_TOP_LEFT, true );
    GPE_DOCK->add_default_panel("Inspector",DOCK_BOTTOM_LEFT, true );
    GPE_DOCK->add_default_panel("Resources",DOCK_TOP_RIGHT, true );
    GPE_DOCK->add_default_panel("Tilesheet",DOCK_BOTTOM_RIGHT, true );
    GPE_DOCK->add_default_panel("Grid Settings",DOCK_BOTTOM_RIGHT, true );
    gpe::error_log->report("Docking System added...");

    spm = new GPE_SceneEditorHelper();

    gpe::game_runtime->loading_data = false;
    GPE_Editor_State * newEditor = new GPE_Editor_State( "gpe_editor");
    gpe::game_runtime->state_add( newEditor );
    gpe::game_runtime->state_set( newEditor->get_state_name() );

    //Opens projects based on args[0] being the application name.
    if( argc > 1)
    {
        for( int iArgc = 1; iArgc <argc; iArgc++ )
        {
            editor_gui_main->open_project( args[iArgc] );
            gpe::error_log->report( args[iArgc] );
        }
    }
    else if( main_EDITOR_SETTINGS!=NULL && main_EDITOR_SETTINGS->launchLastProjectBox!=NULL )
    {
        if( main_EDITOR_SETTINGS->launchLastProjectBox->is_clicked() )
        {
            int iRPList = 0;
            int maxRPList = editor_gui_main->get_recent_project_list_size();
            if( maxRPList > 0 )
            {
                if( sff_ex::file_exists(editor_gui_main->get_recent_project_name(0) ) )
                {
                    //editor_gui_main->open_project( editor_gui_main->get_recent_project_name(0) );
                }
            }
        }
    }

    if( main_EDITOR_SETTINGS!=NULL && main_EDITOR_SETTINGS->ideSettingsFPSRate!=NULL )
    {
        gpe::time_keeper->set_fps( main_EDITOR_SETTINGS->ideSettingsFPSRate->get_selected_value() );
    }
    gpe::error_log->report("Loading in dock settings..");
    GPE_DOCK->load_dock_setings( "dock_settings.gpf");

    return true;
}


bool quit_gpe_editor()
{
    if( GPE_main_TabManager!=NULL)
    {
        gpe::error_log->report("Closing all tabs....");
        GPE_main_TabManager->close_all_tabs();
    }
    if( main_EDITOR_SETTINGS!=NULL)
    {
        gpe::error_log->report("Deleting Settings....");
        main_EDITOR_SETTINGS->save_resource();
        delete main_EDITOR_SETTINGS;
        main_EDITOR_SETTINGS = NULL;
    }
    if( main_ABOUT_PAGE!=NULL)
    {
        gpe::error_log->report("Deleting about page....");
        delete main_ABOUT_PAGE;
        main_ABOUT_PAGE = NULL;
    }
    if( main_HELP_PAGE!=NULL)
    {
        gpe::error_log->report("Deleting help page....");
        delete main_HELP_PAGE;
        main_HELP_PAGE = NULL;
    }
    if( main_START_PAGE!=NULL)
    {
        gpe::error_log->report("Deleting start page....");
        delete main_START_PAGE;
        main_START_PAGE = NULL;
    }
    if( main_EXTRA_TOOLS!=NULL)
    {
        gpe::error_log->report("Deleting extra tools....");
        delete main_EXTRA_TOOLS;
        main_EXTRA_TOOLS = NULL;
    }
    if( main_CONTEXT_MENU!=NULL)
    {
        gpe::error_log->report("Deleting context menu....");
        delete main_CONTEXT_MENU;
        main_CONTEXT_MENU = NULL;
    }
    if( main_TOOLBAR_RECENT_PROJECTS!=NULL)
    {
        gpe::error_log->report("Deleting toolbar recent projects....");
        delete main_TOOLBAR_RECENT_PROJECTS;
        main_TOOLBAR_RECENT_PROJECTS = NULL;
    }

    if( GPE_DOCK!=NULL )
    {
        delete GPE_DOCK;
        GPE_DOCK = NULL;
    }

    if( editor_gui_main!=NULL)
    {
        gpe::error_log->report("Deleting gui....");
        editor_gui_main->save_settings();
        delete editor_gui_main;
        editor_gui_main = NULL;
    }
}
