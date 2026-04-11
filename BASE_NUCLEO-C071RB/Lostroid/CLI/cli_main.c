/******************************************************************************
* File:    base_cli.c
* Author:  Lostroid
* Created: 2025-08-06
*
* Description:
* This is the CLI control.
*
* Revision History:
*   2025-08-06  New.
*   2025-08-13  v2.0
******************************************************************************/

#include "../DBG/dbg.h"
#include "../Base_LL/stm32c071xx.h"
#include "../Base_LL/base_ll_main_type.h"
#include "../Base_LL/base_ll_uart.h"
#include "cli_01_ae.h"
#include "cli_02_fj.h"
#include "cli_03_ko.h"
#include "cli_04_pt.h"
#include "cli_05_uz.h"
#include "cli_main.h"

#define d_CLI_SEND_TARGET(DATA,LEN)   f_LL_UART2_TX_Buff_Write(DATA,LEN)
static tu32 (*gfpa_base_cli_func[d_CLI_MAIN_FUNC_SIZE])(tu8** pa_Data) = {
    f_Cli_Check_a,   // 00
    f_Cli_Check_b,   // 01
    f_Cli_Check_c,   // 02
    f_Cli_Check_d,   // 03
    f_Cli_Check_e,   // 04
    f_Cli_Check_f,   // 05
    f_Cli_Check_g,   // 06
    f_Cli_Check_h,   // 07
    f_Cli_Check_i,   // 08
    f_Cli_Check_j,   // 09
    f_Cli_Check_k,   // 10
    f_Cli_Check_l,   // 11
    f_Cli_Check_m,   // 12
    f_Cli_Check_n,   // 13
    f_Cli_Check_o,   // 14
    f_Cli_Check_p,   // 15
    f_Cli_Check_q,   // 16
    f_Cli_Check_r,   // 17
    f_Cli_Check_s,   // 18
    f_Cli_Check_t,   // 19
    f_Cli_Check_u,   // 20
    f_Cli_Check_v,   // 21
    f_Cli_Check_w,   // 22
    f_Cli_Check_x,   // 23
    f_Cli_Check_y,   // 24
    f_Cli_Check_z    // 25
};

static tu8 gv_base_cli_cmd_historyBuff[d_CLI_MAIN_CMD_HISTORY_SIZE][d_CLI_MAIN_CMD_BUFF_SIZE] = {{0},};
static tu32 gv_base_cli_cmd_history_SetFlag;    // UP키로 선택여부 확인 변수 0: 미선택, 1: 선택
static tu32 gv_base_cli_cmd_history_RunPos;     /// UP키로 기존 내역 선택한 위치 정보
static tu32 gv_base_cli_cmd_history_NewPos;     /// 현재 저장할 위치
static tu32 gv_Base_Cli_CMD_DataPos;            /// 현재까지 저장된 문자열 위치치

static const tu8 cga_base_cli_string_help[] = "help";
static const tu8 cga_Base_Cli_String_Cam[] = "cam";
static const tu8 cga_Base_Cli_String_r[] = "r";
static const tu8 cga_Base_Cli_String_w[] = "w";
static const tu8 cga_Base_Cli_String_all[] = "all";
//----------------------------------------------------------------------------
// Command line interface Initialize
//----------------------------------------------------------------------------
void f_Base_Cli_Init(void)
{
    gv_base_cli_cmd_history_SetFlag = 0;
    gv_base_cli_cmd_history_RunPos = 0;
    gv_base_cli_cmd_history_NewPos = 0;
    gv_Base_Cli_CMD_DataPos = 1;         /// 0은 해당 명령어 길이정보 이므로 1로 초기화
}

