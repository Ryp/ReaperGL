////////////////////////////////////////////////////////////////////////////////
/// Reaper
///
/// Copyright (c) 2015-2017 Thibault Schueller
/// This file is distributed under the MIT License
////////////////////////////////////////////////////////////////////////////////

#ifndef REAPER_ROOT_INCLUDED
#define REAPER_ROOT_INCLUDED

class GameLogic;
class ILog;
struct Renderer;

struct ReaperRoot
{
    GameLogic*  game;
    Renderer*   renderer;
    ILog*       log;
};

#endif // REAPER_ROOT_INCLUDED
