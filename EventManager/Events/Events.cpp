#include "stdafx.h"

#include "DestroyActorEventData.h"
#include "PauseGameEventData.h"
#include "ExitGameEventData.h"
#include "StartGameEventData.h"
#include "DragInitEventData.h"
#include "NewActorEventData.h"
#include "PhysCollisionEventData.h"
#include "ConnectToPVDEventData.h"
#include "DebugRenderingEventData.h"
#include "SaveGameEventData.h"
#include "MinimizeGameEventData.h"
#include "MouseMoveEventData.h"
#include "DebugCameraEventData.h"
#include "ChangeMenuScreenEventData.h"
#include "JumpEventData.h"
#include "CollectItemEventData.h"
#include "LooseEventData.h"
#include "WinEventData.h"
#include "GoToMainMenuEventData.h"
#include "ContinueGameEventData.h"
#include "ChangeGravityEventData.h"
#include "OpenExitDoorEventData.h"
#include "StartLevelEventData.h"


const ULONG DragInitEventData::s_TYPE = 0xda661272;
const ULONG DestroyActorEventData::s_TYPE = 0x9f178637;
const ULONG PauseGameEventData::s_TYPE = 0xa8339f32;
const ULONG ExitGameEventData::s_TYPE = 0xebcb1034;
const ULONG StartGameEventData::s_TYPE = 0x9f08bbb7;
const ULONG NewActorEventData::s_TYPE = 0x4ab69913;
const ULONG PhysCollisionEventData::s_TYPE = 0x843d6771;
const ULONG ConnectToPVDEventData::s_TYPE = 0x653363cb;
const ULONG DebugRenderingEventData::s_TYPE = 0xb4d8ba57;
const ULONG SaveGameEventData::s_TYPE = 0xf97ec779;
const ULONG MinimizeGameEventData::s_TYPE = 0x481e76a5;
const ULONG MouseMoveEventData::s_TYPE = 0x36043aa0;
const ULONG ShowMouseCursorEventData::s_TYPE = 0x3c292670;
const ULONG MovementEventData::s_TYPE = 0x39d7f13f;
const ULONG DebugCameraEventData::s_TYPE = 0x13c76444;
const ULONG ChangeMenuScreenEventData::s_TYPE = 0xf2d7ff0d;
const ULONG ActivateGuiElementEventData::s_TYPE = 0x39229316;
const ULONG ActivateGuiElementByMouseEventData::s_TYPE = 0xbe13d626;
const ULONG RightMouseButtonEventData::s_TYPE = 0x2301f1c1;
const ULONG LeftMouseButtonEventData::s_TYPE = 0xbdca668f;
const ULONG JumpEventData::s_TYPE = 0x7ee6be25;
const ULONG EnableInputEventData::s_TYPE = 0xae3ce019;
const ULONG CollectItemEventData::s_TYPE = 0xee300004;
const ULONG LooseEventData::s_TYPE = 0xe266c2bb;
const ULONG WinEventData::s_TYPE = 0x1204fef9;
const ULONG GoToMainMenuEventData::s_TYPE = 0xc77b0add;
const ULONG ContinueGameEventData::s_TYPE = 0x8128bd4f;
const ULONG ChangeGravityEventData::s_TYPE = 0xce9f81b8;
const ULONG OpenExitDoorEventData::s_TYPE = 0x595411c7;
const ULONG StartLevelEventData::s_TYPE = 0x87295a9f;
const ULONG ComputePlayerRewardEventData::s_TYPE = 0xed0b106b;
const ULONG StartNewGameEventData::s_TYPE = 0x7f19810b;