//----------------------------------------------------------------------------
// Command line interface Module
//----------------------------------------------------------------------------
void f_base_cil_module(void)
{

}
//----------------------------------------------------------------------------
// Command name
//----------------------------------------------------------------------------
void f_Base_CLI_Name(void)
{
    f_DBG_Print_String(d_CLI_MAIN_CMD_NAME);
}
//----------------------------------------------------------------------------
// Message Error
//----------------------------------------------------------------------------
void f_Base_Cli_MsgError(te_CLI_MAIN_MSG e_Massage)
{
    switch(e_Massage)
    {
        case m_CLI_MAIN_MSG_NO_CMD :
        {
            f_DBG_Print_String("\r\n No Command.");
            break;
        }
        case m_CLI_MAIN_MSG_BUSY :
        {
            f_DBG_Print_String("\r\n Wait Busy.");
            break;
        }
        case m_CLI_MAIN_MSG_ERR :
        {
            f_DBG_Print_String("\r\n Error Command.");
            break;
        }
        case m_CLI_MAIN_MSG_LEN :
        {
            f_DBG_Print_String("\r\n Error Len.");
            break;
        }
        case m_CLI_MAIN_MSG_ADDR :
        {
            f_DBG_Print_String("\r\n Error Addr.");
            break;
        }
        default :
        {
            f_DBG_Print_String("\r\n Error MSG Code.");
            break;
        }
    }
    f_Base_CLI_Name();
}
//----------------------------------------------------------------------------
// Command line interface Module
//----------------------------------------------------------------------------
void f_Base_Cli_BuffCheck(tu8* pa_Data, ts_LL_Uart_Ctrol* s_Postion)
{
    /// 순환 버퍼에 command 를 수집 하여 정상적이면 다음 순환 위치로 가는 방식 입니다.
    /// command 가 잘못되면 현재 순환 버퍼를 다시 덥어쓰기 하여 사용 합니다.
    static tu8 v_Buff;
    static tu32 v_UpKey = 0;

    while(s_Postion->v_read_pos != s_Postion->v_write_pos)
    {
        v_Buff = pa_Data[s_Postion->v_read_pos];
        switch (v_Buff)
        {
            case 0x0D : //- Ascii [Enter] 
            {
                if(gv_Base_Cli_CMD_DataPos > 1)                              /// 데이터 하나라도 있으면 진행.
                {
                    gv_Base_Cli_CMD_DataPos++;                               /// 다음 배열(문자끝) NULL 삽입을 위해 공간 확인.
                    if(gv_Base_Cli_CMD_DataPos < d_CLI_MAIN_CMD_BUFF_SIZE)
                    {
                        gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_NewPos][gv_Base_Cli_CMD_DataPos] = 0;          /// NULL 삽입
                        if(f_Base_Cli_Command_Check(&gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_NewPos][1]) == 0)    /// 수신된 문자열 전송
                        {
                            gv_base_cli_cmd_history_NewPos++;                                                            /// 다음 순환버퍼 내역 이동동
                            if(gv_base_cli_cmd_history_NewPos >= d_CLI_MAIN_CMD_HISTORY_SIZE)                            /// 순환 버퍼 끝 확인.
                                { gv_base_cli_cmd_history_NewPos = 0; }
                            gv_Base_Cli_CMD_DataPos = 1;
                            gv_base_cli_cmd_history_SetFlag = 0;
                        }
                        else
                        {
                            /// Command Fail.
                            gv_Base_Cli_CMD_DataPos = 1;
                            gv_base_cli_cmd_history_SetFlag = 0;
                            f_Base_Cli_MsgError(m_CLI_MAIN_MSG_NO_CMD);
                        }
                    }
                    else
                    {
                        /// MAX Over
                        gv_Base_Cli_CMD_DataPos = 1;
                        gv_base_cli_cmd_history_SetFlag = 0;
                        f_Base_Cli_MsgError(m_CLI_MAIN_MSG_NO_CMD);
                    }
                }
                else
                {
                    gv_Base_Cli_CMD_DataPos = 1;
                    gv_base_cli_cmd_history_SetFlag = 0;
                    f_Base_CLI_Name();
                }
                break;
            }	
            case 0x7F : //- Ascii [BackSpace]
            {
                if(gv_Base_Cli_CMD_DataPos > 1)
                {
                    gv_Base_Cli_CMD_DataPos--;
                    d_CLI_SEND_TARGET(&v_Buff, 1);
                    gv_base_cli_cmd_history_SetFlag = 0;
                }
                break;
            }
            case 0x1B : //- Ascii [UP KEY]
            {
                v_UpKey = 1;
                break;
            }
            case 0x5B : //- Ascii [UP KEY]
            {
                if(v_UpKey == 1)
                    { v_UpKey = 2; }
                break;
            }
            case 0x41 : //- Ascii [UP KEY]
            {
                if(v_UpKey == 2)
                {
                    /// 현재 내역 위치에 이전내역을 복사 하는 방식으로 동작.
                    if(gv_base_cli_cmd_history_SetFlag == 0)
                    { 
                        /// UPKEY 처음시
                        gv_base_cli_cmd_history_SetFlag++; 
                        if(gv_base_cli_cmd_history_NewPos == 0)
                            { gv_base_cli_cmd_history_NewPos = d_CLI_MAIN_CMD_HISTORY_SIZE; }
                        gv_base_cli_cmd_history_RunPos = gv_base_cli_cmd_history_NewPos;
                        gv_base_cli_cmd_history_RunPos--;
                        tu32 v_While = gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_RunPos][0];
                        if(gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_RunPos][0] != 0)
                        {
                            while(v_While != 0)
                            {
                                v_While--;
                                gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_NewPos][v_While] 
                                    = gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_RunPos][v_While];
                                d_CLI_SEND_TARGET(&gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_NewPos][1]
                                    , gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_NewPos][0]);
                            }
                        }
                        else
                            { gv_base_cli_cmd_history_SetFlag = 0; }
                    }
                    else
                    {
                        /// UPKEY 연속시
                        if(gv_base_cli_cmd_history_RunPos == 0)
                        { gv_base_cli_cmd_history_RunPos = d_CLI_MAIN_CMD_HISTORY_SIZE; }
                        gv_base_cli_cmd_history_RunPos--;
                        tu32 v_While = gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_RunPos][0];
                        if(gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_RunPos][0] != 0)
                        {
                            while(v_While != 0)
                            {
                                v_While--;
                                gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_NewPos][v_While] 
                                    = gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_RunPos][v_While];
                                d_CLI_SEND_TARGET(&gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_NewPos][1]
                                    , gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_NewPos][0]);
                            }
                        }
                        else
                            { gv_base_cli_cmd_history_SetFlag = 0; }
                    }
                }
                v_UpKey = 0;
                break;
            }
            default :
            {
                gv_base_cli_cmd_historyBuff[gv_base_cli_cmd_history_NewPos][gv_Base_Cli_CMD_DataPos] = v_Buff;
                d_CLI_SEND_TARGET(&v_Buff, 1);
                gv_Base_Cli_CMD_DataPos++;
                if(gv_Base_Cli_CMD_DataPos >= d_CLI_MAIN_CMD_BUFF_SIZE)
                {
                    gv_Base_Cli_CMD_DataPos--;
                    f_DBG_Print_String("W: Buff Over.");
                    /// MAX OVER
                }
                break;
            }
        }

        if( s_Postion->v_read_pos == s_Postion->cv_max_size)
            { s_Postion->v_read_pos = 0; }
        else
            { s_Postion->v_read_pos++; }
    }
}
//----------------------------------------------------------------------------
// Command line interface Module
//----------------------------------------------------------------------------
tu32 f_Base_Cli_Command_Check(tu8* pa_Data)
{
    tu32 v_error = 1;
    tu8 *p_Token[d_CLI_MAIN_CMD_BUFF_SIZE] = {0};
    tu8 v_Cnt = 0;
    tu8 v_Get_Token = 0;
    //- string token
    for(tu8 v_For = 0; v_For < d_CLI_MAIN_CMD_BUFF_SIZE; v_For++)
    {
        /// I2C r 300 => [0] = I2C\0, [1] = r\0   포인터 위치 만 저장.
        if(pa_Data[v_For] != 0)                   //- Check String data null
        {
            if(pa_Data[v_For] == (tu8)' ')
            {
                pa_Data[v_For] = 0;                  //- ' ' = NULL modify
                v_Get_Token = 0;
            }
            else if(v_Get_Token == 0)
            {
                p_Token[v_Cnt] = &pa_Data[v_For];
                v_Get_Token++;
                v_Cnt++;
            }
        }
        else
            { break; }
    }

    tu8 *v_Check = p_Token[0];
    if((v_Check[0] != 0) && (v_Check != 0))
    {
        tu8 v_Data = pa_Data[0];
        if((v_Data >= (tu8)'a') && (v_Data <= (tu8)'z') )
        {
            v_Data -= (tu8)'a'; //- 배열 자동 변환
            v_error = gfpa_base_cli_func[v_Data](p_Token);
        }
        else
            { v_error++; }
    }
    else
        { f_Base_CLI_Name(); }
    
    return v_error;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Compare String
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
tu32 f_Base_Cli_CompareString(tu8 *p_Data1, const tu8 *p_Data2)
{
    tu32 v_Err = 0;
    tu8 v_Loop;
    for(v_Loop = 0; v_Loop < 32; v_Loop++)  //- Max 32
    {
        if( (p_Data1[v_Loop] != 0) && (p_Data2[v_Loop] != 0) )
        {
            if(p_Data1[v_Loop] != p_Data2[v_Loop]) 
                {break;}
        }
        else 
            {break;}
    }
    if(p_Data1[v_Loop] != p_Data2[v_Loop]) 
        {v_Err++;}
    return(v_Err);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  String Dec -> Dec ("999999" -> 999999)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
tu32 f_Base_Cli_StringToDec(tu8 *p_Data)
{
    tu32 v_Data = 0xFFFFFFFFu;	//- Set Err Velue.
    tu8 v_Pos = 0;
    if(p_Data != d_CLI_MAIN_NULL)
    {
        v_Data = 0;
        while(v_Pos < 10)
        {
            if((p_Data[v_Pos] >= (tu8)'0') && (p_Data[v_Pos] <= (tu8)'9'))
            {
                v_Data *= 10; 
                v_Data += (p_Data[v_Pos] - (tu8)'0');
            }
            else
            {
                if(p_Data[v_Pos] != 0)
                    { v_Data = 0xFFFFFFFFu; }	//- Set Err Velue.
                break;
            }
            v_Pos++;
        }
    }
    return(v_Data);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  String Hex -> Dec ("FF" -> 255)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
tu32 f_Base_Cli_StringToHex(tu8 *p_Data)
{
    tu32 v_Data = 0xFFFFFFFFu;     //- Set Err Velue.
    tu8 v_Pos = 0;
    if(p_Data != d_CLI_MAIN_NULL)
    {
        v_Data = 0;
        while(v_Pos < 10)
        {
            if((p_Data[v_Pos] >= (tu8)'0') && (p_Data[v_Pos] <= (tu8)'9'))
            {
                v_Data <<= 4; 
                v_Data += (p_Data[v_Pos] - (tu8)'0');     //- 0 = '0'(48) - '0'(48)
            }
            else if((p_Data[v_Pos] >= (tu8)'a') && (p_Data[v_Pos] <= (tu8)'f'))
            {
                v_Data <<= 4; 
                v_Data += (p_Data[v_Pos] - (tu8)'W');     //- 10 = 'a'(97) - 'W'(87)
            }
            else if((p_Data[v_Pos] >= (tu8)'A') && (p_Data[v_Pos] <= (tu8)'F'))
            {
                v_Data <<= 4; 
                v_Data += (p_Data[v_Pos] - (tu8)'7');     //- 10 = 'A'(65) - '7'(55)
            }
            else
            {
                if(p_Data[v_Pos] != 0)
                    {v_Data = 0xFFFFFFFFu;}	//- Set Err Velue.
                break;
            }
            v_Pos++;
        }
    }
    return(v_Data);
}
