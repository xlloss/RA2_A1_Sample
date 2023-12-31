/***********************************************************************************************************************
 * File Name    : hal_entry.c
 * Description  : Contains data structures and functions used in hal_entry.c.
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
#include "icu_ep.h"
#include "elc_hal.h"
#include "dtc_hal.h"

/*******************************************************************************************************************//**
 * @addtogroup icu_ep
 * @{
 **********************************************************************************************************************/


FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
FSP_CPP_FOOTER

fsp_err_t adc_read_data(void);
void adc_task(void);

volatile bool b_ready_to_read = false;
volatile bool b_ready_to_scan = false;
uint16_t g_adc_data;

static fsp_err_t adc_scan_start(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    if (false == b_ready_to_read)
    {
        /* Open/Initialize ADC module */
        err = R_ADC_Open (&g_adc_ctrl, &g_adc_cfg);
        if (FSP_SUCCESS != err)
        {
            /* ADC Failure message */
            APP_ERR_PRINT("** R_ADC_Open API failed ** \r\n");
            while (1);
            return err;
        }

        /* Configures the ADC scan parameters */
        err = R_ADC_ScanCfg (&g_adc_ctrl, &g_adc_channel_cfg);
        if (FSP_SUCCESS != err)
        {
            /* ADC Failure message */
            APP_ERR_PRINT("** R_ADC_ScanCfg API failed ** \r\n");
            while (1);
            return err;
        }

        /* Start the ADC scan*/
        err = R_ADC_ScanStart (&g_adc_ctrl);
        if (FSP_SUCCESS != err)
        {
            /* ADC Failure message */
            APP_ERR_PRINT("** R_ADC_ScanStart API failed ** \r\n");
            while (1);
            return err;
        }

        /* APP_PRINT("\r\nADC Started Scan\r\n"); */
        b_ready_to_read = true;
    }
    else
    {
        APP_PRINT("\r\nADC Scan already in progress\r\n");
    }

    return err;
}

fsp_err_t adc_read_data(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_05, BSP_IO_LEVEL_LOW);
    err = R_ADC_Read (&g_adc_ctrl, ADC_CHANNEL_0, &g_adc_data);
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_05, BSP_IO_LEVEL_HIGH);

    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** R_ADC_Read API failed ** \r\n");
        return err;
    }

    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_05, BSP_IO_LEVEL_LOW);
    return err;
}

void adc_task(void)
{
    fsp_err_t err = FSP_SUCCESS;

    R_BSP_SoftwareDelay(250, BSP_DELAY_UNITS_MICROSECONDS);

    err = adc_scan_start();
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** adc_scan_start FAILED ** \r\n");
        return;
    }

    err = adc_read_data();
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** adc_read_data FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }
}

void hal_entry(void)
{
    fsp_err_t err = FSP_SUCCESS;
    fsp_pack_version_t version = {RESET_VALUE};

    /* version get API for FLEX pack information */
    R_FSP_VersionGet(&version);

    APP_PRINT(BANNER_INFO, EP_VERSION, version.version_id_b.major,
        version.version_id_b.minor, version.version_id_b.patch);

    APP_PRINT(EP_INFO);

    /* Initialize External IRQ driver*/
    err = icu_init();
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** ICU INIT FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }

    /* Initialize all the links in the Event Link Controller */
    err = init_hal_elc(&g_elc_ctrl,&g_elc_cfg);
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\n** init_hal_elc FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }

    /* Enable the operation of the Event Link Controller */
    err = elc_enable(&g_elc_ctrl);
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** R_ELC_Enable FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }

    err = icu_enable();
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** ICU ENABLE FAILED ** \r\n");
        icu_deinit();
        APP_ERR_TRAP(err);
    }

    err = R_GPT_Open(&g_timer_pwm_ctrl, &g_timer_pwm_cfg);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** R_GPT_Open FAILED ** \r\n");
        return;
    }

    err = R_GPT_Start(&g_timer_pwm_ctrl);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** R_GPT_Start FAILED ** \r\n");
        return;
    }

    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_05, BSP_IO_LEVEL_LOW);

    err = adc_scan_start();
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** adc_scan_start FAILED ** \r\n");
        return;
    }

    /* Main loop */
    while (true)
    {
        if (b_ready_to_scan == true)
        {
            err = adc_read_data();
            if (FSP_SUCCESS != err)
            {
                APP_ERR_PRINT("** adc_read_data FAILED ** \r\n");
                APP_ERR_TRAP(err);
            }
            b_ready_to_scan = false;
            APP_PRINT("\r\n_adc_data %d\r\n", g_adc_data);
        }
    }
}

void R_BSP_WarmStart(bsp_warm_start_event_t event) {
    if (BSP_WARM_START_RESET == event) {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event) {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);
    }
}

/*******************************************************************************************************************//**
 * @} (end addtogroup icu_ep)
 **********************************************************************************************************************/


void adc_callback(adc_callback_args_t *p_args)
{
    /* APP_PRINT("** adc_callback Trigger ** \r\n"); */

    switch(p_args->event)
    {
        case ADC_EVENT_SCAN_COMPLETE:
        {
            /* APP_PRINT("** ADC_EVENT_SCAN_COMPLETE ** \r\n"); */
            /*do nothing*/
            b_ready_to_scan = true;
        }
        break;

        case ADC_EVENT_CONVERSION_COMPLETE:
        {
            /* APP_PRINT("** ADC_EVENT_CONVERSION_COMPLETE ** \r\n"); */
            /*do nothing*/
        }
        break;

        default:
        {

        }
        break;
    }
}
