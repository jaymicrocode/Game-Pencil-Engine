/*
gpe_editor_start_page.cpp
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

#include "gpe_editor_start_page.h"
#include "gpe_editor_helppage.h"
#include "gpe_editor_settings.h"

gamePencilStartPageResource * main_START_PAGE = NULL;


quickProjectReader::quickProjectReader()
{
    detectedName = "";
    detectedIcon = "";
    detectedLocation = "";
    detectedSaveTime = "";
    detectedVersion = "";
    detectedLanguage = "JS";
}

quickProjectReader::~quickProjectReader()
{

}

bool quickProjectReader::review_project(std::string projectFileName)
{
    detectedName = "";
    detectedIcon = "";
    detectedLanguage = "?";
    detectedLocation = "";
    detectedSaveTime = "";
    detectedVersion = "Unknown";
    if( sff_ex::file_exists(projectFileName) )
    {
        std::ifstream newprofileFile( projectFileName.c_str() );

        //If the level file could be loaded
        if( !newprofileFile.fail() )
        {
            //makes sure the file is open
            if (newprofileFile.is_open())
            {
                detectedName = projectFileName;
                detectedLocation = projectFileName;

                std::string firstChar="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string currLine="";
                std::string newresource_name= "";
                std::string tempNewresource_name = "";
                std::string tempFoundIcon;
                int equalPos=-1;
                int endBracketPos = -1;
                int foundResGlobalId = -1;
                int tempStrToInt = -1;
                int iItr = 0;
                int iLang = 0;
                //makes sure the file is in good condition and the version is still unloaded
                while ( newprofileFile.good() )
                {
                    getline (newprofileFile,currLine); //gets the next line of the file
                    currLine = stg_ex::trim_left_inplace(currLine);
                    currLine = stg_ex::trim_right_inplace(currLine);

                    if(!currLine.empty() )
                    {
                        // skips comment lines
                        if( currLine[0]!= '#' && currLine[0]!='/'  )
                        {
                            //searches for an equal character and parses through the variable
                            equalPos=currLine.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                keyString = currLine.substr(0,equalPos);
                                valString = currLine.substr(equalPos+1,currLine.length());
                                //finds the version of the game level file
                                if (keyString== "Version")
                                {
                                    detectedVersion = valString;
                                    if( detectedVersion=="1")
                                    {
                                        detectedVersion = "1.00";
                                    }
                                    else if ( detectedVersion=="2")
                                    {
                                        detectedVersion = "2.00";
                                    }
                                    else if ( detectedVersion=="3")
                                    {
                                        detectedVersion = "3.00";
                                    }
                                }
                                else if( keyString=="Count" || keyString=="ResourcesCount")
                                {
                                    resourcesInProject = stg_ex::string_to_int(valString);
                                }
                                else if(keyString=="ProjectName" || keyString=="Name")
                                {
                                    detectedName = valString;
                                }
                                else if(keyString=="ProjectLanguage" || keyString=="ProgrammingLanguage" || keyString=="CodingLanguage")
                                {
                                    detectedLanguage = valString;
                                }
                                else if( keyString=="ProjectIcon")
                                {
                                    if( sff_ex::path_exists( stg_ex::get_file_noext(projectFileName) ) )
                                    {
                                        tempFoundIcon = stg_ex::get_file_noext(projectFileName)+"/"+valString;
                                        if( sff_ex::file_exists( tempFoundIcon )  && stg_ex::file_is_image(tempFoundIcon) )
                                        {
                                            detectedIcon = tempFoundIcon;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                newprofileFile.close();
                if( ( (int)tempFoundIcon.size()==0) && sff_ex::path_exists( stg_ex::get_file_noext(projectFileName) ) )
                {
                    if( sff_ex::file_exists( stg_ex::get_file_noext(projectFileName)+"/icon.png" ) )
                    {
                        detectedIcon = stg_ex::get_file_noext(projectFileName)+"/icon.png";
                    }
                }
                return true;
            }
        }
    }
    return false;
}

gamePencilStartPageResource::gamePencilStartPageResource( )
{
    headerViewedSpace = new gpe::shape_rect();
    sidePanelRect = new gpe::shape_rect();
    subViewedSpace = new gpe::shape_rect();

    resourceFileName = "";
    resource_name = "Start Page";
    resourceType = -2;
    subResouceId = -2;

    projectsCategoryTabs = new GPE_TabBar();
    projectsCategoryTabs->set_coords(16,16);
    projectsCategoryTabs->add_new_tab("Recent Projects",true);
    projectsCategoryTabs->add_new_tab("Example Projects",false);
    projectsCategoryTabs->add_new_tab("Favorites Folder",false);

    headerSpaceHeight = 192;
    headerViewedSpace->x = 0;
    headerViewedSpace->y = 0;
    headerViewedSpace->w = gpe::screen_width;
    headerViewedSpace->h = headerSpaceHeight;

    subViewedSpace->x = 0;
    subViewedSpace->y = 0;
    subViewedSpace->w = gpe::screen_width;
    subViewedSpace->h = gpe::screen_height;



    headerPageList = new GPE_GuiElementList();
    headerPageList->set_horizontal_align(gpe::fa_left);
    headerPageList->barXMargin = GENERAL_GPE_GUI_PADDING;
    headerPageList->barYMargin = 0;
    headerPageList->barYPadding = GENERAL_GPE_GUI_PADDING/2;
    headerPageList->hideXScroll = true;
    headerPageList->hideYScroll = true;
    //Added as of Version 1.1.3 [ END ]
    processedFirst = false;
    resourceFileName = "";

    sidePanelList = new GPE_GuiElementList();
    sidePanelSelectBox = new GPE_SelectBoxBasic();

    startPageList = new GPE_GuiElementList();
    startPageList->set_horizontal_align(gpe::fa_center);
    startPageList->barXPadding = GENERAL_GPE_GUI_PADDING;
    startPageList->barXMargin = 128;
    labelEditorShortcuts = new GPE_Label_Title("Editor Shortcuts","Editor Shortcuts");

    /*/Patreon Stuff */
    labelPatreon = new GPE_Label_Title("Support Our Patreon","");
    labelPatreonLogo = new GPE_Label_Image("patreon_image",gpe::app_directory_name+"resources/gfx/textures/become_a_patron_button.png", "Support our Patron","https://www.patreon.com/pawbyte?ty=h");
    labelPatreonLogo->webUrl = "https://www.patreon.com/pawbyte?ty=h";

    labelPatreonDescription = new GPE_Label_Paragraph("","By donating a small amount you can keep this engine going!","");
    urlPatreonPage = new GPE_TextURL("Fund future exports and features","A few dollars can go a long way","https://www.patreon.com/pawbyte?ty=h");
    urlPatreonPage->hasLineBreak = true;

    labelHelpfulLinks = new GPE_Label_Title("Helpful Links","Helpful Links");

    GPE_TextURL * tTempUrl = NULL;
    tTempUrl = new GPE_TextURL("Game Pencil Home Page","","http://gamepencil.pawbyte.com");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);
    tTempUrl = new GPE_TextURL("Documentation","","http://docs.pawbyte.com");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    tTempUrl = new GPE_TextURL("Official Forums","","http://forums.pawbyte.com");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    tTempUrl = new GPE_TextURL("itch.io Marketplace","","https://itch.io/game-assets/tag-gamepencil");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    tTempUrl = new GPE_TextURL("Discord Chat","","https://discord.gg/bua3rDW");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    tTempUrl = new GPE_TextURL("Official News","","http://gamepencil.pawbyte.com/category/news/");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    tTempUrl = new GPE_TextURL("GitHub","","https://github.com/pawbyte/Game-Pencil-Engine");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    tTempUrl = new GPE_TextURL("PawByte","","http://www.pawbyte.com");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    forumPageButton = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/comments.png","Forums","",-1);
    forumPageButton->set_website("http://community.pawbyte.com");
    forumPageButton->showBackground = false;

    helpPageButton = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/question.png","Help","",-1);
    helpPageButton->showBackground = false;


    tipOfTheDayButton = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/info.png","Tip of the Day","",-1);
    tipOfTheDayButton->showBackground = false;

    //Community
    communityLinksTitle = new GPE_Label_Title("Community","Community links to stay connected");
    GPE_ToolIconButton * tSocialButton = NULL;

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/facebook.png","Facebook" );
    tSocialButton->set_website("https://facebook.com/pawbyte");
    add_communitylink(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/reddit.png","Reddit" );
    tSocialButton->set_website("https://reddit.com/r/gamepencil");
    add_communitylink(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/github.png","GitHub repo");
    tSocialButton->set_website("https://github.com/pawbyte/Game-Pencil-Engine");
    add_communitylink(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/comments.png","Discord Chat Group" );
    tSocialButton->set_website("https://discord.gg/bua3rDW");
    add_communitylink(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/twitter.png","Twitter" );
    tSocialButton->set_website("https://twitter.com/pawbyte");
    add_communitylink(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/youtube.png","YouTube" );
    tSocialButton->set_website("https://www.youtube.com/user/PawByte");
    add_communitylink(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/gamecontroller.png","IndieDB" );
    tSocialButton->set_website("http://www.indiedb.com/engines/game-pencil-engine");
    add_communitylink(tSocialButton);

    std::string copyRightInfoString = "";

    if( gpe::version_number_update==0)
    {
        copyRightInfoString = stg_ex::float_to_string( gpe::version_number_total )+".0.0";
    }
    else
    {
        copyRightInfoString = stg_ex::float_to_string( gpe::version_number_total );
    }

    if( release_current_mode==release_type_rc)
    {
        copyRightInfoString+="   - Release Candidate";
    }
    else if( release_current_mode==release_type_beta)
    {
        copyRightInfoString+="   - Beta Version";
    }

    labelCopyrightInfo = new GPE_Label_Text("Game Pencil Engine - Version "+copyRightInfoString+".","");
    labelImgGPELogo = new GPE_Label_Image("gpe_logo",gpe::app_directory_name+"resources/gamepencil_icon_72dpi.png" );
    labelImgGPELogo->set_full_width( true );
    labelImgGPELogo->resizeAspect = 0.5;


    startPageList = new GPE_GuiElementList();
    startPageList->set_horizontal_align(gpe::fa_left);
    startPageList->barXMargin = GENERAL_GPE_GUI_PADDING;
    startPageList->barYMargin = 0;
    startPageList->barYPadding = GENERAL_GPE_GUI_PADDING/2;

    emptyProjectsListLabel = new GPE_Label_Text("No Recent projects","No Recent projects");

    scanProjectFolderButton = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/search.png","Scan Folder","Scan Folder");

    newProjectButton = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file.png","New Project","New Project");

    loadProjectButton = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder-open.png","Load Project","Load Project");

    refreshButton = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/refresh.png","Refresh","Refresh Project Lists");

    quickProjectReader * qpr = new quickProjectReader();
    GPE_VerticalCardButton * tRecentListButton = NULL;
    std::string tempProjectName = "";
    std::string newMultiLine = "";
    int  rProjectListSize = (int)recentProjectsList.size();
    int i = 0;
    gpe::error_log->report("Loading Recent Projects...");
    for( i = 0; i < GPE_MAX_RECENT_project_LIST_SIZE; i++)
    {
        tempProjectName = editor_gui_main->get_recent_project_name(i);
        if( (int)tempProjectName.size() > 0 && sff_ex::file_exists(tempProjectName) )
        {
            if( qpr->review_project(tempProjectName) )
            {
                if( sff_ex::file_exists(qpr->detectedIcon) ==false)
                {
                    qpr->detectedIcon =gpe::app_directory_name+"resources/gamepencil_icon_72dpi.png";
                }
                newMultiLine = qpr->detectedName;
                if((int)qpr->detectedVersion.size() > 0 )
                {
                    newMultiLine+="\nVersion: "+qpr->detectedVersion;
                }
                if((int)qpr->detectedLanguage.size() > 0 )
                {
                    newMultiLine+="\nLanguage: "+qpr->detectedLanguage;
                }
                //newMultiLine+="\nLocation: "+tempProjectName;
                tRecentListButton = new GPE_VerticalCardButton( qpr->detectedIcon,newMultiLine,qpr->detectedLocation);
                recentProjectsList.push_back(tRecentListButton);
            }
        }
    }
    delete qpr;
    qpr = NULL;

    gpe::error_log->report("Loading Favorite Projects Folder...");
    std::string favFolder = "";
    if( main_EDITOR_SETTINGS!=NULL)
    {
        favFolder = main_EDITOR_SETTINGS->projectFolderListLocation;
    }
    projectFolderLocationLabel = new GPE_Label_Text(favFolder,favFolder);
    load_projecs_folder(favFolder);

    processedFirst = true;
    load_example_projects_folder();
    gpe::error_log->report("Loading Completed...");
}

gamePencilStartPageResource::~gamePencilStartPageResource()
{
    gpe::error_log->report("Starting to delete start page...");
    subViewedSpace->x = GENERAL_GPE_GUI_PADDING;
    subViewedSpace->x = GENERAL_GPE_GUI_PADDING;
    subViewedSpace->w = 400;
    subViewedSpace->h = 400;
    resourceType = -2;
    subResouceId = -2;
    parentProjectName = "";
    gpe::error_log->report("Deleting recent project lists...");
    GPE_TextURL * tUrlHolder = NULL;
    int i = 0;

    gpe::error_log->report("Deleting helpful website links..");
    for( i = 0; i <(int)helpfulWebLinks.size(); i++)
    {
        tUrlHolder = helpfulWebLinks[i];
        if( tUrlHolder!=NULL)
        {
            delete tUrlHolder;
            tUrlHolder = NULL;
            helpfulWebLinks[i] = NULL;
        }
    }
    helpfulWebLinks.clear();
    if( startPageList!=NULL)
    {
        delete startPageList;
        startPageList = NULL;
    }
    gpe::error_log->report("Deleting editor shortcuts....");
    if( labelEditorShortcuts!=NULL)
    {
        delete labelEditorShortcuts;
        labelEditorShortcuts = NULL;
    }
    gpe::error_log->report("Deleting helpful links....");
    if( labelHelpfulLinks!=NULL)
    {
        delete labelHelpfulLinks;
        labelHelpfulLinks = NULL;
    }
    gpe::error_log->report("Deleting copyright info....");
    if( labelCopyrightInfo!=NULL)
    {
        delete labelCopyrightInfo;
        labelCopyrightInfo = NULL;
    }

    gpe::error_log->report("Deleting gpe img logo....");
    if( labelImgGPELogo!=NULL)
    {
        delete labelImgGPELogo;
        labelImgGPELogo = NULL;
    }

    gpe::error_log->report("Deleting start page list...");
    if( startPageList!=NULL)
    {
        startPageList->clear_list();
        delete startPageList;
        startPageList = NULL;
    }
    gpe::error_log->report("Deleting project categories....");
    if( projectsCategoryTabs!=NULL)
    {
        delete projectsCategoryTabs;
        projectsCategoryTabs = NULL;
    }
    gpe::error_log->report("Deleting start page buttons....");
    if( projectFolderLocationLabel!=NULL)
    {
        delete projectFolderLocationLabel;
        projectFolderLocationLabel = NULL;
    }
    if( scanProjectFolderButton!=NULL)
    {
        delete scanProjectFolderButton;
        scanProjectFolderButton = NULL;
    }
    if( newProjectButton!=NULL)
    {
        delete newProjectButton;
        newProjectButton = NULL;
    }

    if( loadProjectButton!=NULL)
    {
        delete loadProjectButton;
        loadProjectButton = NULL;
    }
    gpe::error_log->report("Startpage successfully deleted....");
}

void gamePencilStartPageResource::add_communitylink( GPE_ToolIconButton * newLink)
{
    if( newLink!=NULL)
    {
        newLink->scale_object( 2 );
        communityButtons.push_back(newLink);
    }
}

bool gamePencilStartPageResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}

void gamePencilStartPageResource::load_example_projects_folder( )
{
    GPE_VerticalCardButton * tempProjectButton = NULL;

    int i = 0;
    int j = 0;


    for( i = (int)examplesProjectsList.size()-1; i>=0; i--)
    {
        tempProjectButton = examplesProjectsList[i];
        if( tempProjectButton!=NULL)
        {
            delete tempProjectButton;
            tempProjectButton = NULL;
        }
    }
    examplesProjectsList.clear();
    std::string tempProjectName= "";
    std::string newMultiLine= "";
    std::string exampleProjectsLocation = gpe::app_directory_name+"examples";
    gpe::error_log->report("Browsing for example projects in ["+exampleProjectsLocation+"] folder");
    if( sff_ex::path_exists( exampleProjectsLocation ) )
    {
        quickProjectReader * qpr = new quickProjectReader();
        gpe::GPE_FileDirectory * exampleFolderDirectory = new gpe::GPE_FileDirectory();

        exampleFolderDirectory->open_directory( exampleProjectsLocation );

        gpe::GPE_File * tempFile = NULL;
        std::vector<std::string > fileFilterTypes;
        fileFilterTypes.push_back("gppf");

        exampleFolderDirectory->filter_directory(false,fileFilterTypes);

        bool projectLoadedAlready = false;
        if( exampleFolderDirectory->get_count() > 0 )
        {
            projectFolderLocationLabel->set_name(exampleProjectsLocation);
            projectFolderLocationLabel->descriptionText = exampleProjectsLocation;

            for( i = 0; i < exampleFolderDirectory->get_count(); i++)
            {
                tempFile = exampleFolderDirectory->get_file(i);
                if( tempFile!=NULL)
                {
                    tempProjectName = exampleProjectsLocation+"/"+tempFile->get_name();

                    if( (int)tempProjectName.size() > 0 && sff_ex::file_exists(tempProjectName) )
                    {
                        projectLoadedAlready = false;
                        /*for( j = (int)recentProjectsList.size()-1; j>=0; j--)
                        {
                            tempProjectButton = recentProjectsList[j];
                            if( tempProjectButton!=NULL && tempProjectButton->descriptionText==tempProjectName )
                            {
                                projectLoadedAlready = true;
                                break;
                            }
                        }*/

                        if( !projectLoadedAlready && qpr->review_project(tempProjectName) )
                        {
                            if( sff_ex::file_exists(qpr->detectedIcon) ==false)
                            {
                                qpr->detectedIcon =gpe::app_directory_name+"resources/gamepencil_icon_72dpi.png";
                            }
                            newMultiLine = qpr->detectedName;
                            if((int)qpr->detectedVersion.size() > 0 )
                            {
                                newMultiLine+="\nVersion: "+qpr->detectedVersion;
                            }
                            if((int)qpr->detectedLanguage.size() > 0 )
                            {
                                newMultiLine+="\nLanguage: "+qpr->detectedLanguage;
                            }
                            //newMultiLine+="\nLocation: "+tempProjectName;
                            tempProjectButton = new GPE_VerticalCardButton( qpr->detectedIcon,newMultiLine, qpr->detectedLocation);
                            examplesProjectsList.push_back(tempProjectButton);
                        }
                    }
                }
            }
        }
        delete exampleFolderDirectory;
        exampleFolderDirectory = NULL;

        delete qpr;
        qpr = NULL;
    }
}

void gamePencilStartPageResource::load_projecs_folder( std::string projectsDirectory)
{
    GPE_VerticalCardButton * tempProjectButton = NULL;

    int i = 0;
    int j = 0;
    std::string tempProjectName = "";
    std::string newMultiLine = "";

    for( i = (int)foundProjectsList.size()-1; i>=0; i--)
    {
        tempProjectButton = foundProjectsList[i];
        if( tempProjectButton!=NULL)
        {
            delete tempProjectButton;
            tempProjectButton = NULL;
        }
    }
    foundProjectsList.clear();

    gpe::error_log->report("Browsing for  projects in ["+projectsDirectory+"] folder");
    if( sff_ex::path_exists(projectsDirectory) )
    {
        quickProjectReader * qpr = new quickProjectReader();
        gpe::GPE_FileDirectory * favFolderDirectory = new gpe::GPE_FileDirectory();
        foundProjectsDirectory = projectsDirectory;

        favFolderDirectory->open_directory(projectsDirectory);

        gpe::GPE_File * tempFile = NULL;
        std::vector<std::string > fileFilterTypes;
        fileFilterTypes.push_back("gppf");

        favFolderDirectory->filter_directory(false,fileFilterTypes);

        bool projectLoadedAlready = false;
        if( favFolderDirectory->get_count() > 0 )
        {
            projectFolderLocationLabel->set_name(projectsDirectory);
            projectFolderLocationLabel->descriptionText = projectsDirectory;
            if( main_EDITOR_SETTINGS!=NULL )
            {
                main_EDITOR_SETTINGS->projectFolderListLocation = projectsDirectory;
            }
            for( i = 0; i < favFolderDirectory->get_count(); i++)
            {
                tempFile = favFolderDirectory->get_file(i);
                if( tempFile!=NULL)
                {
                    tempProjectName = projectsDirectory+"/"+tempFile->get_name();

                    if( (int)tempProjectName.size() > 0 && sff_ex::file_exists(tempProjectName) )
                    {
                        projectLoadedAlready = false;
                        for( j = (int)recentProjectsList.size()-1; j>=0; j--)
                        {
                            tempProjectButton = recentProjectsList[j];
                            if( tempProjectButton!=NULL && tempProjectButton->descriptionText==tempProjectName )
                            {
                                projectLoadedAlready = true;
                                break;
                            }
                        }

                        if( !projectLoadedAlready && qpr->review_project(tempProjectName) )
                        {
                            if( sff_ex::file_exists(qpr->detectedIcon) ==false)
                            {
                                qpr->detectedIcon =gpe::app_directory_name+"resources/gamepencil_icon_72dpi.png";
                            }
                            newMultiLine = qpr->detectedName;
                            if((int)qpr->detectedVersion.size() > 0 )
                            {
                                newMultiLine+="\nVersion: "+qpr->detectedVersion;
                            }
                            if((int)qpr->detectedLanguage.size() > 0 )
                            {
                                newMultiLine+="\nLanguage: "+qpr->detectedLanguage;
                            }
                            //newMultiLine+="\nLocation: "+tempProjectName;
                            tempProjectButton = new GPE_VerticalCardButton( qpr->detectedIcon,newMultiLine, qpr->detectedLocation);
                            foundProjectsList.push_back(tempProjectButton);
                        }
                    }
                }
            }
        }
        delete favFolderDirectory;
        favFolderDirectory = NULL;

        delete qpr;
        qpr = NULL;
    }
}

void gamePencilStartPageResource::prerender_self( )
{

}

void gamePencilStartPageResource::load_resource(std::string file_path)
{

}

void gamePencilStartPageResource::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);

    if( !processedFirst )
    {
        if( main_EDITOR_SETTINGS!=NULL)
        {
            load_projecs_folder(main_EDITOR_SETTINGS->projectFolderListLocation);
            processedFirst = true;
        }
    }

    if( viewedSpace!=NULL && cam!=NULL && startPageList!=NULL)
    {
        headerSpaceHeight = 100;


        if( PANEL_GENERAL_EDITOR!=NULL )
        {
            headerViewedSpace->x = 0;
            headerViewedSpace->y = 0;
            headerViewedSpace->w = viewedSpace->w;
            headerViewedSpace->h = headerSpaceHeight;

            subViewedSpace->x = 0;
            subViewedSpace->y = headerSpaceHeight;
            subViewedSpace->w = viewedSpace->w;
            subViewedSpace->h = viewedSpace->h-headerSpaceHeight;
            sidePanelRect->x = sidePanelRect->y = sidePanelRect->w = sidePanelRect->h;
        }
        else
        {
            sidePanelRect->x = 0;
            sidePanelRect->y = 0;
            sidePanelRect->w = viewedSpace->w/4;
            sidePanelRect->h = viewedSpace->h;

            headerViewedSpace->x = sidePanelRect->w;
            headerViewedSpace->y = 0;
            headerViewedSpace->w = viewedSpace->w - headerViewedSpace->x;
            headerViewedSpace->h = headerSpaceHeight;

            subViewedSpace->x = sidePanelRect->w;
            subViewedSpace->y = headerSpaceHeight;
            subViewedSpace->w = viewedSpace->w - subViewedSpace->x;
            subViewedSpace->h = viewedSpace->h-headerSpaceHeight;
        }

        //Project Browser Code.
        int prevTab = projectsCategoryTabs->tabInUse;
        headerPageList->clear_list();
        headerPageList->set_coords( headerViewedSpace->x, headerViewedSpace->y );
        headerPageList->set_width(headerViewedSpace->w);
        headerPageList->set_height(headerViewedSpace->h );
        headerPageList->barXPadding = GENERAL_GPE_GUI_PADDING;
        headerPageList->barXMargin = 0;
        headerPageList->add_gui_element(labelCopyrightInfo,true);
        headerPageList->add_gui_element(loadProjectButton,false);
        headerPageList->add_gui_element(newProjectButton,false);
        headerPageList->add_gui_element(refreshButton,true);
        projectsCategoryTabs->set_width( headerViewedSpace->w - GENERAL_GPE_GUI_PADDING * 5 );
        headerPageList->add_gui_element(projectsCategoryTabs,true);
        headerPageList->process_self( viewedSpace,cam);


        if( prevTab!=projectsCategoryTabs->tabInUse)
        {
            //startPageList->reset_self();
        }

        startPageList->clear_list();
        startPageList->set_coords(subViewedSpace->x, subViewedSpace->y);
        startPageList->set_width(subViewedSpace->w);
        startPageList->set_height(subViewedSpace->h );
        startPageList->barXPadding = GENERAL_GPE_GUI_PADDING;
        startPageList->barXMargin = GENERAL_GPE_GUI_PADDING;
        startPageList->set_horizontal_align( gpe::fa_center );
        startPageList->yScroll->documentActions = true;
        //Recent Projects
        GPE_TextURL * tTUrl = NULL;
        int i = 0;
        int  rProjectListSize = 0;
        int favProjectListSize = 0;
        int exampleProjectListSize = 0;
        if(projectsCategoryTabs->get_selected_name()=="Recent Projects")
        {
            //int  rProjectListSize = editor_gui_main->get_recent_project_list_size();
            rProjectListSize = (int)recentProjectsList.size();
            if( rProjectListSize > 0)
            {
                for( i = 0; i <  rProjectListSize; i++)
                {
                    if( recentProjectsList[i]!=NULL)
                    {
                        //recentProjectsList[i]->set_height(64);
                        startPageList->add_gui_auto(recentProjectsList[i]);
                    }
                }
            }
            else
            {
                startPageList->add_gui_element(emptyProjectsListLabel,true);
            }
        }
        else if(projectsCategoryTabs->get_selected_name()=="Example Projects")
        {
            //int  rProjectListSize = editor_gui_main->get_recent_project_list_size();
            int  exProjectListSize = (int)examplesProjectsList.size();
            if( exProjectListSize > 0)
            {
                for( i = 0; i <  exProjectListSize; i++)
                {
                    if( examplesProjectsList[i]!=NULL)
                    {
                        //examplesProjectsList[i]->set_height(64);
                        startPageList->add_gui_auto(examplesProjectsList[i] );
                    }
                }
            }
            else
            {
                startPageList->add_gui_element(emptyProjectsListLabel,true);
            }
        }
        else
        {
            //Favorites Folder
            favProjectListSize = (int)foundProjectsList.size();
            startPageList->add_gui_element(scanProjectFolderButton,true);
            if( favProjectListSize > 0)
            {
                startPageList->add_gui_element(projectFolderLocationLabel,true);
                for( i = 0; i <  favProjectListSize; i++)
                {
                    if( foundProjectsList[i]!=NULL)
                    {
                        //foundProjectsList[i]->set_height(64);
                        startPageList->add_gui_auto(foundProjectsList[i] );
                    }
                }
            }
        }

        /*Patreon Area */
        startPageList->add_gui_element(labelPatreon,true);
        startPageList->add_gui_element(labelPatreonLogo,true);
        startPageList->add_gui_element(labelPatreonDescription,true);
        startPageList->add_gui_element(urlPatreonPage,true);

        startPageList->add_gui_element(communityLinksTitle,true);
        GPE_ToolIconButton * tCommunityButton = NULL;
        int menuLinksTotal = (int)communityButtons.size();
        for( int subMenuI= 0; subMenuI < menuLinksTotal; subMenuI++)
        {
            tCommunityButton = communityButtons.at(subMenuI);
            if( tCommunityButton!=NULL)
            {
                startPageList->add_gui_element( tCommunityButton, subMenuI == menuLinksTotal-1 );
            }
        }
        startPageList->process_self(viewedSpace,cam);

        if( refreshButton!=NULL && refreshButton->is_clicked() )
        {
            rProjectListSize = (int)recentProjectsList.size();
            i = 0;
            GPE_VerticalCardButton * tRecentListButton = NULL;
            if( rProjectListSize > 0)
            {
                for( i = rProjectListSize-1; i >=0; i--)
                {
                    tRecentListButton = recentProjectsList[i];
                    if( tRecentListButton!=NULL)
                    {
                        delete tRecentListButton;
                        tRecentListButton = NULL;
                    }
                }
                recentProjectsList.clear();
            }

            quickProjectReader * qpr = new quickProjectReader();
            tRecentListButton = NULL;
            std::string tempProjectName = "";
            std::string newMultiLine = "";
            for( i = 0; i < GPE_MAX_RECENT_project_LIST_SIZE; i++)
            {
                tempProjectName = editor_gui_main->get_recent_project_name(i);
                if( (int)tempProjectName.size() > 0 && sff_ex::file_exists(tempProjectName) )
                {
                    if( qpr->review_project(tempProjectName) )
                    {
                        if( sff_ex::file_exists(qpr->detectedIcon) ==false)
                        {
                            qpr->detectedIcon =gpe::app_directory_name+"resources/gamepencil_icon_72dpi.png";
                        }
                        newMultiLine = qpr->detectedName;
                        if((int)qpr->detectedVersion.size() > 0 )
                        {
                            newMultiLine+="\nVersion: "+qpr->detectedVersion;
                        }
                        if((int)qpr->detectedLanguage.size() > 0 )
                        {
                            newMultiLine+="\nLanguage: "+qpr->detectedLanguage;
                        }
                        //newMultiLine+="\nLocation: "+tempProjectName;
                        tRecentListButton = new GPE_VerticalCardButton( qpr->detectedIcon, newMultiLine, qpr->detectedLocation);
                        recentProjectsList.push_back(tRecentListButton);
                    }
                }
            }
            delete qpr;
            qpr = NULL;
            if( main_EDITOR_SETTINGS!=NULL)
            {
                load_projecs_folder(main_EDITOR_SETTINGS->projectFolderListLocation );
                processedFirst = true;
            }
            else
            {
                std::string favFolder = gpe::app_directory_name+"examples";
                projectFolderLocationLabel = new GPE_Label_Text(favFolder,favFolder);
                load_projecs_folder(favFolder);
            }
            gpe::input->reset_all_input();
            load_example_projects_folder();
            process_self(viewedSpace,cam);
        }
        else if( newProjectButton!=NULL && newProjectButton->is_clicked() )
        {
            editor_gui_main->launch_new_project();
        }
        else if( loadProjectButton!=NULL && loadProjectButton->is_clicked() )
        {
            editor_gui_main->open_new_project();
        }
        else if( scanProjectFolderButton!=NULL && scanProjectFolderButton->is_clicked() )
        {
            std::string lastDirectory = "";
            std::string newPathToScan = GPE_GetDirectoryName("Scan Folder for Projects", lastDirectory,false);
            if( sff_ex::path_exists(newPathToScan) )
            {
                load_projecs_folder(newPathToScan);
                gpe::input->reset_all_input();
                process_self(viewedSpace,cam);
            }
        }
        else
        {
            favProjectListSize = (int)foundProjectsList.size();
            exampleProjectListSize = (int)examplesProjectsList.size();
            rProjectListSize = (int)recentProjectsList.size();
            bool projectOpened = false;
            if(projectsCategoryTabs->get_selected_name()=="Recent Projects")
            {
                for( i = 0; i <  rProjectListSize && !projectOpened; i++)
                {
                    if( recentProjectsList[i]!=NULL)
                    {
                        if( recentProjectsList[i]->is_clicked() )
                        {
                            editor_gui_main->open_project(recentProjectsList[i]->descriptionText );
                            projectOpened= true;
                        }
                    }
                }
            }
            else if( projectOpened==false && projectsCategoryTabs->get_selected_name()=="Example Projects" )
            {
                for( i = 0; i <  exampleProjectListSize && !projectOpened; i++)
                {
                    if( examplesProjectsList[i]!=NULL)
                    {
                        if( examplesProjectsList[i]->is_clicked() )
                        {
                            editor_gui_main->open_project(examplesProjectsList[i]->descriptionText );
                            projectOpened= true;
                        }
                    }
                }
            }
            else if( projectOpened==false)
            {
                for( i = 0; i <  favProjectListSize && !projectOpened; i++)
                {
                    if( foundProjectsList[i]!=NULL)
                    {
                        if( foundProjectsList[i]->is_clicked() )
                        {
                            editor_gui_main->open_project(foundProjectsList[i]->descriptionText );
                            projectOpened = true;
                        }
                    }
                }
            }
        }
        processedFirst = true;

        if( PANEL_GENERAL_EDITOR!=NULL )
        {
            PANEL_GENERAL_EDITOR->clear_panel();
            PANEL_GENERAL_EDITOR->panelGuiList->set_horizontal_align( gpe::fa_center );
            PANEL_GENERAL_EDITOR->add_gui_element(labelImgGPELogo,true);
            PANEL_GENERAL_EDITOR->add_gui_element(helpPageButton,true);
            PANEL_GENERAL_EDITOR->add_gui_element(tipOfTheDayButton,true);
            PANEL_GENERAL_EDITOR->add_gui_element(labelHelpfulLinks,true);
            for( int i = 0 ; i < (int)helpfulWebLinks.size(); i++)
            {
                PANEL_GENERAL_EDITOR->add_gui_element( helpfulWebLinks[i],true);
            }

            PANEL_GENERAL_EDITOR->process_self();
        }
        else if( sidePanelList !=NULL )
        {
            sidePanelList->set_horizontal_align( gpe::fa_center );
            sidePanelList->set_coords(sidePanelRect->x,sidePanelRect->y);
            sidePanelList->set_width(sidePanelRect->w);
            sidePanelList->set_height(sidePanelRect->h);
            sidePanelList->barXPadding = GENERAL_GPE_GUI_PADDING;
            sidePanelList->barXMargin = GENERAL_GPE_GUI_PADDING;

            sidePanelList->clear_list();
            sidePanelList->add_gui_element(labelImgGPELogo,true );
            sidePanelList->add_gui_element(helpPageButton,true);
            sidePanelList->add_gui_element(tipOfTheDayButton,true);
            sidePanelList->yScroll->documentActions = true;

            sidePanelList->add_gui_element(labelHelpfulLinks,true);
            for( int i = 0 ; i < (int)helpfulWebLinks.size(); i++)
            {
                sidePanelList->add_gui_element( helpfulWebLinks[i],true);
            }

            sidePanelList->process_self(viewedSpace,cam);
        }

        //Get individual processes here
        if( tipOfTheDayButton->is_clicked() )
        {
            GPE_Show_Tip_Of_Day();
        }
        else if( helpPageButton->is_clicked() )
        {
            GPE_main_TabManager->add_new_tab(main_HELP_PAGE);
        }
    }
}

void gamePencilStartPageResource::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( viewedSpace!=NULL && cam!=NULL )
    {
        if( startPageList!=NULL)
        {
            startPageList->render_self( viewedSpace,cam);
        }

        if( sidePanelList!=NULL && PANEL_GENERAL_EDITOR==NULL )
        {
            sidePanelList->render_self( viewedSpace,cam );
        }

        if( headerPageList!=NULL)
        {
            headerPageList->render_self( viewedSpace,cam);
        }
    }
}
void gamePencilStartPageResource::save_resource(std::string file_path, int backupId)
{
    bool usingAltSaveSource = false;
    isModified = false;
}

bool gamePencilStartPageResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    return true;
}


