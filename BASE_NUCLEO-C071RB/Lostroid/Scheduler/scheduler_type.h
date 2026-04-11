/******************************************************************************
* File:    scheduler_type.h
* Author:  LOSTROID
* Created: 2025-06-23
*
* Description:
* This is the Scheduler Control.
*
* Revision History:
*   2026-02-23  v1.01  UPDATE
*   2025-06-23  v1.00  NEW
-------------------------------------------------------------------------------
01. a = array variable                      e.g. a_Data[]
02. b = bit fields                          e.g. b_Data
03. c = const                               e.g, c_Data
04. d = define                              e.g. d_Data
05. e = enum Type                           e.g. e_Data
06. f = Function                            e.g. f_Data
07. g = Source file static variable         e.g. gv_Data
08. i = Inline                              e.g. i_Data
09. l = Static variables inside functions   e.g. lv_Data
10. m = enum member                         e.g. m_Data
11. p = Pointer                             e.g. p_Data 
12. s = struct                              e.g. s_Data 
13. t = typedef                             e.g. t_Data 
14. u = Union                               e.g. u_Data 
15. v = variable                            e.g. v_Data
16. x = extern                              e.g. xv_data
******************************************************************************/
#ifndef H_SCHEDULER_TYPE_H
#define H_SCHEDULER_TYPE_H

#include "../Base_LL/base_ll_config.h"
#include "../Base_LL/base_ll_tick_type.h"

#define d_SCHEDULER_RUN_TIME_AVG_SIZE   100  /// 측정 평균 수
//+ When modifying, message-text mapping work in the source file is also required.
//+  수정시 소스 파일에 메시지텍스트 매핑 작업도 필요
typedef enum 
{
    m_SCH_ERROR_NONE,                   //+ No error : 에러 없음.
    m_SCH_ERROR_JOB_TABLE_NULL,         //+ Job table pointer is NULL : Job 테이블 포인터 없음.
    m_SCH_ERROR_JOB_RUN_NULL,           //+ Job execution pointer is NULL : Job 실행 포인터 없음.
    m_SCH_ERROR_JOB_NEXT_NULL,          //+ Job next pointer is NULL : Job 다음 포인터 없음.
    m_SCH_ERROR_JOB_JUMP_NULL,          //+ Job jump pointer is NULL : Job 점프 포인터 없음.
    m_SCH_ERROR_JOB_JUMP_OVER,          //+ Exceeded maximum jump position : 점프 위치를 벗어남.
    m_SCH_ERROR_CRC,                    //+ CRC check failed : CRC이상.
    m_SCH_ERROR_DONE,                   //+ DONE flag is not set : DONE이 되지 않음
    m_SCH_ERROR_LEN,                    //+ Length mismatch : 길이가 틀림.
    m_SCH_ERROR_VALUE,                  //+ Value mismatch : 값이 틀림.
    m_SCH_ERROR_EMPTY,                  //+ Empty : 비어 있음.
    m_SCH_ERROR_RETRY,                  //+ Retry failed : 재시도 실패.
    m_SCH_ERROR_SWITCH_CASE,            //+ Switch-case out of range : 스위치 케이스 영역을 벗어남.
    m_SCH_ERROR_TIMEOUT                 //+ timeout : 시간 초과.
}te_SCHEDULER_ERR;

//+ Function execution level: 레벨에 따라 수행제한
//  Lower value = higher priority: 레벨이 낮을 수록 우선 권이 높다.
typedef enum 
{
    m_SCH_PRIORITY_LV0,     //+ Level 0 higher priority
    m_SCH_PRIORITY_LV1,     //+ Level 1
    m_SCH_PRIORITY_LV2,     //+ Level 2
    m_SCH_PRIORITY_LV3,     //+ Level 3
    m_SCH_PRIORITY_LV4,     //+ Level 4
    m_SCH_PRIORITY_LV5,     //+ Level 5
    m_SCH_PRIORITY_LV6,     //+ Level 6
    m_SCH_PRIORITY_LV7,     //+ Level 7
    m_SCH_PRIORITY_LV8,     //+ Level 8
    m_SCH_PRIORITY_LV9      //+ Level 9 Lower priority
}te_SCHEDULER_PRIORITY;

typedef struct ts_Scheduler_Ctrl 
{
    te_SCHEDULER_PRIORITY e_priority_cfg;               //+ 동작 제한 레벨
    te_SCHEDULER_ERR e_error;                           //+ 에러 정보
    tu32 v_interval_uncount;                            //+ 패널티 횟수(함수 수행시간 초과시 100us 간격으로 실행지연)
    tu32 v_retry_max_count;                             //+ 재시도 횟수 설정
    tu32 v_retry_countdown;                             //+ 재시도 횟수 카운터
    // Time control
    ts_LL_Tick_Context s_cycle_run_time;                //+ 모듈 1주기 타임 (250ms 면 250ms 한 주기 동작 형태
    tu32 v_time_run_count;                              //+ 함수 수행 시간 카운터
    tu32 v_time_run_now_us;                             //+ 함수 현재 수행시간 정보 us
    tu32 v_time_run_avg100_us;                          //+ 함수 평균 수행시간 정보 us 근사치 평균
    tu32 v_time_run_max_us;                             //+ 함수 최대 수행시간 정보 us
    tu32 v_time_cycle_count;                            //+ 전체 사이클 수행 시간 카운터
    tu32 v_time_cycle_now_us;                           //+ 전체 사이클 현재 수행시간 정보 us
    tu32 v_time_cycle_avg100_us;                        //+ 전체 사이클 평균 수행시간 정보 us 근사치 평균
    tu32 v_time_cycle_max_us;                           //+ 전체 사이클 최대 수행시간 정보 us
    tu32 v_timeout_us_cfg;                              //+ "타임아웃 설정값 (외부 인터페이스로 부터 수신 타임아웃 기능)
    tu32 v_timeout_count;                               //+ "타임아웃 카운터 (외부 인터페이스로 부터 수신 카운터 기능)
    tu32 v_delay_interval_target_us;                    //+ 간격 지연 설정값 (연속 전송 대기시간)
    tu32 v_delay_interval_tick_count;                   //+ 간격 지연 카운터 (연속 전송 대기시간)
    tu32 v_delay_operation_target_us;                   //+ 수행 지연 설정값 (특정 기능수행 대기시간)
    tu32 v_delay_operation_tick_count;                  //+ 수행 지연 카운터 (특정 기능수행 대기시간)
    // Table control
    void (**ps_table_list)(struct ts_Scheduler_Ctrl*);  //+ 테이블 리스트
    char* p_table_name;                                 //+ 테이블 이름 string
    tu32 v_table_size;                                  //+ 태이블 사이즈 정보
    tu32 v_table_pos;                                   //+ 태이블 위치 정보
    void (*pf_func)(struct ts_Scheduler_Ctrl*);         //+ 함수 포인터
    tu32 v_case_step;                                   //+ case 문 위치 저장
    void *ps_user_struct;                               //+ 사용자 구조체 등록
}ts_Scheduler_Ctrl;      

#endif
