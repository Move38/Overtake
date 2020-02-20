#include "global-events.h"
#include "game-def.h"

#pragma clang diagnostic ignored "-Wnarrowing"

namespace globalEvents {

    void changeAllToReset(){
        action::broadcast(action::Action{.type=GAME_DEF_ACTION_RESET, .payload=millis()});
        stateCommon::handleStateChange(GAME_DEF_STATE_DEFAULT);
    }

    bool isHandled(const stateCommon::LoopData& data){
        if(buttonDoubleClicked()) {
            changeAllToReset();
            return true;
        } 
        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_RESET)) {
            stateCommon::handleStateChange(GAME_DEF_STATE_DEFAULT);
            return true;
        }
        return false;
    }

}