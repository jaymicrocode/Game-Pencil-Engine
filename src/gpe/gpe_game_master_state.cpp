/*
GPE_Game_Master_State.cpp
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

#include "gpe_game_master_state.h"
#include "gpe_settings.h"

namespace gpe
{
    game_master::game_master( std::string sName )
    {
        if( (int)sName.size() > 0 )
        {
            stateName = sName;
        }
        else
        {
            stateName = "GPE_Master";
        }
        for( int imCam = 0; imCam < max_cameras_allowed; imCam++)
        {
            MOUSE_CAM_X[imCam] = 0;
            MOUSE_CAM_Y[imCam] = 0;
            MOUSE_IN_CAM[imCam] = true;
        }

        currentScene = NULL;
        currentSceneId = -1;

        if(  GPE_SPATIAL_GRID != NULL)
        {
            delete GPE_SPATIAL_GRID;
            GPE_SPATIAL_GRID = NULL;
        }
        spatialPartitonController = new spatial_partition_controller();
    }


    game_master::~game_master()
    {
        clean_up();
    }

    //Main loop functions

    void game_master::apply_game_objects_logic()
    {
        if( currentScene!=NULL )
        {
            currentScene->apply_logic();
        }
    }

    void game_master::apply_game_objects_prelogic()
    {

    }

    void game_master::apply_logic()
    {
        if( window_controller_main->window_closed || input->exit_requested  )
        {
            game_runtime->state_set( "exit" );
        }

        process_scene_movement();
        if( currentScene == NULL )
        {
            return;
        }

        if( currentScene->scene_is_ready()!=true)
        {
            currentScene->scene_init();
        }
        currentScene->process_scene();
        if( currentScene->scene_is_ready() == false )
        {
            return;
        }
        game_object * foundRegObj = NULL;
        game_object * foundTypedObj = NULL;
        int iObjectType = -1;
        int jObject = -1;
        game_object_list *  foundObjTypeHolder = NULL;
        int jj = 0;
        int kItr = 0;
        int fcSpace = 0;

        //deletes objects set for deletion
        remove_deleted_objects();

        //apply game object prelogics
        apply_game_objects_prelogic();
        //apply_spatial_game_objects_prelogics(this.collisionSpacesInView);
        init_spatialpartioning();

        update_spaces_in_view();
        //process collisions
        parse_spatial_collisions();

        currentScene->apply_logic();
    }

    void game_master::init_spatialpartioning()
    {
        if( currentScene!=NULL )
        {
            currentScene->init_spatialpartioning();
        }
    }

    void game_master::clean_up()
    {
        currentScene = NULL;
        game_scene * cScene = NULL;
        error_log->report("Attempting to remove ["+ stg_ex::int_to_string( (int)gpeScenes.size() )+"] scenes");
        for( int iScene = (int)gpeScenes.size()-1; iScene >=0; iScene--)
        {
            cScene = gpeScenes[iScene];
            if( cScene!=NULL)
            {
                delete cScene;
                cScene = NULL;
            }
        }
        gpeScenes.clear();

        game_path2d * cPath = NULL;
        error_log->report("Attempting to remove ["+ stg_ex::int_to_string( (int)gamePaths.size() )+"] paths");

        for( int iPath = (int)gamePaths.size()-1; iPath >=0; iPath--)
        {
            cPath = gamePaths[iPath];
            if( cPath!=NULL)
            {
                delete cPath;
                cPath = NULL;
            }
        }
        gamePaths.clear();

        GAME_OBJECTS.clear();
        GAME_OBJECTS_TO_BE_DESTROYED.clear();
        GAME_OBJECTS_TO_DELETE_TYPES.clear();
        GAME_OBJECTS_CONTINUOUS.clear();
        collisionSpacesInView.clear();
        objectCollisionIsChecked.clear();

        error_log->report("State removed..");
    }

    void game_master::end_state()
    {

    }

    void game_master::parse_spatial_collisions()
    {

    }

    void game_master::process_input()
    {
        if( window_controller_main->window_closed || input->exit_requested  )
        {
            game_runtime->state_set( "exit" );
        }
    }

    void game_master::process_scene_movement()
    {
        if( currentSceneId!=sceneToEnter && sceneToEnter>=0 && sceneToEnter < (int)gpeScenes.size() )
        {
            int nextSceneToGo = sceneToEnter;
            int previousSceneIn = currentSceneId;
            int jj;
            int iHolderType;
            game_object_list * foundObjTypeHolder;
            game_object * foundRegObj;
            GAME_OBJECTS_CONTINUOUS.clear();
            for( iHolderType = (int)GAME_OBJECTS.size()-1; iHolderType>=0; iHolderType--)
            {
                foundObjTypeHolder = GAME_OBJECTS.at( iHolderType );
                for( jj=(int)foundObjTypeHolder->internalArray.size()-1; jj>=0; jj--)
                {
                    foundRegObj = foundObjTypeHolder->internalArray[jj];
                    if( foundRegObj->gpeIsBeingDestroyed==false )
                    {
                        foundRegObj->scene_end();
                    }
                    if( foundRegObj->isContinuousObject  )
                    {
                        GAME_OBJECTS_CONTINUOUS.push_back( foundRegObj );
                    }
                }
            }
            //if( this.currentScene.sceneIsContinuous ==false )
            {
                //Restart Object and scene related things
                scene_restart_object_info();

                currentScene->scene_restart();
            }
            currentScene = gpeScenes[ nextSceneToGo ];
            sceneToEnter = nextSceneToGo;
            currentSceneId = nextSceneToGo;
            currentScene->update_tilemap();
        }
    }

    void game_master::render()
    {
        renderer_main->reset_viewpoint();
        gcanvas->render_rectangle( 0, 0, screen_width, screen_height, c_black, false, 255 );
        if( currentScene!=NULL )
        {
            currentScene->render_scene();
            gfs->render_text(32,32,"Current Scene = ["+currentScene->get_scene_name()+"]",c_white, font_default,  fa_left, fa_top );

        }
        else
        {
            gfs->render_text(32,32,"Error: Current Scene = NULL",c_red, font_default,  fa_left, fa_top );
        }
        gfs->render_fps(font_default, c_green, screen_width -32, 32, fa_right, fa_top );
    }

    int game_master::remove_deleted_objects()
    {
        int objectsDeleted = 0;
        int jObject = 0;
        int kItr = 0;
        game_object * foundRegObj = NULL;
        game_object_list * foundObjTypeHolder = NULL;
        for( int iObjectType = (int)GAME_OBJECTS.size()-1; iObjectType>=0; iObjectType--)
        {
            foundObjTypeHolder = GAME_OBJECTS[iObjectType];
            if( foundObjTypeHolder!=NULL )
            {
                for( jObject = (int)foundObjTypeHolder->internalArray.size() -1; jObject >=0; iObjectType-- )
                {
                    foundRegObj = foundObjTypeHolder->internalArray[jObject];
                    if( foundRegObj->gpeIsBeingDestroyed == true )
                    {
                        foundObjTypeHolder->internalArray.erase( foundObjTypeHolder->internalArray.begin() + jObject );
                        /*
                        for( kItr = foundRegObj.SPATIAL_PARTITION_LIST.length -1; kItr>=0; kItr--)
                        {
                            fcSpace = foundRegObj.SPATIAL_PARTITION_LIST[kItr];
                            if( fcSpace < this.COLLISION_AREA_SPACES.length && fcSpace >=0 )
                            {
                                spaceToDeleteObjectFrom = this.COLLISION_AREA_SPACES[fcSpace];
                                spaceToDeleteObjectFrom.remove_held_object(foundRegObj);
                            }
                            else
                            {
                                this.COLLISION_AREA_OUTOFLEVEL.remove_held_object(foundRegObj);
                            }
                        }
                        */
                        objectsDeleted++;
                    }
                }
            }
        }
        return objectsDeleted;
    }

    void game_master::scene_restart_object_info()
    {

    }

    void game_master::start_state()
    {
        cursor_main_controller->cursor_change( cursor_main_controller->cursor_system_name( cursor_default_type::arrow ) );
    }

    void game_master::update_spaces_in_view()
    {
        collisionSpacesInView.clear();
        int cSceneViewFound = 0;
        int icameraX = 0;
        int jcameraY = 0;
        int viewPartitionSpace = 0;
        int currentSpace = is_null;
        if( currentScene->scene_is_ready() )
        {
            for( int iV=0; iV < max_cameras_allowed; iV++)
            {
                /*
                cSceneViewFound = currentScene->sceneView[iV];
                if( cSceneViewFound->isVisible)
                {
                    for( icameraX = cSceneViewFound.cameraRect.xPos; icameraX <= cSceneViewFound.cameraRect.x2Pos+spatialGridWidthSize;icameraX+=spatialGridWidthSize)
                    {
                        for(  jcameraY = cSceneViewFound.cameraRect.yPos; jcameraY <= cSceneViewFound.cameraRect.y2Pos+spatialGridHeightSize;jcameraY+=spatialGridHeightSize)
                        {
                            viewPartitionSpace = (( icameraX / spatialGridWidthSize)|0) * spatialGridHeightAmount;
                            viewPartitionSpace+= ( jcameraY / spatialGridHeightSize)|0;
                            if( viewPartitionSpace < COLLISION_AREA_SPACES.length )
                            {
                                collisionSpacesInView.push_back(viewPartitionSpace);
                            }
                        }
                    }
                }
                */
            }
        }
    }
}
