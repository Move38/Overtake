#include "state-player-assign.h"
#include "game-def.h"
#include "action.h"
#include "player.h"
#include "animate.h"
#include "timer.h"
#include "state-board.h"

#pragma clang diagnostic ignored "-Wnarrowing"

namespace statePlayerAssign{
    
    bool _neighboors[FACE_COUNT];
    
    byte getMissingNeighboor(){
        FOREACH_FACE(f) {
            if(_neighboors[f] && isValueReceivedOnFaceExpired(f)){
                return f;
            }
        }
        return FACE_COUNT;
    }

    void handlePlayerLocked(byte nextState) {
        for(int i = 0; i < PLAYER_LIMIT; i++) {
            player::add(i);
        }
        timer::cancel();
        stateBoard::reset();
        stateCommon::handleStateChange(nextState);
    }

    void handleNeighborLost(){
        if(getMissingNeighboor() == FACE_COUNT) {
            return;
        }
        if(!isAlone()){
            action::broadcast(action::Action{.type=GAME_DEF_ACTION_PLAYER_LOCKED, .payload=0});
            handlePlayerLocked(GAME_DEF_STATE_BOARD);
            return;
        }
        handlePlayerLocked(GAME_DEF_STATE_MOVER);        
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        if(isEnter) {
            player::reset();
            timer::cancel();
            buttonSingleClicked();
            FOREACH_FACE(f){
                _neighboors[f] = !isValueReceivedOnFaceExpired(f);
            }
        }
        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_PLAYER_LOCKED)){
            handlePlayerLocked(GAME_DEF_STATE_BOARD);
            return;
        }
        if(getMissingNeighboor() < FACE_COUNT && timer::runningFor() == 0) {
            timer::mark(700, handleNeighborLost);
        }
        setColor(dim(WHITE, 100));
    }
}