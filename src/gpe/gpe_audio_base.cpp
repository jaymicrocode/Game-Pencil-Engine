/*
gpe_audio_base.cpp
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

#include "gpe_audio_base.h"

namespace gpe
{
    bool sound_is_working = false;
    float sound_global_volume = 100.f;

    music_base * sound_music_object = NULL;
    sound_base * standard_sound_object = NULL;

    bool sound_is_format_supported[ sound_format_max ];
    std::string sound_type_names[ sound_format_max ];
    std::string sound_system_name = "undefined";
    sound_base::sound_base()
    {
        sound_type = "sound_base";
        last_channel_played = -1;
        sound_loaded = false;
        sound_id = -1;
        sound_name = "";
        file_name = "";
        sound_group_id = -1;
    }

    sound_base::sound_base( std::string s_name, std::string s_file, int group_id_number, int s_id )
    {
        last_channel_played = -1;
        sound_loaded = false;
        sound_id = s_id;
        sound_name = s_name;
        file_name = s_file;
        sound_group_id = group_id_number;
    }

    sound_base::~sound_base()
    {

    }

    sound_base * sound_base::create_new( std::string s_name, std::string s_file, int group_id_number, int s_id  )
    {
        return NULL;
    }

    int sound_base::get_id()
    {
        return sound_id;
    }

    std::string sound_base::get_name()
    {
        return sound_name;
    }

    std::string sound_base::get_file_name()
    {
        return file_name;
    }

    std::string sound_base::get_type()
    {
        return sound_type;
    }

    float sound_base::get_volume()
    {
        return -1;
    }

    bool sound_base::is_loaded()
    {
        return sound_loaded;
    }

    bool sound_base::load( std::string s_file)
    {
        return false;
    }

    void sound_base::pause()
    {

    }

    void sound_base::set_volume( float vol )
    {

    }

    int sound_base::play( int play_count ,int s_channel )
    {
        return -1;
    }


    void sound_base::stop()
    {

    }

    void sound_base::unload()
    {

    }

    music_base::music_base()
    {
        sound_type = "music_base";
        last_channel_played = -1;
        sound_loaded = false;
        sound_id = -1;
        sound_name = "";
        file_name = "";
        sound_group_id = -1;
        music_is_playing = -1;
    }
    music_base::music_base( std::string s_name, std::string s_file, int group_id_number, int s_id )
    {
        sound_id = s_id;
        sound_name = s_name;
        file_name = s_file;
        music_is_playing = false;
        sound_group_id = group_id_number;
    }

    music_base::~music_base()
    {

    }

    sound_base * music_base::create_new( std::string s_name, std::string s_file, int group_id_number, int s_id  )
    {
        return NULL;
    }

    music_base * music_base::create_new_music( std::string s_name, std::string s_file, int group_id_number, int s_id  )
    {
        return NULL;
    }

    bool music_base::is_playing()
    {
        return music_is_playing;
    }

    bool music_base::load( std::string s_file )
    {
        return false;
    }

    void music_base::pause()
    {

    }

    int music_base::play( int play_count, int s_channel  )
    {
        return -1;
    }

    void music_base::stop()
    {

    }

    void music_base::unload()
    {

    }

}
