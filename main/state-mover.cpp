#include "blink-types.h"
#include "state-mover.h"
#include "animate.h"
#include "player.h"
#include "action.h"
#include "game-def.h"
#include "timer.h"
namespace stateMover {

    #define SHOW_IDLE 0
    #define SHOW_SENDING 1
    #define SHOW_DONE 2
    
    byte _showStatus = SHOW_IDLE;
    
    byte _currentPlayer;

    void handleSendDone(){
        _showStatus = SHOW_DONE;
    }
    void handleDelayedSend() {
        action::send(action::Action{.type=GAME_DEF_ACTION_MOVE_REQUEST, .payload = _currentPlayer}, 0);
        timer::mark(800, handleSendDone);
    }

    void handleConnectionStatus(){
        if(isAlone()) {
            _showStatus = SHOW_IDLE;
            timer::cancel();
            return;
        }
        if(_showStatus == SHOW_IDLE && !isValueReceivedOnFaceExpired(0)) {
            _showStatus = SHOW_SENDING;
            timer::mark(50, handleDelayedSend);
        }
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data) {
        if(isEnter) {
            _showStatus = SHOW_IDLE;
            _currentPlayer = 0;
            buttonSingleClicked();
        }
        if(buttonSingleClicked()){
            _currentPlayer = (_currentPlayer + 1) % player::getCount();
        }
        handleConnectionStatus();
        if(_showStatus == SHOW_SENDING) {
            animate::spin(player::getColor(_currentPlayer), 4);
            return;
        }
        animate::radiate(player::getColor(_currentPlayer), 0, 6);
    }
}