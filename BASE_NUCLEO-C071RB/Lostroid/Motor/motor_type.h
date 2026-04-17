/********************************************************************
* File:    mortor_type.h
* Author:  LOSTROID
* Created: 2026-03-07
* Encoding: UTF-8
********************************************************************/
#ifndef H_MOTOR_TYPE_H
#define H_MOTOR_TYPE_H

#include "../Base/config.h"

typedef enum{
    m_MOTOR_LAST_STATE_OPEN,
    m_MOTOR_LAST_STATE_CLOSE,
    m_MOTOR_LAST_STATE_UNKNOWN
}te_Motor_State;

typedef struct {
    te_Motor_State e_state;
    tu32 v_time;
}ts_Motor_Context;

#endif
