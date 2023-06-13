/***********************************************************************************************************************
 * File Name    : elc_hal.c
 * Description  : Contains function definition.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

#include "common_utils.h"
#include "elc_hal.h"

/*******************************************************************************************************************//**
 * @addtogroup elc_hal.c
 * @{
 **********************************************************************************************************************/

/*****************************************************************************************************************
 * @brief       Initializes ELC module instances.
 * @param[IN]   p_ctrl_instance_elc         ELC instance control structure.
 * @param[IN]   p_cfg_instance_elc          ELC instance config  structure.
 * @retval      FSP_SUCCESS                 ELC instance opened successfully.
 * @retval      Any Other Error code apart from FSP_SUCCES on Unsuccessful opening of elc instance .
*****************************************************************************************************************/
fsp_err_t init_hal_elc(elc_ctrl_t * const p_ctrl_instance_elc, elc_cfg_t const * const p_cfg_instance_elc)
{
    fsp_err_t err = FSP_SUCCESS;
    /*Open elc instance */
    err = R_ELC_Open(p_ctrl_instance_elc, p_cfg_instance_elc);
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\n** R_ELC_Open FAILED ** \r\n");
    }
    return err;
}

/*****************************************************************************************************************
 * @brief       Enable ELC module instances.
 * @param[IN]   p_ctrl_instance_elc         ELC instance control structure.
 * @retval      FSP_SUCCESS                 ELC instance enabled successfully.
 * @retval      Any Other Error code apart from FSP_SUCCES on Unsuccessful enable of elc instance.
*****************************************************************************************************************/
fsp_err_t elc_enable(elc_ctrl_t * const p_ctrl_instance_elc)
{
    fsp_err_t err = FSP_SUCCESS;
    /* Enable the operation of the Event Link Controller */
    err = R_ELC_Enable(p_ctrl_instance_elc);
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\n** R_ELC_Enable FAILED ** \r\n");
    }
    return err;
}

/*******************************************************************************************************************//**
 * @brief       This functions closes ELC instance.
 * @param[IN]   p_ctrl_instance_elc         ELC control instance
 * @retval      FSP_SUCCESS                 Upon successful close operation.
 * @retval      Any Other Error code apart from FSP_SUCCESS
 **********************************************************************************************************************/
void deinit_hal_elc(elc_ctrl_t * const p_ctrl_instance_elc)
{
    fsp_err_t err = FSP_SUCCESS;
    /* close opened elc instance */
    err = R_ELC_Close(p_ctrl_instance_elc);
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\n** R_ELC_Close FAILED ** \r\n");
    }
}


/*******************************************************************************************************************//**
 * @} (end addtogroup elc_hal.c)
 **********************************************************************************************************************/
