/********************************************************************
* File:    scheduler_type.h
* Author:  LOSTROID
* Created: 2025-06-23
* Encoding: UTF-8
---------------------------------------------------------------------
01. a_ = array                         e.g. a_data[]
02. b_ = bit field                     e.g. b_Data
03. c_ = constant                      e.g, c_Data
04. d_ = #define macro                 e.g. d_Data
05. e_ = enum type                     e.g. e_Data
06. f_ = function                      e.g. f_Data
07. g_ = global (source file static)   e.g. gv_Data
08. i_ = inline function               e.g. i_Data
09. l_ = local static variable         e.g. lv_Data
10. m_ = enum member                   e.g. m_Data
11. p_ = pointer                       e.g. p_Data 
12. s_ = struct                        e.g. s_Data 
13. t_ = typedef                       e.g. t_Data 
14. u_ = union                         e.g. u_Data 
15. v_ = variable                      e.g. v_Data
16. x_ = extern variable               e.g. xv_data
********************************************************************/
#ifndef H_SCHEDULER_TYPE_H
#define H_SCHEDULER_TYPE_H

#include "../Base/config.h"
#include "../Base/base_tick_type.h"

#define d_SCHEDULER_RUN_TIME_AVG_SIZE   100  /// 측정 평균 수

//+ When modifying, message-text mapping work in the source file is also required.
//"수정시 소스 파일에 메시지텍스트 매핑 작업도 필요"
typedef enum 
{
    m_SCH_ERROR_NONE,                                       //+ No error "에러 없음"
    m_SCH_ERROR_JOB_TABLE_NULL,                             //+ Job table pointer is NULL "Job 테이블 포인터 없음"
    m_SCH_ERROR_JOB_RUN_NULL,                               //+ Job execution pointer is NULL "Job 실행 포인터 없음"
    m_SCH_ERROR_JOB_NEXT_NULL,                              //+ Job next pointer is NULL "Job 다음 포인터 없음"
    m_SCH_ERROR_JOB_JUMP_NULL,                              //+ Job jump pointer is NULL "Job 점프 포인터 없음"
    m_SCH_ERROR_JOB_JUMP_OVER,                              //+ Exceeded maximum jump position "점프 위치를 벗어남"
    m_SCH_ERROR_CRC,                                        //+ CRC check failed "CRC이상"
    m_SCH_ERROR_DONE,                                       //+ DONE flag is not set "DONE이 되지 않음"
    m_SCH_ERROR_LEN,                                        //+ Length mismatch "길이가 틀림"
    m_SCH_ERROR_VALUE,                                      //+ Value mismatch "값이 틀림"
    m_SCH_ERROR_EMPTY,                                      //+ Empty "비어 있음"
    m_SCH_ERROR_RETRY,                                      //+ Retry failed "재시도 실패"
    m_SCH_ERROR_SWITCH_CASE,                                //+ Switch-case out of range "스위치 케이스 영역을 벗어남"
    m_SCH_ERROR_TIMEOUT                                     //+ timeout "시간 초과"
}te_SCHEDULER_ERR;

//+ Collection of scheduler control states
//"스케쥴 제어 상태 모음"
typedef struct ts_Scheduler_Control 
{
    te_SCHEDULER_ERR e_error;                               //+ Error information "에러 정보"
    tu32 v_penalty_set_value;                               //+ Wait weight "대기 가중치"
    tu32 v_penalty_down_count;                              //+ Wait decrement counter "대기 감소 카운터"
    tu32 v_retry_set_value;                                 //+ Retry count setting "재시도 횟수 설정"
    tu32 v_retry_down_count;                                //+ Retry counter "재시도 횟수 카운터"
    ts_Base_Tick_Context s_time_interval;                   //+ Module cycle time "모듈 1주기 타임"

    tu32 v_time_run_count;                                  //+ Function execution time counter "함수 수행 시간 카운터"
    tu32 v_time_run_now_us;                                 //+ Current function execution time (us) "함수 현재 수행시간 us"
    tu32 v_time_run_tick_us;                                //+ Average function execution time (approximate average in us) "함수 평균 수행시간 us 근사치 평균"
    tu32 v_time_run_max_us;                                 //+ Maximum function execution time (us) "함수 최대 수행시간 us"
    tu32 v_time_loop_count;                                 //+ Total loop execution time counter "전체 loop 수행 시간 카운터"
    tu32 v_time_loop_now_us;                                //+ Current total loop execution time (us) "전체 loop 현재 수행시간 us"
    tu32 v_time_loop_tick_us;                               //+ Average total loop execution time (approximate average in us) "전체 loop 평균 수행시간 us 근사치 평균"
    tu32 v_time_loop_max_us;                                //+ Maximum total loop execution time (us) "전체 loop 최대 수행시간 us"

    tu32 v_timeout_set_value_us;                            //+ Operation timeout setting "동작 타임아웃 설정값"
    tu32 v_timeout_tick_count;                              //+ Operation timeout counter "동작 타임아웃 카운터"
    tu32 v_delay_set_us;                                    //+ Delay time us "설정 대기시간 us"
    tu32 v_delay_tick_count;                                //+ Delay tick counter "연속 전송 대기시간 카운터"
    tu32 v_wait_set_us;                                     //+ Wait time us "설정 대기시간 us"
    tu32 v_wait_tick_count;                                 //+ Wait tick counter "틱 카운터 변수"

    void (**ps_table_list)(struct ts_Scheduler_Control*);   //+ Function Table list "함수 테이블 리스트"
    char* p_table_name;                                     //+ Table name (string) "테이블 이름 string"
    tu32 v_table_size;                                      //+ Table size information "태이블 사이즈 정보"
    tu32 v_table_pos;                                       //+ Table position information "태이블 위치 정보"
    void (*pf_func)(struct ts_Scheduler_Control*);          //+ Function pointer "함수 포인터"
    tu32 v_case_step;                                       //+ Case execution location memory "case 동작 위치 기억"
    void *ps_user_struct;                                   //+ User structure registration "사용자 구조체 등록"
}ts_Scheduler_Control;      

#endif
