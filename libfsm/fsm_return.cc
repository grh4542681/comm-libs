#include "fsm_return.h"

namespace fsm {

FsmRet::ECodeMapType FsmRet::ECodeMap = {
    { ECode::FSM_EVENT_ETRANSEXIST, "There exists a trans corresponding to event" },
    { ECode::FSM_STATE_ETRANSEXIST, "There exists a trans corresponding to state" },
    { ECode::FSM_EVENT_ENOTEXIST, "Event not exist" },
    { ECode::FSM_STATE_ENOTEXIST, "State not exist" },
    { ECode::FSM_STATE_EEXIT, "State exit function fail" },
    { ECode::FSM_STATE_EUPDATE, "State update function fail" },
    { ECode::FSM_STATE_EENTER, "State enter function fail" },
    { ECode::FSM_TRANS_ENOTEXIST, "Transition not exits" },
    { ECode::FSM_TRANS_ECHECK, "Transition check function fail" },
    { ECode::FSM_TRANS_ECOMPLETE, "Transition complete function fail" },
};

}
